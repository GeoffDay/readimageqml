#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <readimage.h>
#include <dataobject.h>



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<ReadImage>("Readstuff", 1, 0, "ReadImage");
    qmlRegisterType<DataObject>("Displaystuff", 1, 0, "DataObject");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/MyMain.qml")));





    return app.exec();
}
