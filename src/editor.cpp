#include "editor.hpp"
#include "log.hpp"
#include <QLatin1StringView>
#include <QString>
#include <QBoxLayout>

using FakeVim::Internal::FakeVimHandler;
using FakeVim::Internal::ExCommand;

nvt_widgets::editor_status_bar::editor_status_bar(QWidget* parent) :
    QWidget(parent)
{
    setLayout(new QHBoxLayout);
    m_right->setAlignment(Qt::AlignRight);
    layout()->addWidget(m_left);
    layout()->addWidget(m_right);
}

nvt_widgets::editor::editor(fs::path file_path, QWidget* parent) :
    QWidget(parent),
    working_file{file_path},
    m_file_path{file_path}
{
    nvt_widgets::log log{};

    setLayout(new QVBoxLayout);
    layout()->addWidget(text_edit);
    layout()->addWidget(status_bar);
    
    if (working_file.open()) {
        
        QFile file{ file_path };
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            text_edit->setText(file.readAll());
        } else {
            log("file " + file.fileName() + " did not open");
        }
    } else {
        log("working file " + working_file.fileName() + " did not open");
    }

    handler.commandBufferChanged.set(
        [this](const QString& contents, int cursorPos, int anchorPos, int messageLevel) {
            nvt_widgets::log log{};
            log(contents);
            log("cursorPos: " + std::to_string(cursorPos));
            //log("text == contents: " + std::to_string(status_bar->left()->text() == contents));
            if ((status_bar->left()->text() == contents) && (cursorPos == -1) && (message != "")) {
                status_bar->left()->setText(message);
                message = "";
            }
            else status_bar->left()->setText(contents);
        }
    );
    handler.extraInformationChanged.set(
        [this](const QString& text) {
            status_bar->left()->setText(text);
        }
    );
    handler.statusDataChanged.set(
        [this](const QString& text) {
            status_bar->right()->setText(text);
        }
    );
    handler.highlightMatches.set(
        [](const QString& needle) {
        }
    );
    handler.handleExCommandRequested.set(
        [this](bool* handled, const ExCommand& cmd) {
            std::error_code ec{};

            switch (scan(cmd)) {
            case save_and_quit:
                *handled = true;
                break;
            case save:
                *handled = true;
                ec = saveFile();
                if (ec.value() == 0) message = "saved to " + QString::fromStdString(m_file_path.string());
                else                 message = QString::fromStdString(ec.message());
                break;
            case quit:
                *handled = true;
                break;
            default:
                *handled = false;
            }
        }
    );
    handler.requestSetBlockSelection.set(
        [](const QTextCursor& cursor) {
        }
    );
    handler.requestDisableBlockSelection.set(
        [] {
        }
    );
    handler.requestHasBlockSelection.set(
        [](bool* on) {
        }
    );
    handler.indentRegion.set(
        [](int beginBlock, int endBlock, QChar typedChar) {
        }
    );
    handler.checkForElectricCharacter.set(
        [](bool* result, QChar c) {
        }
    );

    handler.handleCommand("set nopasskeys");
    handler.handleCommand("set nopasscontrolkey");

    handler.installEventFilter();
    handler.setupWidget();
}

std::error_code nvt_widgets::editor::saveFile() {
    QFile file{ m_file_path };

    if (file.open(QIODevice::WriteOnly | QIODevice::Text) == false)
        return std::error_code{ file.error(), QFileDeviceError };
    if (file.write(text_edit->toPlainText().toLocal8Bit()) == -1)
        return std::error_code{ file.error(), QFileDeviceError };

    return std::error_code{ 0, QFileDeviceError };
}
