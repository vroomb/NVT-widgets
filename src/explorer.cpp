#include "explorer.hpp"

nvt::explorer::explorer(QWidget* parent) :
    QTreeView(parent)
{
    auto model = new QFileSystemModel;
    setModel(model);
    setRootIndex(model->setRootPath("H:/Documents/Obsidian/Reason"));

    hideColumn(1);
    hideColumn(2);
    hideColumn(3);

    setMaximumWidth(500);
    setHeaderHidden(true);
}