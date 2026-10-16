#include "log.hpp"
#include "application.hpp"
#include <project/global.hpp>
#include <QDir>

int main(int argc, char *argv[]) {
    nvt::widgets::log log;
    log("New run");

    nvt::core::global::open(
        QDir::home().filesystemPath()
#ifdef WIN32
        / "AppData" / "Roaming" / "NVT"
#endif
#ifdef linux
        / ".config" / "NVT"
#endif
    );


    nvt::widgets::application a(argc, argv);
    return a.exec();
}
