#include "mainwindow.hpp"
#include <QTreeWidget>
#include <QBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(new QHBoxLayout);
    centralWidget()->layout()->addWidget(tab_widget);
    centralWidget()->layout()->addWidget(explorer);

    tab_widget->setTabsClosable(true);

    tab_widget->addTab(new nvt::editor, "ed1");
}

MainWindow::~MainWindow() {
}
