#include "PetShaderManager.h"

namespace miraipet::render
{

    PetShaderManager::PetShaderManager()
        : m_mmdShader(nullptr), m_toonTexture(nullptr)
    {
        InitMMDShader();
        CreateToonTexture();
    }

    PetShaderManager::~PetShaderManager()
    {
        delete m_mmdShader;
        delete m_toonTexture;
    }

    QOpenGLShaderProgram *PetShaderManager::GetMMDShader()
    {
        return m_mmdShader;
    }

    QOpenGLTexture *PetShaderManager::GetToonTexture()
    {
        return m_toonTexture;
    }

    void PetShaderManager::InitMMDShader()
    {
        m_mmdShader = new QOpenGLShaderProgram();
        const char *vs = R"(
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
        const char *fs = R"(
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

        m_mmdShader->addShaderFromSourceCode(QOpenGLShader::Vertex, vs);
        m_mmdShader->addShaderFromSourceCode(QOpenGLShader::Fragment, fs);
        m_mmdShader->link();
    }

    void PetShaderManager::CreateToonTexture()
    {
        // 创建toon纹理：简单的4阶梯toon shading
        // 0.0-0.25: 0.2, 0.25-0.5: 0.5, 0.5-0.75: 0.8, 0.75-1.0: 1.0
        const int toonSize = 256;
        std::vector<float> toonData(toonSize);

        for (int i = 0; i < toonSize; ++i) {
            float intensity = static_cast<float>(i) / (toonSize - 1);
            if (intensity < 0.25f) {
                toonData[i] = 0.2f;
            } else if (intensity < 0.5f) {
                toonData[i] = 0.5f;
            } else if (intensity < 0.75f) {
                toonData[i] = 0.8f;
            } else {
                toonData[i] = 1.0f;
            }
        }

        m_toonTexture = new QOpenGLTexture(QOpenGLTexture::Target1D);
        m_toonTexture->setSize(toonSize);
        m_toonTexture->setFormat(QOpenGLTexture::R32F);
        m_toonTexture->allocateStorage();
        m_toonTexture->setData(QOpenGLTexture::Red, QOpenGLTexture::Float32, toonData.data());
        m_toonTexture->setMinificationFilter(QOpenGLTexture::Linear);
        m_toonTexture->setMagnificationFilter(QOpenGLTexture::Linear);
        m_toonTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
    }
}