#include <QGuiApplication>
//#include <QtQuick/QQuickView>
#include <QQmlApplicationEngine>

#include <readimage.h>




int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<ReadImage>("Readstuff", 1, 0, "ReadImage");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));




//    QQuickView view;
//    view.setResizeMode(QQuickView::SizeRootObjectToView);
//    view.setSource(QUrl("qrc:///main.qml"));
//    view.show();

    return app.exec();
}
