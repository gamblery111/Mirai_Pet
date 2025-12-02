#include <iostream>

#include <QApplication>
#include <QMainWindow>

#include "PetWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

	PetWindow win;
	win.show();

	win.loadModel("E:/MMd/haila/海拉3.0.pmx", "E:/MMd/haila");

	return app.exec();
}