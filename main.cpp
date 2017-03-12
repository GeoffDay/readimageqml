
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlFileSelector>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
  //  QQmlFileSelector* selector = new QQmlFileSelector(&engine);
    return app.exec();
}
