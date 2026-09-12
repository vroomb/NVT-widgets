#include "nvt_colors.hpp"

std::shared_ptr<nvt::color::colors> nvt::color::colors::m_instance = nullptr;

QColor nvt::color::bse() { return nvt::color::colors::instance()->bse(); }
QColor nvt::color::pri() { return nvt::color::colors::instance()->pri(); }
QColor nvt::color::sec() { return nvt::color::colors::instance()->sec(); }
QColor nvt::color::ter() { return nvt::color::colors::instance()->ter(); }
