#include "log.hpp"
#include "application.hpp"

int main(int argc, char *argv[]) {
    nvt::widgets::log log;
    log("New run");

    nvt::widgets::application a(argc, argv);
    return a.exec();
}
