#include "main_window.hpp"
#include <timeline/graph.hpp>
#include <QTreeWidget>
#include <QBoxLayout>

nvt_widgets::main_window::main_window(QWidget *parent) :
    QMainWindow(parent)
{
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(new QHBoxLayout);
    centralWidget()->layout()->addWidget(tab_widget);
    centralWidget()->layout()->addWidget(m_explorer);

    tab_widget->setTabsClosable(true);

    tab_widget->addTab(new nvt_widgets::editor{"H:/Projects/cpp/NVT-widgets/thing.txt"}, "ed1");
    tab_widget->addTab(new nvt::timeline::graph{}, "ed2");
}

nvt_widgets::main_window::~main_window() {}
