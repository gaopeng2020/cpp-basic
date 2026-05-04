#include "AdtConsistencyCheckApp.h"
// #include "logger.h"
#include "common-utils/Log.hpp"

#include <QApplication>

int main(int argc, char *argv[]) {
    // Logger::Init(argv[0]);
    common_utils::log::Log::init(argv[0],common_utils::log::Log::Kind::Debug,true,true,5);
    QApplication a(argc, argv);

    //在退出时调用Logger::Shutdown();
    QObject::connect(&a, &QApplication::aboutToQuit, [] {
        // Logger::Shutdown();
      common_utils::log::Log::shutdown();
    });

    AdtConsistencyCheckApp w;
    w.show();
    return QApplication::exec();
}
