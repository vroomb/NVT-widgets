#include "main_window.hpp"
#include "editor.hpp"
#include "yaml_display.hpp"

#include <project/story.hpp>
#include <project/global.hpp>
#include <timeline/graph.hpp>
#include <log.hpp>

#include <QTreeWidget>
#include <QBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QStatusBar>
#include <fstream>
#include <QScrollArea>

nvt::widgets::main_window::main_window(QWidget* parent) :
    QMainWindow(parent),
    tab_widget{ new QTabWidget },
    m_explorer{ new explorer }
{
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(new QHBoxLayout);
    centralWidget()->layout()->addWidget(tab_widget);
    centralWidget()->layout()->addWidget(m_explorer);

    tab_widget->setTabsClosable(true);

    resize(1280, 720);

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

    std::ifstream f{ res_dir"sample.yaml",  std::ios_base::in | std::ios_base::ate };

    std::string yaml;
    if (f.is_open()) {
        auto size = f.tellg();
        yaml = std::string(size, ' ');

        f.seekg(0);
        f.read(&yaml[0], size);
    }

    auto scroll = new QScrollArea;
    tab_widget->addTab(scroll, "yaml");
    scroll->setWidget(new nvt::widgets::yaml_display{ YAML::Load(yaml) });

    /* setup menu bar */ {
        QMenu* menu;

        setMenuBar(new QMenuBar{ this });

        menu = menuBar()->addMenu("File");

        connect(menu->addAction("Open a story..."), &QAction::triggered, this, 
            [this]() {
                auto f = QFileDialog::getExistingDirectory(this, tr("Open a story"));
                nvt::core::story::close();
                nvt::core::story::open(f.toStdString());
            }
        );

        menuBar()->addMenu("Edit");
        menuBar()->addMenu("View");
        menuBar()->addMenu("Git");
    }

    /* setup stylesheets */ {
        QFile file{ res_dir"style.qss" };
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            setStyleSheet(file.readAll());
        }
    }
}
