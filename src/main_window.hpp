#pragma once

#include "nvt_widgets.hpp"
#include "explorer.hpp"
#include <QMainWindow>
#include <QTabWidget>

class nvt::widgets::main_window : public QMainWindow {
public:
    explicit main_window(QWidget *parent = nullptr);
    ~main_window() override;

private:
    QTabWidget* tab_widget = new QTabWidget;
    explorer* m_explorer = new explorer;
};
