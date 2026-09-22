#include <timeline/chain.hpp>
#include <timeline/graph.hpp>
#include <timeline/log.hpp>

nvt::timeline::chain::chain(graph* parent) :
    QObject(parent)
{
    update_path();
}

nvt::timeline::chain::chain(QPointF position, graph* parent) :
    QObject(parent),
    tile{ parent->get_tile_ref() }
{
    update_path();
    set_start_pos(position);
}

void nvt::timeline::chain::attach_node(node* add) {
    if (add->chains().contains(this)) { return; }

    auto i = m_nodes.begin();
    for (; i != m_nodes.end(); i++) {
        if (add->x() < std::get<node*>(*i)->x()) break;
    }
    i = m_nodes.insert(i, { add, {} });

    node* p;
    node* n;

    if (i == m_nodes.begin()) p = nullptr;
    else {
        i--;
        p = std::get<node*>(*i);
        (*i).second.clear();
        i++;
    }

    i++;

    if (i == m_nodes.end()) n = nullptr;
    else {
        n = std::get<node*>(*i);
    }


    add->add_chain(this, p, n);
    update_path();

    connect(add, &node::position_changed, this, &chain::update_node);
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
            result = ((((a <= x) && (x <= c)) || ((c <= x) && (x <= a))) && (abs(y - b) < l));
            continue;
        }
        else if (a == c) {
            result = ((((b <= y) && (y <= d)) || ((d <= y) && (y <= b))) && (abs(x - a) < l));
            continue;
        }

        auto tan_m = (b - d) / (a - c);
        auto csc_m = sqrt(1 + tan_m * tan_m) / tan_m;

        result = (((tan_m * (x - a - l * csc_m) + b) <= y) && (y <= (tan_m * (x - a + l * csc_m) + b)))
            && (((((-1 / tan_m) * (x - a) + b) <= y) && (y <= ((-1 / tan_m) * (x - c) + d)))
                || ((((-1 / tan_m) * (x - a) + b) >= y) && (y >= ((-1 / tan_m) * (x - c) + d))));
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

void nvt::timeline::chain::set_start_pos(QPointF position) {
    m_path.translate(position - m_path[0]);
    emit path_changed();
}

void nvt::timeline::chain::translate(QPointF position) {
    m_path.translate(position);

    // for (auto i = m_nodes.begin(); i != m_nodes.end(); i++) {
    //     auto p = std::move((*i).second);
    //     p.translate(position);
    //     (*i).second = std::move(p);
    // }

    emit path_changed();
}

void nvt::timeline::chain::silent_translate(QPointF position) {
    m_path.translate(position);

    for (auto i = m_nodes.begin(); i != m_nodes.end(); i++) {
        (*i).second.translate(position);
    }
}

/*

Here to explain this function, here we go.

i is the iterator pointing to the node to be updated,
j is the iterator after the valid position of updated node.

we want to keep the function short in the case the position
does not need to update. that is why we do a j--; if (i == j)
as it verifies if the position is valid.

in the case the position does need to update, we erase(i)
and insert it at just before j.

this code is insanely messy because for some reason someone
up in the c++ standards committee decided we shouldn't have
an i.next() function

even java got this right.

*/
//
void nvt::timeline::chain::update_node(node* add) {
    auto i = m_nodes.end();
    auto j = m_nodes.end();
    for (auto it = m_nodes.begin(); it != m_nodes.end(); it++) {
        if (i == m_nodes.end() && (*it).first == add) {
            i = it;
        }

        if (j == m_nodes.end() && add->x() < (*it).first->x()) {
            j = it;
        }
    }

    if (j == m_nodes.begin()) {
        (*j).second.clear();
        m_nodes.erase(i);
        m_nodes.insert(j, { add, {} });
        add->chain_update(this, nullptr, (*j).first);

    } else {
        j--;
        if (i == j) {
            (*i).second.clear();

            if (i != m_nodes.begin()) {
                j--;
                (*j).second.clear();
            }
        } else {
            node* n = nullptr;
            node* p = (*j).first;

            (*j).second.clear();
            j++;
            if (j != m_nodes.end()) {
                n = (*j).first;
            }

            if (i != m_nodes.begin()) {
                auto k = i; k--;
                (*k).second.clear();
            }

            m_nodes.erase(i);
            m_nodes.insert(j, { add, {} });

            add->chain_update(this, p, n);
        }
    }

    update_path();
}

void nvt::timeline::chain::update_path() {
    m_path.clear();
    for (auto i = m_nodes.begin(); i != m_nodes.end(); i++) {
        if ((*i).second.isEmpty()) {
            auto j = i; j++;

            if (j != m_nodes.end())
                (*i).second = VerticesHH((*i).first->get_center(), (*j).first->get_center());
            else
                (*i).second << (*i).first->get_center();
        }
        m_path << (*i).second;
    }

    if (m_path.size() > 0) {
        m_path.push_front(QPointF{ m_path.front().x() - tile->width(), m_path.front().y() });
        m_path.push_back(QPointF{ m_path.back().x() + tile->width(), m_path.back().y()});
    } else {
        m_path << QPointF{ -tile->width() / 2, 0 };
        m_path << QPointF{  tile->width() / 2, 0 };
    }
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