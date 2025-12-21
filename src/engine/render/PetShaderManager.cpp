#include "PetShaderManager.h"

namespace miraipet::render
{

    PetShaderManager::PetShaderManager()
    {
    }

    PetShaderManager::~PetShaderManager()
    {
    }

    QOpenGLShaderProgram *miraipet::render::PetShaderManager::CreateBasicShader()
    {
        auto *program = new QOpenGLShaderProgram();

        const char *vert = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
        }
        )";

        const char *frag = R"(
        #version 330 core
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(0.8, 0.3, 0.2, 1.0);
        }
        )";

        program->addShaderFromSourceCode(QOpenGLShader::Vertex, vert);
        program->addShaderFromSourceCode(QOpenGLShader::Fragment, frag);
        program->link();
        return program;
    }

    QOpenGLShaderProgram *PetShaderManager::CreateMMDShader()
    {
        auto *program = new QOpenGLShaderProgram();

        const char *vert = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aNormal;
        layout(location = 2) in vec2 aTexCoord;
        
        out vec3 FragPos;
        out vec3 Normal;
        out vec2 TexCoord;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        void main()
        {
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;
            TexCoord = aTexCoord;
            
            gl_Position = projection * view * vec4(FragPos, 1.0);
        }
        )";

        const char *frag = R"(
        #version 330 core
        out vec4 FragColor;
        
        in vec3 FragPos;
        in vec3 Normal;
        in vec2 TexCoord;
        
        uniform sampler2D texture1;
        
        void main()
        {
            // 简单的漫反射光照
            vec3 lightPos = vec3(1.0, 1.0, 2.0);
            vec3 lightColor = vec3(1.0, 1.0, 1.0);
            vec3 objectColor = texture(texture1, TexCoord).rgb;
            
            // 环境光
            float ambientStrength = 0.2;
            vec3 ambient = ambientStrength * lightColor;
            
            // 漫反射
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;
            
            vec3 result = (ambient + diffuse) * objectColor;
            FragColor = vec4(result, 1.0);
        }
        )";

        program->addShaderFromSourceCode(QOpenGLShader::Vertex, vert);
        program->addShaderFromSourceCode(QOpenGLShader::Fragment, frag);
        program->link();
        return program;
    }

}