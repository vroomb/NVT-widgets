#pragma once

#include "nvt_timeline.hpp"
#include <QWidget>
#include <QPointF>
#include <QPen>

class QLineEdit;
class QGridLayout;
class QTimer;

class nvt::timeline::circle : public QWidget {
public:
    circle(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumSize(50, 50);
        setMaximumSize(50, 50);
    }

    void paintEvent(QPaintEvent* event) override;

private:
    qreal outer_radius = 1;
    qreal inner_radius = 0.8;
};

class nvt::timeline::node : public QWidget {
    friend circle;

public:
    node(graph* parent);

    bool hit(QPointF position);

    void fade();

    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    circle* station;
    QLineEdit* title;
    QGridLayout* m_layout;

    QTimer* timer;

    static QColor ref_background_color;
    static QPen ref_border;

    int alpha;

    QColor background_color;
    QPen border;

    static int duration;

    bool fade_in = true;
};
