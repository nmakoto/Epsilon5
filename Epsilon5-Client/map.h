#pragma once
#include <QObject>
#include <QImage>
#include <QPainter>
#include <QPoint>
#include <QColor>
//------------------------------------------------------------------------------
class TMap : public QObject
{
    Q_OBJECT
public:
    TMap(QObject* parent = 0);
    ~TMap();
    size_t GetWidth() const;
    size_t GetHeight() const;
    QColor GetBackgroundColor() const;
    QSize GetMinimapSize(quint8 maxMinimapSize) const;
    QPoint GetObjectPosOnMinimap(int objectPosX, int objectPosY,
            quint8 maxMinimapSize) const;
    QPoint GetObjectPosOnMinimap(QPoint objectPos, quint8 maxMinimapSize) const;

    // Draw background at correct position
    void DrawBackground(const QPoint& playerPos, const QSize& frameSize,
            QPainter& painter) const;

    const QImage* GetBackground() const;

signals:
    void MapLoaded();

public slots:
    void LoadMap(QString map);

private:
    size_t Width;
    size_t Height;
    QImage* Background;
    QColor Color;
    bool Loaded;
    QPointF WHRatio;
};
//------------------------------------------------------------------------------
