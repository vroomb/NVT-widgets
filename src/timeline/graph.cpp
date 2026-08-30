#include <timeline/graph.hpp>
#include <timeline/chain.hpp>
#include <QMouseEvent>
#include <QMessageBox>

nvt::timeline::graph::graph(QWidget* parent) :
    QWidget(parent)
{
    setBaseSize(50, 50);

    /* initialize pen */ {
        pen.setWidth(10);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
    }

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
    if (std::holds_alternative<node*>(working_mouse_item)) {

        auto n = std::get<node*>(working_mouse_item);
        n->move(event->position().toPoint());

    } else if (std::holds_alternative<chain*>(working_mouse_item)) {

        auto n = std::get<chain*>(working_mouse_item);
        n->set_cursor(event->position().toPoint());
    }

    repaint();
}

void nvt::timeline::graph::mousePressEvent(QMouseEvent* event) {
    for (auto i : m_nodes) if (i->hit(event->position())) {
        working_mouse_item = i;
        return;
    }

    for (auto i : m_chains) if (i->hit(event->position())) {
        working_mouse_item = i;
        return;
    }

    repaint();
}

void nvt::timeline::graph::mouseReleaseEvent(QMouseEvent* event) {
    repaint();
}
