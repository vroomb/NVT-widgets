#include <timeline/log.hpp>
#include <timeline/node.hpp>
#include <timeline/graph.hpp>
#include <timeline/chain.hpp>

#include <colors/nvt_colors.hpp>

#include <QGridLayout>
#include <QLineEdit>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QMenu>
#include <QMouseEvent>

const qreal nvt::timeline::circle::outer_radius = 1;
const qreal nvt::timeline::circle::inner_radius = 0.8;

const QColor nvt::timeline::node::ref_background_color{ nvt::color::bse() };
const QPen nvt::timeline::node::ref_border{ nvt::color::sec(), 5 };
const int nvt::timeline::node::duration{ 16 };

const QMargins nvt::timeline::node::m_margins{ 20, 20, 20, 20 };

void nvt::timeline::circle::paintEvent(QPaintEvent*) {
    QPainter painter{ this };
    painter.setRenderHint(QPainter::Antialiasing, true);

    qreal a = size().height() / 2;
    qreal b = size().height() / 2;

    QPointF center{ a, b };

    QPainterPath pp; pp.addEllipse(center,
        a*outer_radius - nvt::timeline::node::ref_border.width(),
        b*outer_radius - nvt::timeline::node::ref_border.width()
    );
    painter.fillPath(pp, nvt::timeline::node::ref_background_color);
    painter.setPen(nvt::timeline::node::ref_border);
    painter.drawPath(pp);
}

nvt::timeline::node::node(graph* parent) :
    QWidget(parent),
    title{ new QLineEdit{this} },
    timer{ new QTimer },
    station{ new circle{this} },
    background_color{ ref_background_color },
    border{ ref_border },
    alpha{ 0 },
    tile{ parent->get_tile_ref() },
    m_menu{ new QMenu }
{
    /* setup title */ {
        auto f = title->font();
        f.setPixelSize(30);
        title->setFont(f);

        title->setPlaceholderText("Event title");
        //title->setStyleSheet("QLineEdit {border: 0px; background: transparent;}");

        QFontMetrics fm{f};
        title->resize(
            fm.horizontalAdvance(title->placeholderText()) + 20,
            fm.height()
        );
    }

    move_title_down();

    /* setup menu */ {
        connect(m_menu->addAction("Detach"), &QAction::triggered, this, []() {});
        connect(m_menu->addAction("Create a chain"), &QAction::triggered, this, []() {});
        connect(m_menu->addAction("Toggle title position"), &QAction::triggered, this,
            [this]() {
                if (up_down) move_title_down();
                else         move_title_up();
            }
        );
    }

    connect(timer, &QTimer::timeout, this, &nvt::timeline::node::fade);
    connect(title, &QLineEdit::textChanged, this,
        [this](QString text) {
            QFontMetrics fm(title->font());
            title->resize(
                ((text == "") ?
                    fm.horizontalAdvance(title->placeholderText()) :
                    fm.horizontalAdvance(text)) + 20,
                fm.height()
            );
            auto w = m_margins;
            this->setFixedWidth(
                station->width() +
                title->width() +
                w.left() + w.right()
            );
        }
    );
}

void nvt::timeline::node::move_title_up() {
    up_down = true;

    auto s = station->size();
    auto t = title->size();

    station->move(m_margins.left(), m_margins.top() + t.height());
    title->move(m_margins.left() + s.width(), m_margins.top());

    resize(childrenRect().size().grownBy(m_margins));

    offset = station->geometry().center();

    center(m_center);
}

void nvt::timeline::node::move_title_down() {
    up_down = false;

    auto s = station->size();
    // auto t = title->size();

    station->move(m_margins.left(), m_margins.top());
    title->move(m_margins.left() + s.width(), m_margins.top() + s.height());

    resize(childrenRect().size().grownBy(m_margins));

    offset = station->geometry().center();

    center(m_center);
}

bool nvt::timeline::node::hit(QPointF position) {
    position = position - pos();
    return (0 < position.x()) && (position.x() < size().width())
        && (0 < position.y()) && (position.y() < size().height());
}

QPointF nvt::timeline::node::translate(QPointF position) {
    m_center = m_center + position;
    return center(m_center);
}

void nvt::timeline::node::silent_translate(QPointF position) {
    m_center = m_center + position;
    move(m_center.toPoint() - offset);
}

QPointF nvt::timeline::node::center(QPointF position) {
    m_center = position;
    move(position.toPoint() - offset);

    emit position_changed(this);

    return m_center;
}

void nvt::timeline::node::fade() {
    if (fade_in) {
        if (alpha < 195) {
            timer->start(duration);
            alpha = alpha + 16;
            background_color.setAlpha(alpha);
            auto c = std::move(border.color());
            c.setAlpha(alpha);
            border.setColor(std::move(c));
            repaint();
        }
    } else {
        if (alpha > 0) {
            timer->start(duration);
            alpha = alpha - 16;
            background_color.setAlpha(alpha);
            auto c = std::move(border.color());
            c.setAlpha(alpha);
            border.setColor(std::move(c));
            repaint();
        }
    }
}

void nvt::timeline::node::enterEvent(QEnterEvent* event) {
    fade_in = true;
    fade();
}

void nvt::timeline::node::leaveEvent(QEvent* event) {
    timer->stop();
    fade_in = false;
    fade();
}

void nvt::timeline::node::paintEvent(QPaintEvent* event) {
    QPainter painter{ this };
    painter.setRenderHint(QPainter::Antialiasing, true);

    auto r = rect();
    auto w = border.width();

    r.adjust(w, w, -2*w, -2*w);

    QPainterPath pp{}; pp.addRoundedRect(r, 20, 20);
    painter.fillPath(pp, background_color);
    painter.setPen(border);
    painter.drawPath(pp);
}

void nvt::timeline::node::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        m_menu->exec(event->globalPosition().toPoint());
    } else {
        event->ignore();
    }
}
