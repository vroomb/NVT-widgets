#pragma once

#include "nvt_widgets.hpp"
#include <yaml-cpp/yaml.h>
#include <QWidget>
#include <QPainter>

class nvt::widgets::yaml_display : public QWidget {
public:
    yaml_display(YAML::Node node, QWidget* parent = nullptr);

private:
    void paintEvent(QPaintEvent*) override;
    YAML::Node m_node;

    QBrush brush;
    QPen pen;
};
