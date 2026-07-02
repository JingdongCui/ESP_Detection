#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "hostcontroller.h"

int main(int argc, char *argv[])
{
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
