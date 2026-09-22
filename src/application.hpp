#pragma once

#include "nvt_widgets.hpp"
#include "main_window.hpp"
#include <QApplication>

class nvt::widgets::application : public QApplication {
public:
    application(int argc, char* argv[]);

    int exec();

private:
    main_window w{};
};