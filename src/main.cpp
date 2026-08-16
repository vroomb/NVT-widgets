#include "nvt.hpp"
#include "log.hpp"
#include "application.hpp"

int main(int argc, char *argv[])
{
    nvt::log log;
    log("New run");

    nvt::application a(argc, argv);
    return a.exec();
}
