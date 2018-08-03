#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <readimage.h>
#include <histogram.h>


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<ReadImage>("Readstuff", 1, 0, "ReadImage");
    qmlRegisterType<ReadImage>("Histstuff", 1, 0, "Histogram");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/MyMain.qml")));


    return app.exec();
}
