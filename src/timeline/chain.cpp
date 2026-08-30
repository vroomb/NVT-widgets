#include <timeline/chain.hpp>
#include <timeline/graph.hpp>

nvt::timeline::chain::chain(graph* parent) :
    QObject(parent)
{}

nvt::timeline::chain::chain(QPointF position, graph* parent) :
    QObject(parent)
{
    translate(position);
}

QPolygonF nvt::timeline::chain::path() {
    return m_path;
}

// questions? consult this graph: https://www.desmos.com/calculator/imc5insskm
std::optional<QPointF> nvt::timeline::chain::hit(QPointF position) {
    bool result = false;

    qreal l = stroke_width() + hit_width();

    auto x = position.x();
    auto y = position.y();

    int i = 0;
    for (; (i < (m_path.length() - 1)) && (result == false); i++) {
        auto a = m_path[i].x();
        auto b = m_path[i].y();
        auto c = m_path[i + 1].x();
        auto d = m_path[i + 1].y();

        if (b == d) {
            result = ((((a < x) && (x < c)) || ((c < x) && (x < a))) && (abs(y - b) < l));
            continue;
        }
        else if (a == c) {
            result = ((((b < y) && (y < d)) || ((d < y) && (y < b))) && (abs(x - a) < l));
            continue;
        }

        auto tan_m = (b - d) / (a - c);
        auto csc_m = sqrt(1 + tan_m * tan_m) / tan_m;

        result = (((tan_m * (x - a - l * csc_m) + b) < y) && (y < (tan_m * (x - a + l * csc_m) + b)))
            && (((((-1 / tan_m) * (x - a) + b) < y) && (y < ((-1 / tan_m) * (x - c) + d)))
                || ((((-1 / tan_m) * (x - a) + b) > y) && (y > ((-1 / tan_m) * (x - c) + d))));
    }

    if (result == false) return std::nullopt;

    auto a = m_path[i - 1].x();
    auto b = m_path[i - 1].y();
    auto c = m_path[i].x();
    auto d = m_path[i].y();

    if (b == d)
        return QPointF{ x, b };
    else if (a == c)
        return QPointF{ a, y };
    else {
        auto m = (b - d) / (a - c);
        auto h = (x + m * (y + m * a - b)) / (m * m + 1);
        auto k = m * (h - a) + b;
        return QPointF{ h, k };
    }
}

std::optional<QPointF> nvt::timeline::chain::cursor() {
    return m_cursor;
}

void nvt::timeline::chain::set_cursor(std::optional<QPointF> point) {
    m_cursor = point;
}

void nvt::timeline::chain::translate(QPointF position) {
    start_point += position;
    end_point += position;
    m_path.translate(position);
}

void nvt::timeline::chain::update_path() {
    QPolygonF poly{start_point, end_point};
    if (m_cursor.has_value()) poly << m_cursor.value();
    for (auto i : m_nodes) poly << i->pos();

    auto mode = m_mode;

    std::sort(poly.begin(), poly.end(),
        [mode](QPointF lhs, QPointF rhs) {
            if (mode == horizontal) return lhs.x() < rhs.x();
            else                    return lhs.y() < rhs.y();
        }
    );

    m_path.clear(); int i = 0;
    for (; i < poly.size() - 1; i++) {
        m_path << VerticesHH(poly[i], poly[i + 1]);
    }
    m_path << poly[i];

    emit path_changed();
}

QPolygonF nvt::timeline::chain::VerticesVH(QPointF FromV, QPointF ToH) {
    qreal a = ToH.x() - FromV.x();
    qreal b = ToH.y() - FromV.y();

    qreal B = nvt::sgn(b) * nvt::max(std::abs(b) - std::abs(a), 0.);
    qreal A = nvt::sgn(a * b) * (b - B);

    return QPolygonF{ 
        FromV, 
        {FromV.x(), (FromV.y() + B)},
        {(FromV.x() + A), ToH.y()},
    };
}

QPolygonF nvt::timeline::chain::VerticesHV(QPointF FromH, QPointF ToV) {
    qreal a = FromH.x() - ToV.x();
    qreal b = FromH.y() - ToV.y();

    qreal B = nvt::sgn(b) * nvt::max(std::abs(b) - std::abs(a), 0.);
    qreal A = nvt::sgn(a * b) * (b - B);

    return QPolygonF{ 
        FromH, 
        {(ToV.x() + A), FromH.y()},
        {ToV.x(), (ToV.y() + B)},
    };
}

QPolygonF nvt::timeline::chain::VerticesVV(QPointF FromV, QPointF ToV) {
    auto median = (FromV + ToV) / 2;

    auto p = VerticesVH(FromV, median);
    p += VerticesHV(median, ToV);

    return p;
}

QPolygonF nvt::timeline::chain::VerticesHH(QPointF FromH, QPointF ToH) {
    auto median = (FromH + ToH) / 2;

    auto p = VerticesHV(FromH, median);
    p += VerticesVH(median, ToH);

    return p;
}