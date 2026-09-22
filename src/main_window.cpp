#include "main_window.hpp"
#include "editor.hpp"
#include <timeline/graph.hpp>
#include <QTreeWidget>
#include <QBoxLayout>
#include <QTextBrowser>

nvt::widgets::main_window::main_window(QWidget *parent) :
    QMainWindow(parent),
    tab_widget{ new QTabWidget },
    m_explorer{ new explorer }
{
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(new QHBoxLayout);
    centralWidget()->layout()->addWidget(tab_widget);
    centralWidget()->layout()->addWidget(m_explorer);

    tab_widget->setTabsClosable(true);

    resize(1080, 720);

    connect(m_explorer, &explorer::requested, this,
        [this](fs::path path) {
            tab_widget->setCurrentIndex(
                tab_widget->addTab(
                    new editor{ path },
                    QString::fromStdString(path.filename().string())
                )
            );
        }
    );

    connect(tab_widget, &QTabWidget::tabCloseRequested, this,
        [this](int index) {
            auto w = tab_widget->widget(index);
            w->deleteLater();
        }
    );

    tab_widget->addTab(new nvt::timeline::graph, "graph");
}

nvt::widgets::main_window::~main_window() {}
