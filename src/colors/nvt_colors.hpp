#pragma once

#include <QColor>

// #ifdef    COLORLIB
// #define  COLOR_API __declspec(dllexport)
// #else
// #define  COLOR_API __declspec(dllimport)
// #endif // COLORLIB

namespace nvt {
    namespace color {
        //class color;
        class colors;

        QColor bse();
        QColor pri();
        QColor sec();
        QColor ter();
    }
}

class nvt::color::colors {
public:
    colors() {}

    static std::shared_ptr<colors> instance() {
        if (m_instance == nullptr) {
            m_instance = std::make_shared<colors>();
        }
        return std::shared_ptr{m_instance};
    }

    QColor bse() const { return m_bse; }
    QColor pri() const { return m_pri; }
    QColor sec() const { return m_sec; }
    QColor ter() const { return m_ter; }

    void set_bse(QColor bse) { m_bse = bse; }
    void set_pri(QColor pri) { m_pri = pri; }
    void set_sec(QColor sec) { m_sec = sec; }
    void set_ter(QColor ter) { m_ter = ter; }

private:
    QColor m_bse = QColor{31, 31, 31};
    QColor m_pri = QColor{206, 189, 217};
    QColor m_sec = QColor{143, 216, 218};
    QColor m_ter = QColor{214, 218, 143};

    static std::shared_ptr<colors> m_instance;
};
