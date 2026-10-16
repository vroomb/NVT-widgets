#include "launch_window.hpp"
#include "editor.hpp"

#include <project/story.hpp>
#include <project/global.hpp>

#include <chrono>
namespace chr = std::chrono;

#include <QBoxLayout>
#include <QFileDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QStatusBar>
#include <QMessageBox>

nvt::widgets::launch_window::launch_window(QWidget* parent) :
    QMainWindow(parent),
    search_bar{ new QLineEdit },
    story_list{ new QListWidget }
{
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(new QVBoxLayout);
    centralWidget()->layout()->addWidget(search_bar);
    centralWidget()->layout()->addWidget(story_list);

    centralWidget()->layout()->setContentsMargins(QMargins{30, 30, 30, 30});

    QFile file{ res_dir"launch.qss" };

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(file.readAll());
    }

    auto label = new QLabel{};
    label->setTextFormat(Qt::RichText);
    label->setText(
        "<style>" +
            styleSheet() +
        "</style>"
        "<div class=\"nvt_version_label\">"
            "<div class=\"nvt_label\">NVT"
                "<span class=\"version_label\">v0.0.1</span>"
            "</div>"
        "</div>"
    );

    label->setMargin(10);

    setStatusBar(new QStatusBar);

    statusBar()->addWidget(label);

    resize(1280, 720);

    /* setup story_list */ {
        auto l = nvt::core::global::instance()->launches();

        for (auto i = l->begin(); i != l->end(); i++) {
            auto item = new QListWidgetItem{
                QString::fromStdString(i->location.string())
            };
            story_list->addItem(item);
        }
        story_list->addItem("Add a story...");

        connect(story_list, &QListWidget::itemDoubleClicked, this,
            [this](QListWidgetItem* item) {
                if (item->text() == "Add a story...") {
                    fs::path f = QFileDialog::getExistingDirectory(this, tr("Open a story")).toStdString();
                    nvt::core::global::instance()->add_launch({
                            f.filename().string(),
                            f,
                            chr::steady_clock::now().time_since_epoch().count()
                        }
                    );
                    story_list->addItem(QString::fromStdString(f.string()));
                } else {
                    auto ec = nvt::core::story::open(item->text().toStdString());
                    if (ec.value() != 0) {
                        auto mb = new QMessageBox{ this };
                        mb->setText(QString::fromStdString(ec.message()));
                        mb->exec();
                        mb->deleteLater();
                    } else {
                        emit launchRequested();
                    }
                }
            }
        );
    }
}
