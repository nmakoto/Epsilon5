#pragma once

#include <QObject>
#include <QPainter>
#include <QVector>
#include <QString>
#include <QStaticText>
#include <QPoint>
#include <QImage>
#include "imagestorage.h"
#include "../utils/usound.h"

class TApplication;
class TMenu;

class TMenuItem : public QObject
{
    Q_OBJECT
public:
    TMenuItem(const QImage& image, const QImage& imageHover,
              const QPoint& pos, QObject* obj = 0)
        : QObject(obj)
        , Pos(pos)
        , Image(image)
        , ImageHover(imageHover)
    {}

    void paint(QPainter* p);

protected:
    bool event(QEvent*);
private:
    TApplication* Application();
signals:
    void Clicked();
private:
    QPoint Pos;
    const QImage& Image;
    const QImage& ImageHover;
};

class TMenu : public QObject
{
    Q_OBJECT
public:
    explicit TMenu(TImageStorage* images, QObject* parent = 0);
    ~TMenu();
    void paint(QPainter* p);
    TApplication* Application();

    void Init();
    bool event(QEvent*ev);

private slots:
    void OnClickedItem();

private:
    TMenuItem* AddMenuItem(TMenuItem* item);
    QVector<TMenuItem*> Items;
    TImageStorage* Images;
    utils::USound* SoundClick;
};
