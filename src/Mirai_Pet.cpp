#include <iostream>

#include <QApplication>

#include "PetMainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

	miraipet::ui::PetMainWindow win;
	win.show();

	// win.loadModel("E:/MMd/haila/海拉3.0.pmx", "E:/MMd/haila");

	return app.exec();
}