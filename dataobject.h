#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include <QObject>

class DataObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)

public:
    DataObject(QObject *parent=0);
    DataObject(const QString &name, const QString &color, QObject *parent=0);

    QString name() const;
    void setName(const QString &name);

    QString color() const;
    void setColor(const QString &color);

signals:
    void nameChanged();
    void colorChanged();

private:
    QString m_name;
    QString m_color;
};

#endif // DATAOBJECT_H


//// the equivalent of your SearchClass
//class Test : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(QStringList model MEMBER m_model NOTIFY modelChanged)
//    QStringList m_model;
//  public slots:
//    void setModel(QString m) {
//      m_model = m.split(" ");
//      modelChanged();
//    }
//  signals:
//    void modelChanged();
//};

//// in main.cpp
//  Test t;
//  engine.rootContext()->setContextProperty("Test", &t);

//// in main.qml
//Column {
//    TextField {
//      onTextChanged: Test.setModel(text)
//    }
//    ListView {
//      width: 200; height: 300
//      spacing: 5
//      model: Test.model
//      delegate: Rectangle {
//        height: 25
//        width: 200
//        color: "lightgray"
//        Text { text: modelData; anchors.centerIn: parent }
//      }
//    }
//  }
