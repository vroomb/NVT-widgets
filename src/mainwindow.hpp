#pragma once

#include "nvt.hpp"
#include "editor.hpp"
#include "explorer.hpp"
#include <QMainWindow>
#include <QTabWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    QTabWidget* tab_widget = new QTabWidget;
    nvt::explorer* explorer = new nvt::explorer;
};
