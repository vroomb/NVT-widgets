#include "explorer.hpp"
#include <log.hpp>
#include <QMouseEvent>

nvt::widgets::explorer::explorer(QWidget* parent) :
    QTreeView(parent),
    model{ new QFileSystemModel }
{
    setModel(model);
    setRootIndex(model->setRootPath(root));

    hideColumn(1);
    hideColumn(2);
    hideColumn(3);

    setMaximumWidth(500);
    setHeaderHidden(true);
}

void nvt::widgets::explorer::mouseDoubleClickEvent(QMouseEvent* event) {
    log log{};
    if (event->button() == Qt::LeftButton) {
        auto i = indexAt(event->pos());
        
        if (model->isDir(i)) {
            expand(i);
        } else {
            emit requested(model->filePath(i).toStdString());
        }
    }
}