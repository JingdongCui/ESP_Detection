#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtGlobal>

#if defined(Q_OS_UNIX)
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sys/resource.h>
#endif

#include "hostcontroller.h"

namespace {
void raiseProcessPriority()
{
#if defined(Q_OS_UNIX)
    errno = 0;
    const int currentNice = getpriority(PRIO_PROCESS, 0);
    if (errno != 0 || currentNice <= 0) {
        return;
    }

    if (setpriority(PRIO_PROCESS, 0, 0) != 0) {
        std::cerr << "host priority unchanged: " << std::strerror(errno) << '\n';
    }
#endif
}
} // namespace

int main(int argc, char *argv[])
{
    raiseProcessPriority();

    QGuiApplication app(argc, argv);

    HostController hostController;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("hostController"), &hostController);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("esp32_host", "Main");

    return QGuiApplication::exec();
}
