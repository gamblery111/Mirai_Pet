#include "PetRenderer.h"
#include "PetModelController.h"

#include <saba/Model/MMD/MMDModel.h>
#include <saba/Model/MMD/MMDMaterial.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QTimerEvent>

PetRenderer::PetRenderer(QWidget *parent)
	: QOpenGLWidget(parent)
	, m_ibo(QOpenGLBuffer::IndexBuffer)
	, m_vbo(QOpenGLBuffer::VertexBuffer)
{
	m_modelCtrl = std::make_unique<PetModelController>();

	m_view = glm::lookAt(glm::vec3(0, 8, 25), glm::vec3(0, 8, 0), glm::vec3(0, 1, 0));

}

PetRenderer::~PetRenderer()
{
	makeCurrent();
	delete m_toonRamp;
	doneCurrent();
}

void PetRenderer::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // 改成全透明！！！

	glEnable(GL_DEPTH_TEST);
	 glEnable(GL_CULL_FACE); // MMD模型通常建议关闭剔除，或者小心使用，因为有些裙子是双面的

	// ========== 极简 Toon Shader（硬编码）==========
	const char* vs = R"(
        #version 330 core
        layout(location=0) in vec3 aPos;
        layout(location=1) in vec3 aNor;
        layout(location=2) in vec2 aUV;
        out vec3 vPos; out vec3 vNor; out vec2 vUV;
        uniform mat4 uMVP; uniform mat4 uMV;
        void main(){
            gl_Position = uMVP * vec4(aPos,1.0);
            vPos = (uMV * vec4(aPos,1.0)).xyz;
            vNor = mat3(uMV) * aNor;
            vUV = aUV;
        }
    )";

	const char* fs = R"(
        #version 330 core
        in vec3 vPos; in vec3 vNor; in vec2 vUV;
        out vec4 fragColor;
        uniform sampler2D uTex;        // 主纹理（可不绑，用纯色也行）
        uniform sampler1D uToon;       // 1D toon ramp
        uniform vec4  uDiff;
        uniform float uAlpha;
        uniform vec3  uLightDir = vec3(0.5,1.0,0.5);
        void main(){
            vec4 tex = texture(uTex, vUV);
			//vec4 tex = vec4(1.0);
            float ndl = max(dot(normalize(vNor), normalize(uLightDir)), 0.0);
            float toon = texture(uToon, ndl).r;
            vec3 color = uDiff.rgb * toon;
            fragColor = vec4(color, uDiff.a * uAlpha) * tex;
        }
    )";

	m_shader.addShaderFromSourceCode(QOpenGLShader::Vertex, vs);
	m_shader.addShaderFromSourceCode(QOpenGLShader::Fragment, fs);
	if (!m_shader.link()) {
		qDebug() << "Shader link failed:" << m_shader.log();
	}

	// ========== 1D Toon Ramp（完美替代 10 张 bmp）==========
	m_toonRamp = new QOpenGLTexture(QOpenGLTexture::Target1D);
	m_toonRamp->create();
	m_toonRamp->setSize(256);
	m_toonRamp->setFormat(QOpenGLTexture::R8_UNorm);
	m_toonRamp->allocateStorage();

	std::vector<uint8_t> ramp(256);
	for (int i = 0; i < 256; ++i) {
		float v = i / 255.0f;
		if (v > 0.95f)      ramp[i] = 255;
		else if (v > 0.4f)  ramp[i] = 190;
		else                ramp[i] = 70;
	}
	m_toonRamp->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, ramp.data());

	// ========== 创建 VBO/IBO（在加载模型后真正分配）==========
	m_vbo.create();
	m_ibo.create();
	m_vao.create();

	m_timerId = startTimer(16); // ~60fps

}

void PetRenderer::resizeGL(int w, int h)
{
	m_proj = glm::perspective(glm::radians(45.0f), float(w) / qMax(h, 1), 0.1f, 1000.0f);
}

void PetRenderer::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto* model = m_modelCtrl->GetModel();
	if (!model) return;

	// 1. 更新动画
	m_modelCtrl->Update(1.0f / 60.0f);

	// 2. 上传最新顶点数据
	const auto* pos = model->GetPositions();
	const auto* nor = model->GetNormals();
	const auto* uv = model->GetUVs();
	size_t vcount = model->GetVertexCount();

	m_vbo.bind();
	if (m_vbo.size() == 0) {
		// 第一次分配
		m_vbo.allocate(vcount * (3 + 3 + 2) * sizeof(float));
		m_ibo.bind();
		m_ibo.allocate(model->GetIndices(), model->GetIndexCount() * sizeof(uint32_t));
	}
	m_vbo.write(0, pos, vcount * sizeof(glm::vec3));
	m_vbo.write(vcount * sizeof(glm::vec3), nor, vcount * sizeof(glm::vec3));
	m_vbo.write(vcount * sizeof(glm::vec3) * 2, uv, vcount * sizeof(glm::vec2));

	// 3. 绑定 VAO + Shader
	m_vao.bind();
	m_shader.bind();

	glm::mat4 mvp = m_proj * m_view;
	m_shader.setUniformValue("uMVP", QMatrix4x4(glm::value_ptr(mvp)));
	m_shader.setUniformValue("uMV", QMatrix4x4(glm::value_ptr(m_view)));
	m_toonRamp->bind(1);
	m_shader.setUniformValue("uToon", 1);

	// 属性指针
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vcount * sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(vcount * sizeof(glm::vec3) * 2));
	m_ibo.bind();

	// 按材质绘制（最简单版：用材质 diffuse 色）
	const auto* subMeshes = model->GetSubMeshes();       
	// 获取材质数组（只用来取颜色、纹理路径等）
	const saba::MMDMaterial* materials = model->GetMaterials();

	size_t indexOffset = 0;
	for (size_t i = 0; i < model->GetSubMeshCount(); ++i)
	{
		const auto& subMesh = subMeshes[i];
		const auto& mat = materials[i];   // 一一对应

		// 设置当前材质的颜色（diffuse + alpha）
		m_shader.setUniformValue("uDiff", QVector4D(mat.m_diffuse.r, mat.m_diffuse.g, mat.m_diffuse.b, 1.0f));
		m_shader.setUniformValue("uAlpha", mat.m_alpha);

		// 绘制这个 SubMesh
		glDrawElements(GL_TRIANGLES,
			subMesh.m_vertexCount,           // 关键！顶点数量（不是三角形数）
			GL_UNSIGNED_INT,
			(void*)(subMesh.m_beginIndex * sizeof(uint32_t)));  // 开始索引
	}

	m_shader.release();
	m_vao.release();
	m_vbo.release();
	m_ibo.release();
}

void PetRenderer::timerEvent(QTimerEvent* evt)
{
	if (evt->timerId() == m_timerId)
		update();   // 触发 paintGL
}

bool PetRenderer::LoadModel(const std::string& path, const std::string& mmdDir)
{
	return m_modelCtrl->LoadModel(path, mmdDir);
}

bool PetRenderer::LoadMotion(const std::string& path)
{
	return m_modelCtrl->LoadMotion(path);
}