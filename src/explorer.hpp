#pragma once

#include "nvt_widgets.hpp"
#include <QTreeView>
#include <QFileSystemModel>
#include <filesystem>

namespace fs = std::filesystem;

class nvt::widgets::explorer : public QTreeView {
    Q_OBJECT

public:
    explorer(QWidget* parent = nullptr);

signals:
    void requested(fs::path path);

private:
    void mouseDoubleClickEvent(QMouseEvent* event) override;

    const char* root = "H:/Documents/Obsidian/Reason";

    QFileSystemModel* model;
};
