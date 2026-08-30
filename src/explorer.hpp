#pragma once

#include "nvt_widgets.hpp"
#include <QTreeView>
#include <QFileSystemModel>

class nvt_widgets::explorer : public QTreeView {
public:
    explorer(QWidget* parent = nullptr);

private:
};
