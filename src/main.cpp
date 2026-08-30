#include "log.hpp"
#include "application.hpp"

int main(int argc, char *argv[]) {
    nvt_widgets::log log;
    log("New run");

    nvt_widgets::application a(argc, argv);
    return a.exec();
}
