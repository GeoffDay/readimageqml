#ifndef READIMAGE_H
#define READIMAGE_H


#include <QtQuick/QQuickPaintedItem>


class ReadImage : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)

public:
    ReadImage(QQuickItem *parent = 0);

    QString name() const;
    void setName(const QString &name);
    void paint(QPainter *painter);

    Q_INVOKABLE void clearChart();
signals:
    void chartCleared();


private:
    QString m_name;
};

#endif // READIMAGE_H
