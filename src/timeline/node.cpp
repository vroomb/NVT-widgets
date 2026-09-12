#include <timeline/log.hpp>
#include <timeline/node.hpp>
#include <timeline/graph.hpp>

#include <colors/nvt_colors.hpp>

#include <QGridLayout>
#include <QLineEdit>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>

QColor nvt::timeline::node::ref_background_color{ nvt::color::bse() };
QPen nvt::timeline::node::ref_border{ nvt::color::pri(), 5 };
int nvt::timeline::node::duration{ 256 };

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
    title{ new QLineEdit },
    m_layout{ new QGridLayout },
    timer{ new QTimer },
    station{ new circle },
    background_color{ ref_background_color },
    border{ ref_border },
    alpha{ 0 }
{
    setLayout(m_layout);

    m_layout->setContentsMargins(20, 20, 20, 20);

    auto f = title->font();
    f.setPixelSize(30);
    title->setFont(f);

    title->setPlaceholderText("Event title");
    title->setStyleSheet("QLineEdit {border: 0px; background: transparent;}");

    QFontMetrics fm(f);
    title->resize(
        fm.horizontalAdvance(title->placeholderText()) + 20,
        fm.height()
    );

    auto w = m_layout->contentsMargins();
    this->setFixedWidth(
        station->width() +
        title->width() +
        m_layout->horizontalSpacing() +
        w.left() + w.right()
    );

    m_layout->addWidget(station, 0, 0);
    m_layout->addWidget(title, 1, 1);
    m_layout->setColumnStretch(1, 1);

    connect(timer, &QTimer::timeout, this, &nvt::timeline::node::fade);
    connect(title, &QLineEdit::textChanged, this,
        [this](QString text) {
            log log;
            log("start");
            ++log;
            QFontMetrics fm(title->font());
            title->resize(
                ((text == "") ?
                    fm.horizontalAdvance(title->placeholderText()) :
                    fm.horizontalAdvance(text)) + 20,
                fm.height()
            );
            log(std::to_string(title->width()));
            auto w = m_layout->contentsMargins();
            this->setFixedWidth(
                station->width() +
                title->width() +
                m_layout->horizontalSpacing() +
                w.left() + w.right()
            );
            log(std::to_string(station->width()));
            log(std::to_string(title->width()));
            log(std::to_string(m_layout->horizontalSpacing()));
            log(std::to_string(w.left() + w.right()));
            --log;
            log("end");
        }
    );
}

bool nvt::timeline::node::hit(QPointF position) {
    position = position - pos();
    return (0 < position.x()) && (position.x() < size().width())
        && (0 < position.y()) && (position.y() < size().height());
}

void nvt::timeline::node::fade() {
    if (fade_in) {
        if (alpha < 255) {
            timer->start(duration / 16);
            alpha = alpha + 16;
            background_color.setAlpha(alpha);
            auto c = std::move(border.color());
            c.setAlpha(alpha);
            border.setColor(std::move(c));
            repaint();
        }
    } else {
        if (alpha > 0) {
            timer->start(duration / 16);
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
