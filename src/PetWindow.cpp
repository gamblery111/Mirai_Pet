#include "PetWindow.h"
#include "PetRenderer.h"

#include <QVBoxLayout>
#include <QMouseEvent>
#include <QScreen>
#include <QGuiApplication>



PetWindow::PetWindow(QWidget *parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint |  
		Qt::WindowStaysOnTopHint |  
		Qt::Tool | 
		Qt::WindowTransparentForInput); 

	setAttribute(Qt::WA_TranslucentBackground, true);  
	setAttribute(Qt::WA_NoSystemBackground, true);


	m_renderer = new PetRenderer(this);

	auto* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_renderer);
	setLayout(layout);

	resize(500, 700);
	m_renderer->resize(500, 700);

	if (auto* screen = QGuiApplication::primaryScreen()) {
		QRect screenRect = screen->availableGeometry();
		move(screenRect.center() - rect().center());
	}
}

PetWindow::~PetWindow() = default;

void PetWindow::mousePressEvent(QMouseEvent* ev)
{
	if (ev->button() == Qt::LeftButton) {
		m_dragOffset = ev->globalPosition().toPoint() - frameGeometry().topLeft();
		ev->accept();
	}
}

void PetWindow::mouseMoveEvent(QMouseEvent* ev)
{
	if (ev->buttons() & Qt::LeftButton) {
		move(ev->globalPosition().toPoint() - m_dragOffset);
		ev->accept();
	}
}

bool PetWindow::loadModel(const QString& pmxPath, const QString& mmdDir /*= ""*/)
{
	return m_renderer->LoadModel(pmxPath.toStdString(),
		mmdDir.isEmpty() ? "" : mmdDir.toStdString());
}

bool PetWindow::loadMotion(const QString& vmdPath)
{
	return m_renderer->LoadMotion(vmdPath.toStdString());
}
