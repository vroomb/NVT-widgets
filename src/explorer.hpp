#pragma once

#include "nvt.hpp"
#include <QTreeView>
#include <QFileSystemModel>

class nvt::explorer : public QTreeView {
public:
    explorer(QWidget* parent = nullptr);

private:
};
