#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PetModelController;

class PetRenderer  : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
private:
	std::unique_ptr<PetModelController> m_modelCtrl;

	// OpenGL 对象
	QOpenGLShaderProgram    m_shader;
	QOpenGLShaderProgram    m_edgeShader;   // 可选
	QOpenGLBuffer           m_vbo;
	QOpenGLBuffer           m_ibo;
	QOpenGLVertexArrayObject m_vao;
	QOpenGLTexture* m_toonRamp{ nullptr };   // 1D toon 渐变纹理

	glm::mat4 m_proj{};
	glm::mat4 m_view{};

	int m_timerId{ 0 };

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void timerEvent(QTimerEvent* evt) override;

public:
	explicit PetRenderer(QWidget* parent = nullptr);
	~PetRenderer();

	// 外部调用
	bool LoadModel(const std::string& path, const std::string& mmdDir = "");
	bool LoadMotion(const std::string& path);

private:
	void SetupVAO(size_t vcount);
};

