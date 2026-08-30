#pragma once

#include "nvt_timeline.hpp"
#include "node.hpp"
#include <QWidget>
#include <QMenu>
#include <QPainter>
#include <variant>

class TIMELINE_API nvt::timeline::graph : public QWidget {
public:
    graph(QWidget* parent = nullptr);

    void add_event(bool);
    void add_chain(bool);

    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
private:
    QPen pen{ QColor{"white"} };
    QMenu* m_menu = new QMenu{ this };

    QPoint context_menu_pos{};

    QList<node*> m_nodes{};
    QList<chain*> m_chains{};

    std::variant<chain*, node*> working_mouse_item;
};