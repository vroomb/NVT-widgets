#pragma once

#include "nvt_timeline.hpp"
#include <QWidget>
#include <QPointF>
#include <QPen>
#include <map>

class QLineEdit;
class QGridLayout;
class QTimer;

namespace nvt {
    namespace timeline {
        typedef std::map<chain*, std::pair<node*, node*>> event_map;
    }
}

class nvt::timeline::circle : public QWidget {
public:
    circle(QWidget* parent = nullptr) : QWidget(parent) {
        setFixedSize(50, 50);
    }

    void paintEvent(QPaintEvent* event) override;

private:
    static const qreal outer_radius;
    static const qreal inner_radius;
};

class nvt::timeline::node : public QWidget {
    Q_OBJECT;
    friend circle;
    friend chain;

public:
    node(graph* parent);

    struct less {
        bool operator()(const node& lhs, const node& rhs) {
            return lhs.x() < lhs.x();
        }
    };

    bool hit(QPointF position);
    void fade();

    inline QPointF get_center() const {
        return m_center;
    }

    QPointF center(QPointF position);
    QPointF translate(QPointF position);
    void silent_translate(QPointF position);

    void move_title_up();
    void move_title_down();

    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    const event_map& chains() const {
        return m_chains;
    }

signals:
    void position_changed(node* n);

private:
    void add_chain(
        chain* chain,
        node* circumstance = nullptr,
        node* consequence = nullptr
    ) {
        m_chains.insert({ chain, {circumstance, consequence} });
    }

    void chain_update(
        chain* chain,
        node* circumstance = nullptr,
        node* consequence = nullptr
    ) {
        m_chains[chain] = {circumstance, consequence};
    }

    QTimer* timer;
    circle* station;
    QLineEdit* title;
    QMenu* m_menu;
    std::shared_ptr<QRectF> tile;

    event_map m_chains;

    QPointF m_center;
    QPoint offset;

    QColor background_color;
    QPen border;

    int alpha;

    bool fade_in = true;
    bool up_down = false;

    static const int duration;
    static const QPen ref_border;
    static const QColor ref_background_color;
    static const QMargins m_margins;
};
