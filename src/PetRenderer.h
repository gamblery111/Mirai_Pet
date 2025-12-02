#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QTimerEvent>
#include <QImage>
#include <memory>
#include <unordered_map>


#include <glm/glm.hpp>


class PetModelController; // forward

class PetRenderer : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT
public:
	explicit PetRenderer(QWidget* parent = nullptr);
	~PetRenderer() override;


	bool LoadModel(const std::string& path, const std::string& mmdDir);
	bool LoadMotion(const std::string& path);


protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void timerEvent(QTimerEvent* evt) override;


private:
	struct Vertex {
		glm::vec3 pos;
		glm::vec3 nor;
		glm::vec2 uv;
	};


	void ensureBuffers(size_t vcount, size_t indexCount);
	GLuint loadTextureFromImage(const QImage& img);
	QOpenGLTexture* loadOrGetTexture(const std::string& path);
	QOpenGLTexture* m_toonRamp = nullptr;


	std::unique_ptr<PetModelController> m_modelCtrl;


	glm::mat4 m_proj;
	glm::mat4 m_view;


	QOpenGLBuffer m_vbo{ QOpenGLBuffer::VertexBuffer };
	QOpenGLBuffer m_ibo{ QOpenGLBuffer::IndexBuffer };
	QOpenGLVertexArrayObject m_vao;


	QOpenGLShaderProgram m_shader;


	std::unordered_map<std::string, QOpenGLTexture*> m_texCache;


	int m_timerId = 0;
	size_t m_vboVertexCount = 0;
};