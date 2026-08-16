#pragma once

#include "nvt.hpp"
#include "mainwindow.hpp"

class nvt::application : public QApplication {
public:
    application(int argc, char* argv[]);

    int exec();

private:
    MainWindow w{};
};