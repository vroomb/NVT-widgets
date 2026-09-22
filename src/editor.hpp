#pragma once

#include "nvt_widgets.hpp"
#include <QTextEdit>
#include <QMainWindow>
#include <QLabel>
#include <fakevim/fakevimhandler.h>
#include <filesystem>
#include <QTemporaryFile>

class QTimer;

namespace nvt::widgets {
    const class QFileDevice_error_category : public std::error_category {
    public:
        virtual const char* name() const noexcept {
            return "QFileDeviceError";
        }

        virtual std::string message(int ev) const {
            switch (ev) {
            case QFileDevice::NoError:          return "No error occurred.";
            case QFileDevice::ReadError:        return "An error occurred when reading from the file.";
            case QFileDevice::WriteError:       return "An error occurred when writing to the file.";
            case QFileDevice::FatalError:       return "A fatal error occurred.";
            case QFileDevice::ResourceError:    return "Out of resources.";
            case QFileDevice::OpenError:        return "The file could not be opened.";
            case QFileDevice::AbortError:       return "The operation was aborted.";
            case QFileDevice::TimeOutError:     return "A timeout occurred.";
            case QFileDevice::UnspecifiedError: return "An unspecified error occurred.";
            case QFileDevice::RemoveError:      return "The file could not be removed.";
            case QFileDevice::RenameError:      return "The file could not be renamed.";
            case QFileDevice::PositionError:    return "The position in the file could not be changed.";
            case QFileDevice::ResizeError:      return "The file could not be resized.";
            case QFileDevice::PermissionsError: return "The file could not be accessed.";
            case QFileDevice::CopyError:        return "The file could not be copied.";
            default:                            return "Unknown Error";
            }
        }
    } QFileDeviceError;
}

namespace fs = std::filesystem;

using FakeVim::Internal::ExCommand;
using FakeVim::Internal::FakeVimHandler;

class nvt::widgets::editor_status_bar : public QWidget {
public:
    editor_status_bar(QWidget* parent = nullptr);

    QLabel* left() { return m_left; }
    QLabel* right() { return m_right; }

private:
    QLabel* m_left = new QLabel;
    QLabel* m_right = new QLabel;
};

class nvt::widgets::editor_text_edit : public QTextEdit {
public:
    editor_text_edit(QWidget* parent = nullptr) : QTextEdit(parent) {}

    QRect selected_rect;
    QString plain_text;
private:
    bool read_mode = false;

    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
};

class nvt::widgets::editor : public QWidget{
    friend editor_text_edit;

public:
    editor(fs::path file_path = "", QWidget* parent = nullptr);

    std::error_code saveFile();

private:
    void paintEvent(QPaintEvent* event) override;

    editor_text_edit* text_edit = new editor_text_edit{};
    editor_status_bar* status_bar = new editor_status_bar{};
    QTimer* timer;
    FakeVimHandler handler{text_edit, 0};

    QString message{};

    QTemporaryFile working_file;
    fs::path m_file_path;
    bool saved = true;

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

    static std::string markdown_css;
};
