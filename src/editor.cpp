#include "editor.hpp"
#include "log.hpp"

#include <QLatin1StringView>
#include <QString>
#include <QBoxLayout>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <fstream>
#include <string>
#include <QMenu>

#include <maddy/parser.h>

using FakeVim::Internal::FakeVimHandler;
using FakeVim::Internal::ExCommand;

std::string nvt::widgets::editor::markdown_css = "";

nvt::widgets::editor_status_bar::editor_status_bar(QWidget* parent) :
    QWidget(parent)
{
    setLayout(new QHBoxLayout);
    m_right->setAlignment(Qt::AlignRight);
    layout()->addWidget(m_left);
    layout()->addWidget(m_right);
}

void nvt::widgets::editor_text_edit::paintEvent(QPaintEvent* event) {
    QTextEdit::paintEvent(event);
}

void nvt::widgets::editor_text_edit::contextMenuEvent(QContextMenuEvent* event) {
    auto menu = createStandardContextMenu();

    connect(menu->addAction("toggle read mode"), &QAction::triggered, this,
        [this](){
            if (read_mode == false) {
                plain_text = toPlainText();
                std::stringstream ss{ plain_text.toStdString() };

                auto body = maddy::Parser{}.Parse(ss);
                body = "<html><head><style>" + editor::markdown_css + "</style></head><body>" + body + "</body></html>";
                setHtml(QString::fromStdString(std::move(body)));
                read_mode = true;
            } else {
                setPlainText(plain_text);
                read_mode = false;
            }
        }
    );

    menu->exec(event->globalPos());
    menu->deleteLater();
}

nvt::widgets::editor::editor(fs::path file_path, QWidget* parent) :
    QWidget(parent),
    working_file{ file_path },
    m_file_path{ file_path },
    timer{ new QTimer{this} }
{
    nvt::widgets::log log{};

    if (markdown_css.size() == 0) {
        std::ifstream f{ res_dir"markdown.css",  std::ios_base::in | std::ios_base::ate };

        if (f.is_open()) {
            auto size = f.tellg();
            markdown_css = std::string(size, ' ');

            f.seekg(0);
            f.read(&markdown_css[0], size);
        }

        log("loaded markdown.css with contents:\n" + markdown_css);
    }

    setLayout(new QVBoxLayout);
    layout()->addWidget(text_edit);
    layout()->addWidget(status_bar);

    timer->setInterval(5000);

    if (working_file.open()) {
        QFile f{ file_path };

        if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            text_edit->setPlainText(f.readAll());
        }
        else
            log("file " + file_path.filename().string() + " did not open");
    }
    else {
        log("working file " + working_file.fileName() + " did not open");
    }

    handler.commandBufferChanged.set(
        [this](const QString& contents, int cursorPos, int anchorPos, int messageLevel) {
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
                ec = saveFile();
                if (ec.value() == 0) deleteLater();
                else                 message = QString::fromStdString(ec.message());
                break;
            case save:
                *handled = true;
                ec = saveFile();
                if (ec.value() == 0) message = "saved to " + QString::fromStdString(m_file_path.string());
                else                 message = QString::fromStdString(ec.message());
                break;
            case quit:
                *handled = true;
                if (saved) deleteLater();
                else       message = "save first.";
                break;
            default:
                *handled = false;
            }
        }
    );
    handler.requestSetBlockSelection.set(
        [this](const QTextCursor& cursor) {
            auto tc = text_edit->textCursor();
            tc.setPosition(cursor.position() - 1);
            text_edit->setTextCursor(tc);

            auto c = text_edit->cursorRect();
            if (text_edit->selected_rect.isNull()) {
                text_edit->selected_rect.setTopLeft(c.topLeft());
            }
            text_edit->selected_rect.setBottomRight(c.bottomRight());
        }
    );
    handler.requestDisableBlockSelection.set(
        [this] {
            text_edit->selected_rect = QRect{};
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

    connect(text_edit, &QTextEdit::textChanged, this,
        [this]() {
            saved = false;
            timer->stop();
            timer->start();
        }
    );

    connect(timer, &QTimer::timeout, this,
        [this]() {
            std::error_code ec;

            if (working_file.open() == false)
                ec = std::error_code{ working_file.error(), QFileDeviceError };
            else if (working_file.write(text_edit->toPlainText().toLocal8Bit()) == -1)
                ec = std::error_code{ working_file.error(), QFileDeviceError };
            else working_file.close();
        }
    );

    connect(text_edit, &QTextEdit::cursorPositionChanged, this,
        [this]() {
            QFontMetrics f{ text_edit->currentFont() };
            text_edit->setCursorWidth(f.horizontalAdvance(
                text_edit->document()->characterAt(text_edit->textCursor().position())));
        }
    );
}

void nvt::widgets::editor::paintEvent(QPaintEvent* event) {
    QPainter painter{ this };

    painter.fillRect(rect(), QColor{ 13, 17, 23 });
    if (text_edit->selected_rect.isNull() == false) {
        auto r = text_edit->selected_rect;
        r.translate(text_edit->pos() + QPoint{ 100, 25 });
        painter.fillRect(r, QColor{ 0, 160, 200 });
    }
}

std::error_code nvt::widgets::editor::saveFile() {
    QFile file{ m_file_path };

    if (file.open(QIODevice::WriteOnly | QIODevice::Text) == false)
        return std::error_code{ file.error(), QFileDeviceError };
    else if (file.write(text_edit->toPlainText().toLocal8Bit()) == -1)
        return std::error_code{ file.error(), QFileDeviceError };
    else file.close();

    saved = true;

    return std::error_code{ 0, QFileDeviceError };
}
