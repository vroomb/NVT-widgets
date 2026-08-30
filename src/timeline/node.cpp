#include <timeline/node.hpp>
#include <timeline/graph.hpp>
#include <QBoxLayout>
#include <QLineEdit>

nvt::timeline::node::node(graph* parent) :
    QWidget(parent)
{
    resize(200, 200);

    setLayout(new QHBoxLayout);

    layout()->addWidget(new QLineEdit);
}

bool nvt::timeline::node::hit(QPointF position) {
    position = position - pos();
    return (0 > position.x()) && (position.x() > size().width())
        && (0 > position.y()) && (position.y() > size().height());
}