#include <timeline/graph.hpp>
#include <timeline/chain.hpp>

#include <colors/nvt_colors.hpp>

#include <QMouseEvent>
#include <QMessageBox>
#include <QPainter>
#include <QMenu>

nvt::timeline::graph::graph(QWidget* parent) :
    QWidget(parent),
    pen{ nvt::color::pri(), 10, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin },
    tile{ std::make_shared<QRectF>(0, 0, 100, 100) },
    m_menu{ new QMenu{this} }
{
    setBaseSize(50, 50);

    /* initialize menu */ {
        connect(m_menu->addAction("Add an event"), &QAction::triggered, this, &graph::add_event);
        connect(m_menu->addAction("Add a chain"), &QAction::triggered, this, &graph::add_chain);
    }
}

void nvt::timeline::graph::add_event(bool) {
    auto n = new node{ this };
    n->center(snap(context_menu_pos));
    n->show();

    m_nodes << n;

    repaint();
}

void nvt::timeline::graph::add_chain(bool) {
    auto n = new chain{ snap(context_menu_pos), this };
    m_chains << n;
    connect(n, &chain::path_changed, this, [this]() { repaint(); });
    repaint();
}

void nvt::timeline::graph::translate(QPointF position) {
    tile->translate(position);

    for (auto i : m_nodes)
        i->silent_translate(position);

    for (auto i : m_chains)
        i->silent_translate(position);

    repaint();
}

void nvt::timeline::graph::paintEvent(QPaintEvent*) {
    QPainter painter{ this };

    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(*tile, nvt::color::bse());

    /* draw top bar */ { 
        auto w = tile->width();
        for (auto x = modulo(tile->x(), 2*w); x < width(); x += 2*w) {
            painter.fillRect(QRectF{ x, 0, w, 10 }, nvt::color::ter());
            painter.fillRect(QRectF{ x + w, 0, w, 10 }, nvt::color::bse());
        }
    }

    /* draw left bar */ { 
        auto h = tile->height();
        for (auto y = modulo(tile->y(), 2*h); y < height(); y += 2*h) {
            painter.fillRect(QRectF{ 0, y, 10, h }, nvt::color::ter());
            painter.fillRect(QRectF{ 0, y + h, 10, h }, nvt::color::bse());
        }
    }

    /* draw the chains */ for (auto i : m_chains) {
        painter.drawPolyline(i->path());
    }
}

void nvt::timeline::graph::contextMenuEvent(QContextMenuEvent* event) {
    context_menu_pos = event->pos();
    m_menu->popup(event->globalPos());
}

#define sp(t, w, x) t + w * ((int)((x - t)/w + (int)(t/w)) - (int)(t/w) + 0.5)

inline QPoint nvt::timeline::graph::snap(QPoint pos) {
    pos.setX(sp(tile->x(), tile->width(), pos.x()));
    pos.setY(sp(tile->y(), tile->height(), pos.y()));
    return pos;
}

void nvt::timeline::graph::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        for (auto i : m_nodes) if (i->hit(event->position())) {
            offset = event->position() - i->get_center();
            working_mouse_item = i;
            return;
        }

        for (auto i : m_chains) if (i->hit(event->position())) {
            offset = event->position() - i->get_start_pos();
            working_mouse_item = i;
            return;
        }

        if (working_mouse_item == std::nullopt)
            offset = event->position();
    }

    repaint();
}

void nvt::timeline::graph::mouseMoveEvent(QMouseEvent* event) {
    if (working_mouse_item.has_value()) {
        auto item = working_mouse_item.value();

        if (std::holds_alternative<node*>(item)) {
            auto point = std::get<node*>(item)
                ->center((event->position() - offset).toPoint());

            for (auto i : m_chains) if (i->hit(point)) {

            }

        } else if (std::holds_alternative<chain*>(item)) {
            std::get<chain*>(item)
                ->set_start_pos((event->position() - offset).toPoint());
        }

    } else {
        translate(event->position() - offset);
        offset = event->position();
    }
}

void nvt::timeline::graph::mouseReleaseEvent(QMouseEvent* event) {
    if (working_mouse_item.has_value()) {
        auto item = working_mouse_item.value();

        if (std::holds_alternative<node*>(item)) {
            auto n = std::get<node*>(item);
            auto point = n->center(snap((event->position() - offset).toPoint()));

            for (auto i : m_chains) if (i->hit(point) && !n->chains().contains(i)) {
                i->attach_node(std::get<node*>(item));
                break;
            }
        }

        if (std::holds_alternative<chain*>(item))
            std::get<chain*>(item)
                ->set_start_pos(snap((event->position() - offset).toPoint()));
    }

    working_mouse_item = std::nullopt;

    repaint();
}
