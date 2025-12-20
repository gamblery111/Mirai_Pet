#include <iostream>

#include <QApplication>

#include "PetMainWindow.h"

int main(int argc, char *argv[])
{
    try {
        QApplication app(argc, argv);

        miraipet::ui::PetMainWindow w;
        w.resize(800, 600);
        w.show();

        return app.exec();
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown exception caught." << std::endl;
        return EXIT_FAILURE;
    }
}
