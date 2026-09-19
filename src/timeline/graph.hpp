#pragma once

#include "nvt_timeline.hpp"
#include "node.hpp"
#include <QWidget>
#include <variant>

class TIMELINE_API nvt::timeline::graph : public QWidget {
public:
    graph(QWidget* parent = nullptr);

    void translate(QPointF position);

    QPoint snap(QPoint pos);

    void add_event(bool);
    void add_chain(bool);

    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    std::shared_ptr<QRectF> get_tile_ref() {
        return tile;
    }

private:
    QPen pen;
    QMenu* m_menu;
    std::shared_ptr<QRectF> tile;

    QPoint context_menu_pos{};
    QPointF offset{};

    QList<node*> m_nodes{};
    QList<chain*> m_chains{};

    std::optional<std::variant<chain*, node*>> working_mouse_item;
};