#include "editor.hpp"
#include <QLatin1StringView>
#include <QString>
#include <QBoxLayout>

using FakeVim::Internal::FakeVimHandler;
using FakeVim::Internal::ExCommand;

nvt::editor_status_bar::editor_status_bar(QWidget* parent) :
    QWidget(parent)
{
    setLayout(new QHBoxLayout);
    m_right->setAlignment(Qt::AlignRight);
    layout()->addWidget(m_left);
    layout()->addWidget(m_right);
}

nvt::proxy::proxy(
    QWidget* editor,
    editor_status_bar* status_bar,
    FakeVimHandler* handler
) :
    QObject(handler),
    m_editor{ editor },
    m_status_bar{ status_bar }
{
    handler->commandBufferChanged.set(
        [this](const QString& contents, int cursorPos, int anchorPos, int messageLevel) {
            m_status_bar->left()->setText(contents);
        }
    );
    handler->extraInformationChanged.set(
        [](const QString& text) {
        }
    );
    handler->statusDataChanged.set(
        [this](const QString& text) {
            m_status_bar->right()->setText(text);
        }
    );
    handler->highlightMatches.set(
        [](const QString& needle) {
        }
    );
    handler->handleExCommandRequested.set(
        [this](bool* handled, const ExCommand& cmd) {
            switch (scan(cmd)) {
            case save_and_quit:
                emit requestSaveAndQuit();
                *handled = true;
                break;
            case save:
                emit requestSave();
                *handled = true;
                break;
            case quit:
                emit requestQuit();
                *handled = true;
                break;
            default:
                *handled = false;
            }
        }
    );
    handler->requestSetBlockSelection.set(
        [](const QTextCursor& cursor) {
        }
    );
    handler->requestDisableBlockSelection.set(
        [] {
        }
    );
    handler->requestHasBlockSelection.set(
        [](bool* on) {
        }
    );
    handler->indentRegion.set(
        [](int beginBlock, int endBlock, QChar typedChar) {
        }
    );
    handler->checkForElectricCharacter.set(
        [](bool* result, QChar c) {
        }
    );

    QObject::connect(this, &proxy::handleInput, handler,
        [handler] (const QString &text) {
            handler->handleInput(text);
        }
    );
    QObject::connect(this, &proxy::requestSave, this,
        [] () {

        }
    );
    QObject::connect(this, &proxy::requestSaveAndQuit, this,
        [] () {
  
        }
    );
    QObject::connect(this, &proxy::requestQuit, this,
        [] () {
        
        }
    );

    handler->handleCommand("set nopasskeys");
    handler->handleCommand("set nopasscontrolkey");

    handler->installEventFilter();
    handler->setupWidget();
}

void nvt::proxy::openFile(const QString &fileName) {
    QString s(":r %1<CR>");
    s = s.arg(fileName);
    emit handleInput(s);
}

nvt::editor::editor(QWidget* parent) :
    QWidget(parent)
{
    setLayout(new QVBoxLayout);
    layout()->addWidget(text_edit);
    layout()->addWidget(status_bar);
}
