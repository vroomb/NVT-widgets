#include <timeline/graph.hpp>
#include <timeline/chain.hpp>

#include <colors/nvt_colors.hpp>

#include <QMouseEvent>
#include <QMessageBox>
#include <QPainter>

nvt::timeline::graph::graph(QWidget* parent) :
    QWidget(parent),
    pen{nvt::color::ter(), 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin}
{
    setBaseSize(50, 50);

    /* initialize menu */ {
        connect(m_menu->addAction("Add an event"), &QAction::triggered, this, &graph::add_event);
        connect(m_menu->addAction("Add a chain"), &QAction::triggered, this, &graph::add_chain);
    }
}

void nvt::timeline::graph::add_event(bool) {
    auto n = new node{ this };
    n->move(context_menu_pos);
    n->show();

    m_nodes << n;

    repaint();
}

void nvt::timeline::graph::add_chain(bool) {
    auto n = new chain{ context_menu_pos, this };
    m_chains << n;

    repaint();
}

void nvt::timeline::graph::translate(QPointF position) {
    for (auto i : m_nodes)
        i->move(i->pos() + position.toPoint());

    for (auto i : m_chains)
        i->translate(position);
}

void nvt::timeline::graph::paintEvent(QPaintEvent*) {
    QPainter painter{ this };

    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing, true);

    for (auto i : m_chains) {
        painter.drawPolyline(i->path());
    }
}

void nvt::timeline::graph::contextMenuEvent(QContextMenuEvent* event) {
    context_menu_pos = event->pos();
    m_menu->popup(event->globalPos());
}

void nvt::timeline::graph::mouseMoveEvent(QMouseEvent* event) {
    if (working_mouse_item.has_value()) {
        auto item = working_mouse_item.value();

        if (std::holds_alternative<node*>(item)) {
            auto n = std::get<node*>(item);
            n->move((event->position() - offset).toPoint());

        } else if (std::holds_alternative<chain*>(item)) {
            auto n = std::get<chain*>(item);
            n->set_cursor(event->position().toPoint());
        }

    } else {
        translate(event->position() - offset);
        offset = event->position();
    }

    repaint();
}

void nvt::timeline::graph::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        for (auto i : m_nodes) if (i->hit(event->position())) {
            offset = event->position() - i->pos();
            working_mouse_item = i;
            return;
        }

        for (auto i : m_chains) {
            auto a = i->hit(event->position());
            if (a != std::nullopt) {
                i->set_cursor(a);
                working_mouse_item = i;
                return;
            }
        }

        if (working_mouse_item == std::nullopt)
            offset = event->position();
    }

    repaint();
}

void nvt::timeline::graph::mouseReleaseEvent(QMouseEvent* event) {
    if (working_mouse_item.has_value()) {
        auto item = working_mouse_item.value();

        if (std::holds_alternative<chain*>(item))
            std::get<chain*>(item)->set_cursor(std::nullopt);
    }

    working_mouse_item = std::nullopt;

    repaint();
}
