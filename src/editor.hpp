#pragma once

#include "nvt.hpp"
#include <QTextEdit>
#include <QMainWindow>
#include <QLabel>
#include <fakevim/fakevimhandler.h>

using FakeVim::Internal::ExCommand;
using FakeVim::Internal::FakeVimHandler;

class nvt::editor_status_bar : public QWidget {
public:
    editor_status_bar(QWidget* parent = nullptr);

    QLabel* left() { return m_left; }
    QLabel* right() { return m_right; }

private:
    QLabel* m_left = new QLabel;
    QLabel* m_right = new QLabel;
};

class nvt::proxy : public QObject {
    Q_OBJECT

public:
    proxy(
        QWidget* editor,
        editor_status_bar* status_bar,
        FakeVimHandler* parent
    );

    void openFile(const QString& fileName);

signals:
    void handleInput(QString& keys);
    void requestSave();
    void requestSaveAndQuit();
    void requestQuit();

public slots:

private:
    QWidget* m_editor;
    editor_status_bar* m_status_bar;

    enum commands {
        parse_fail,
        save_and_quit,
        save,
        quit
    };

    inline commands scan(const ExCommand& cmd) {
        if (cmd.cmd == "wq")
            return save_and_quit;
        else if (cmd.matches("w", "write") || cmd.matches("wa", "wall"))
            return save;
        else if (cmd.matches("q", "quit") || cmd.matches("qa", "qall"))
            return quit;
        else
            return parse_fail;
    }
};

class nvt::editor : public QWidget {
public:
    editor(QWidget* parent = nullptr);

private:
    QTextEdit* text_edit  = new QTextEdit{};
    editor_status_bar* status_bar = new editor_status_bar{};
    FakeVimHandler handler{text_edit, 0};
    proxy* proxy = new nvt::proxy{ text_edit, status_bar, &handler };
};
