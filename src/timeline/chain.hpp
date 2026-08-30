#pragma once

#include <timeline/nvt_timeline.hpp>
#include <QObject>
#include <QPolygonF>

class nvt::timeline::chain : public QObject {
    Q_OBJECT

public:
    enum chain_mode {
        horizontal,
        vertical
    };

    chain(graph* parent);
    chain(QPointF position, graph* parent);

    QPolygonF path();
    qreal hit_width() { return m_hit_width; }
    qreal stroke_width() { return m_stroke_width; }
    std::optional<QPointF> cursor();
    std::optional<QPointF> hit(QPointF position);

    void translate(QPointF position);
    void set_cursor(std::optional<QPointF> point);

    static QPolygonF VerticesVH(QPointF FromV, QPointF ToH);
    static QPolygonF VerticesHV(QPointF FromH, QPointF ToV);
    static QPolygonF VerticesVV(QPointF FromV, QPointF ToV);
    static QPolygonF VerticesHH(QPointF FromH, QPointF ToH);

signals:
    void path_changed();

private:
    void update_path();

    chain_mode m_mode = horizontal;

    QList<node*> m_nodes{};
    QPolygonF m_path{};

    qreal m_stroke_width{ 5 };
    qreal m_hit_width{ 5 };

    QPointF start_point{ 0, 0 };
    QPointF end_point{ 400, 0 };
    std::optional<QPointF> m_cursor = std::nullopt;
};