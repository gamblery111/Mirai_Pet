#include "PetMainWindow.h"
#include "PetGLWidget.h"

#include <QVBoxLayout>
#include <QMouseEvent>
#include <QScreen>
#include <QGuiApplication>

namespace miraipet::ui
{
    PetMainWindow::PetMainWindow()
    {
        setWindowFlags(Qt::FramelessWindowHint |
                       Qt::WindowStaysOnTopHint |
                       Qt::Tool);

        setAttribute(Qt::WA_TranslucentBackground, true);
        setAttribute(Qt::WA_NoSystemBackground, true);

        m_glWidget = new PetGLWidget(this);

        auto *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(m_glWidget);
        setLayout(layout);

        resize(500, 700);
        m_glWidget->resize(500, 700);

        if (auto *screen = QGuiApplication::primaryScreen())
        {
            QRect screenRect = screen->availableGeometry();
            move(screenRect.center() - rect().center());
        }
    }

    PetMainWindow::~PetMainWindow()
    {
    }

    void PetMainWindow::mousePressEvent(QMouseEvent *ev)
    {
        if (ev->button() == Qt::LeftButton)
        {
            m_dragOffset = ev->globalPosition().toPoint() - frameGeometry().topLeft();
            ev->accept();
        }
    }

    void PetMainWindow::mouseMoveEvent(QMouseEvent *ev)
    {
        if (ev->buttons() & Qt::LeftButton)
        {
            move(ev->globalPosition().toPoint() - m_dragOffset);
            ev->accept();
        }
    }
}
