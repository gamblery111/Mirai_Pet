#include "PetRenderer.h"
#include "PetModelController.h"
#include <saba/Model/MMD/MMDModel.h>
#include <saba/Model/MMD/MMDMaterial.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QDebug>

PetRenderer::PetRenderer(QWidget* parent)
	: QOpenGLWidget(parent)
{
	m_modelCtrl = std::make_unique<PetModelController>();
	m_view = glm::lookAt(glm::vec3(0, 8, 25), glm::vec3(0, 8, 0), glm::vec3(0, 1, 0));
}

PetRenderer::~PetRenderer()
{
	makeCurrent();
	for (auto& p : m_texCache) {
		delete p.second;
	}
	m_texCache.clear();
	delete m_toonRamp;
	doneCurrent();
}

void PetRenderer::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// simple vertex/fragment shader (same logic as your previous shader)
	const char* vs = R"(
		#version 330 core
		layout(location=0) in vec3 aPos;
		layout(location=1) in vec3 aNor;
		layout(location=2) in vec2 aUV;
		out vec3 vPos; out vec3 vNor; out vec2 vUV;
		uniform mat4 uMVP; 
		uniform mat4 uMV; 
		uniform mat3 uN;
		void main(){
		gl_Position = uMVP * vec4(aPos,1.0);
		vPos = (uMV * vec4(aPos,1.0)).xyz;
		vNor = uN * aNor;
		vUV = aUV;
		}
	)";

	// **片段着色器修正版本**
	const char* fs = R"(
		#version 330 core
		in vec3 vPos; in vec3 vNor; in vec2 vUV;
		out vec4 fragColor;
		uniform sampler2D uTex;
		uniform sampler1D uToon;
		uniform vec4 uDiff;
		uniform float uAlpha;
		uniform vec3 uLightDir = vec3(0.5,1.0,0.5); 
		uniform vec3 uAmbient = vec3(0.55);
		void main(){
			vec4 tex = texture(uTex, vUV);
			float ndl = max(dot(normalize(vNor), normalize(uLightDir)), 0.0);
			float toon = texture(uToon, ndl).r;
			float light_factor = toon;
			vec3 lit_diffuse = uDiff.rgb * light_factor + uAmbient;
			vec3 final_color = lit_diffuse * tex.rgb;
			fragColor = vec4(final_color, uDiff.a * uAlpha * tex.a);
		}
	)";

	m_shader.addShaderFromSourceCode(QOpenGLShader::Vertex, vs);
	m_shader.addShaderFromSourceCode(QOpenGLShader::Fragment, fs);
	if (!m_shader.link()) {
		qDebug() << "Shader link failed:" << m_shader.log();
	}

	// create toon ramp 1D texture
	m_toonRamp = new QOpenGLTexture(QOpenGLTexture::Target1D);
	m_toonRamp->create();
	m_toonRamp->setSize(256);
	m_toonRamp->setFormat(QOpenGLTexture::R8_UNorm);
	m_toonRamp->allocateStorage();
	std::vector<uint8_t> ramp(256);
	for (int i = 0; i < 256; ++i) {
		float v = i / 255.0f;
		if (v > 0.98f)      ramp[i] = 255;
		else if (v > 0.65f) ramp[i] = 220;
		else if (v > 0.05f) ramp[i] = 80;   // 暗部回到 80 左右
		else                ramp[i] = 30;   // 最暗部 30
	}
	m_toonRamp->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, ramp.data());

	// create buffers & vao
	m_vbo.create();
	m_ibo.create();
	m_vao.create();

	m_timerId = startTimer(16);
}

void PetRenderer::resizeGL(int w, int h)
{
	m_proj = glm::perspective(glm::radians(45.0f), float(w) / qMax(h, 1), 0.1f, 1000.0f);
}

void PetRenderer::ensureBuffers(size_t vcount, size_t indexCount)
{
	// allocate/resize VBO & IBO when vertex count changes
	if (m_vboVertexCount != vcount) {
		m_vbo.bind();
		m_vbo.allocate(nullptr, vcount * sizeof(Vertex)); // allocate size only
		m_vboVertexCount = vcount;
		m_vbo.release();
	}

	if (m_ibo.size() == 0 && indexCount > 0) {
		m_ibo.bind();
		m_ibo.allocate(indexCount * sizeof(uint16_t));
		m_ibo.release();
	}
}

QOpenGLTexture* PetRenderer::loadOrGetTexture(const std::string& path)
{
	if (path.empty()) return nullptr;
	auto it = m_texCache.find(path);
	if (it != m_texCache.end()) return it->second;

	QImage img(QString::fromStdString(path));
	if (img.isNull()) {
		qDebug() << "Failed to load image:" << QString::fromStdString(path);
		m_texCache[path] = nullptr;
		return nullptr;
	}
	// convert to RGBA
	img = img.convertToFormat(QImage::Format_RGBA8888);
	QOpenGLTexture* tex = new QOpenGLTexture(img);

	tex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	tex->setMagnificationFilter(QOpenGLTexture::Linear);
	tex->setWrapMode(QOpenGLTexture::Repeat);

	tex->generateMipMaps();

	m_texCache[path] = tex;
	return tex;
}

void PetRenderer::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto* model = m_modelCtrl->GetModel();
	if (!model) return;

	// update animation
	m_modelCtrl->Update(1.0f / 60.0f);

	const auto* pos = model->GetPositions();
	const auto* nor = model->GetNormals();
	const auto* uv = model->GetUVs();
	size_t vcount = model->GetVertexCount();

	// indices
	const void* indices = model->GetIndices();
	size_t indexCount = model->GetIndexCount();

	// ensure buffers sized
	ensureBuffers(vcount, indexCount);

	// build interleaved vertex array
	std::vector<Vertex> verts;
	verts.resize(vcount);
	for (size_t i = 0; i < vcount; ++i) {
		verts[i].pos = pos[i];
		verts[i].nor = nor[i];
		verts[i].uv = uv[i];
	}

	// upload vertex data
	m_vbo.bind();
	// overwrite whole buffer (safe)
	m_vbo.write(0, verts.data(), static_cast<int>(vcount * sizeof(Vertex)));
	m_vbo.release();

	// upload indices once (if empty or changed)
	m_ibo.bind();
	if (m_ibo.size() == 0) {
		m_ibo.allocate(indices, static_cast<int>(indexCount * sizeof(uint16_t)));
	}
	m_ibo.release();

	m_vao.bind();
	m_vbo.bind();
	m_ibo.bind();

	m_shader.bind();

	glm::mat4 mvp = m_proj * m_view;
	m_shader.setUniformValue("uMVP", QMatrix4x4(glm::value_ptr(mvp)).transposed());
	m_shader.setUniformValue("uMV", QMatrix4x4(glm::value_ptr(m_view)).transposed());
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(m_view)));
	m_shader.setUniformValue("uN", QMatrix3x3(glm::value_ptr(normalMatrix)).transposed());

	// bind toon ramp to unit 1
	if (m_toonRamp) {
		m_toonRamp->bind(1);
		m_shader.setUniformValue("uToon", 1);
	}

	// vertex attrib pointers: interleaved
	const GLsizei stride = sizeof(Vertex);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, pos)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, nor)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, uv)));

	const auto* subMeshes = model->GetSubMeshes();
	const saba::MMDMaterial* materials = model->GetMaterials();

	for (size_t i = 0; i < model->GetSubMeshCount(); ++i) {
		const auto& sub = subMeshes[i];
		const auto& mat = materials[i];

		// set diffuse + alpha
		m_shader.setUniformValue("uDiff", QVector4D(mat.m_diffuse.r, mat.m_diffuse.g, mat.m_diffuse.b, 1.0f));
		m_shader.setUniformValue("uAlpha", static_cast<float>(mat.m_alpha));

		std::string texPath;
		try {
			texPath = mat.m_texture; // if this compiles and exists
		}
		catch (...) {
		}
		QOpenGLTexture* tex = nullptr;
		if (!texPath.empty()) tex = loadOrGetTexture(texPath);

		glActiveTexture(GL_TEXTURE0);
		if (tex) tex->bind();
		else glBindTexture(GL_TEXTURE_2D, 0); 
		m_shader.setUniformValue("uTex", 0);

		glDrawElements(GL_TRIANGLES,
			static_cast<GLsizei>(sub.m_vertexCount),
			GL_UNSIGNED_SHORT,
			reinterpret_cast<void*>(sub.m_beginIndex * sizeof(uint16_t)));

		if (tex) tex->release();
	}

	m_shader.release();
	m_ibo.release();
	m_vbo.release();
	m_vao.release();
}

void PetRenderer::timerEvent(QTimerEvent* evt)
{
	if (evt->timerId() == m_timerId) update();
}

bool PetRenderer::LoadModel(const std::string& path, const std::string& mmdDir)
{
	return m_modelCtrl->LoadModel(path, mmdDir);
}

bool PetRenderer::LoadMotion(const std::string& path)
{
	return m_modelCtrl->LoadMotion(path);
}