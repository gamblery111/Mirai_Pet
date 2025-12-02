#include <iostream>

#include <QApplication>
#include <QMainWindow>

//#include <QQmlApplicationEngine>
//#include <QQuickWindow>
//#include <Saba/Model/MMD/PMXModel.h>

#include "PetWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    //// 启用透明窗口支持
    //QQuickWindow::setDefaultAlphaBuffer(true);

    //QQmlApplicationEngine engine;
    //engine.loadFromModule("MyPet", "Main");

    //auto pmdModel = std::make_unique<saba::PMXModel>();
    //if (!pmdModel->Load("D:/WorkSpace/C++/Project_Code/Mirai_Pet/haila.pmx", "E:/MMd/haila"))
    //{
    //    std::cout << "Failed to load pmd file.\n";
    //    return false;
    //}


	PetWindow win;
	win.show();

	win.loadModel("E:/MMd/haila/海拉3.0.pmx", "E:/MMd/haila");

	return app.exec();
}