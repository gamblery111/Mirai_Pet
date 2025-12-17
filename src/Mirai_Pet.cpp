#include <iostream>

#include <QApplication>

#include "PetMainWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	miraipet::ui::PetMainWindow w;
	w.resize(800, 600);
	w.show();

	return app.exec();
}