#pragma once

#include "nvt_timeline.hpp"
#include <QWidget>
#include <QPointF>

class nvt::timeline::node : public QWidget {
public:
    node(graph* parent);

    bool hit(QPointF position);
};
