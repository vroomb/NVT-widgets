#pragma once

#include "nvt_timeline.hpp"
#include "node.hpp"
#include <QWidget>
#include <QMenu>
#include <variant>

class TIMELINE_API nvt::timeline::graph : public QWidget {
public:
    graph(QWidget* parent = nullptr);

    void translate(QPointF position);

    void add_event(bool);
    void add_chain(bool);

    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;



private:
    QPen pen;
    QMenu* m_menu = new QMenu{ this };

    QPoint context_menu_pos{};
    QPointF offset{};

    QList<node*> m_nodes{};
    QList<chain*> m_chains{};

    std::optional<std::variant<chain*, node*>> working_mouse_item;
};