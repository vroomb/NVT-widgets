#pragma once

#include "nvt_widgets.hpp"
#include <QMainWindow>

class QLineEdit;
class QListWidget;

class nvt::widgets::launch_window : public QMainWindow {
    Q_OBJECT

public:
    explicit launch_window(QWidget *parent = nullptr);

signals:
    void launchRequested();

private:
    QLineEdit* search_bar;
    QListWidget* story_list;
};
