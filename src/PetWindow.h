#pragma once

#include <QWidget>
#include <QPoint>

class PetRenderer;

class PetWindow  : public QWidget
{
	Q_OBJECT

private:
	PetRenderer* m_renderer{ nullptr };
	QPoint m_dragOffset;

protected:
	void mousePressEvent(QMouseEvent* ev) override;
	void mouseMoveEvent(QMouseEvent* ev) override;

public:
	explicit PetWindow(QWidget *parent = nullptr);
	~PetWindow();

	// 外部调用：加载模型和动作
	bool loadModel(const QString& pmxPath, const QString& mmdDir = "");
	bool loadMotion(const QString& vmdPath);
};

