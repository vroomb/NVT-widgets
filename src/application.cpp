#include "application.hpp"
#include "main_window.hpp"
#include "launch_window.hpp"
#include "log.hpp"
#include <QFontDatabase>
#include <project/global.hpp>

nvt::widgets::application::application(int argc, char* argv[]) :
    QApplication(argc, argv)
{
    nvt::widgets::log log;

    /* setup fonts */ {
        int johnston_bold_id =
            QFontDatabase::addApplicationFont(res_dir"johnston-itc/johnston-itc-std-bold.otf");
        int johnston_medium_id =
            QFontDatabase::addApplicationFont(res_dir"johnston-itc/johnston-itc-std-medium.otf");
    
        log("loading johnston bold itc");
        if (johnston_bold_id != -1)
            for (auto& i : QFontDatabase::applicationFontFamilies(johnston_bold_id))
                log("loaded " + i.toStdString());
        else log("could not load johnston bold itc");
    
        log("loading johnston medium itc");
        if (johnston_medium_id != -1)
            for (auto& i : QFontDatabase::applicationFontFamilies(johnston_medium_id))
                log("loaded " + i.toStdString());
        else log("could not load johnston medium itc");
    
        QFont f{ "Johnston ITC Std Medium" };
        f.setPixelSize(20);
        setFont(f);
    
        setCursorFlashTime(0);
    }

    /* setup launch window */ {
        lw = new launch_window;
        connect(lw, &nvt::widgets::launch_window::launchRequested, this,
            [this]() {
                lw->close();
                lw->deleteLater();
                lw = nullptr;
                w = new main_window;
                w->show();
            }
        );
    }
}

int nvt::widgets::application::exec() {
    lw->show();
    return QApplication::exec();
}