#ifndef READIMAGE_H
#define READIMAGE_H

#include <QRgb>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QVector>
//#include <QPainter>
#include <QByteArray>
#include <QBasicTimer>
#include <QWheelEvent>


#include <QtQuick/QQuickPaintedItem>


class ReadImage : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(quint32 magnification READ Magnification WRITE setMagnification)
    Q_PROPERTY(QString iFileName READ IFileName WRITE openIFileName)

public:
    ReadImage(QQuickItem *parent = 0);

    QStringList listIFiles(QFileInfo);            // for next and prev file buttons
    Q_INVOKABLE bool openIFileName(QString);
    Q_INVOKABLE void setMagnification(int);

    int getBinImage(int, int);

    QString name() const;
    QString IFileName() const;
    void setName(const QString &name);
    void paint(QPainter *painter);
    quint32 Magnification() const;




signals:
    void currFrame(int);

private:
        QFile file, exportFile;
        QString dirStr;
        QString iFileName, iExt, exportFileName;
        QStringList iFileList;
        quint32 filePos, beginPos, endPos;

        double pixelScale;
        quint32 iWidth, iHeight, xPos, yPos;
        quint32  magnification;
        QString m_name;
};

#endif // READIMAGE_H
