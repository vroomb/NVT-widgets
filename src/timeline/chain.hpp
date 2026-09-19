#pragma once

#include <timeline/nvt_timeline.hpp>
#include <QObject>
#include <QPolygonF>
#include <set>

class nvt::timeline::chain : public QObject {
    Q_OBJECT;
    friend node;

public:
    enum chain_mode {
        horizontal,
        vertical
    };

    chain(graph* parent);
    chain(QPointF position, graph* parent);

    const QPolygonF& path() const { return m_path; }
    qreal hit_width() const { return m_hit_width; }
    qreal stroke_width() const { return m_stroke_width; }
    void attach_node(node* add);

    std::optional<QPointF> hit(QPointF position);

    void set_start_pos(QPointF position);
    void translate(QPointF position);
    void silent_translate(QPointF position);

    QPointF get_start_pos() {
        return m_path[0];
    }

signals:
    void path_changed();

private:
    void update_path();
    void update_node(node* n);

    static const chain_mode m_mode = horizontal;

    std::shared_ptr<QRectF> tile;
    std::list<std::pair<node*, QPolygonF>> m_nodes{};
    QPolygonF m_path;

    qreal m_stroke_width{ 5 };
    qreal m_hit_width{ 5 };

    static QPolygonF VerticesVH(QPointF FromV, QPointF ToH);
    static QPolygonF VerticesHV(QPointF FromH, QPointF ToV);
    static QPolygonF VerticesVV(QPointF FromV, QPointF ToV);
    static QPolygonF VerticesHH(QPointF FromH, QPointF ToH);
};