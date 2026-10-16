#include "yaml_display.hpp"
#include <QBoxLayout>
#include <QPainter>
#include <QLabel>

nvt::widgets::yaml_display::yaml_display(
    YAML::Node node,
    QWidget* parent
): 
    QWidget(parent),
    m_node{ node },
    brush{ QColor{255, 255, 255, 16} },
    pen{ QColor{128, 128, 128}, 5 }
{
    auto grid = new QGridLayout;
    setLayout(grid);
    int n = 0;
    switch (m_node.Type()) {
        case YAML::NodeType::Null:
            grid->addWidget(new QLabel{ "<i>null</i>" });
            break;
        case YAML::NodeType::Scalar:
            grid->addWidget(new QLabel{
                QString::fromStdString(m_node.as<std::string>())
            }, 0, 0);
            break;
        case YAML::NodeType::Sequence:
            for (auto i = node.begin(); i != node.end(); i++) {
                grid->addWidget(new QLabel{ QString::number(n + 1) }, n, 0);
                grid->addWidget(new yaml_display{ node[n] }, n, 1);
                n++;
            }
            break;
        case YAML::NodeType::Map:
            for (auto i = node.begin(); i != node.end(); i++) {
                grid->addWidget(new yaml_display{ i->first  }, n, 0);
                grid->addWidget(new yaml_display{ i->second }, n, 1);
                n++;
            }
            break;
        case YAML::NodeType::Undefined:
            grid->addWidget(new QLabel{ "<i>undefined</i>" });
            break;
    }
}

void nvt::widgets::yaml_display::paintEvent(QPaintEvent*) {
    QPainter painter{ this };
    
    painter.setBrush(brush);
    painter.setPen(pen);

    painter.drawRect(rect());
}
