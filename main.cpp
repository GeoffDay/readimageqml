#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <readimage.h>
#include <dataobject.h>



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<ReadImage>("Readstuff", 1, 0, "ReadImage");
    qmlRegisterType<DataObject>("myModel", 1, 0, "DataObject");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/MyMain.qml")));

    QList<QObject*> dataList;
    dataList.append(new DataObject("Item 1", "red"));
    dataList.append(new DataObject("Item 2", "green"));
    dataList.append(new DataObject("Item 3", "blue"));
    dataList.append(new DataObject("Item 4", "yellow"));

    QQmlContext *context1 = new QQmlContext(engine.rootContext());
    context1->setContextProperty("myModel", QVariant::fromValue(dataList));




    return app.exec();
}
