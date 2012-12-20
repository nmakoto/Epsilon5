#pragma once
#include <QObject>
#include <QPainter>
//------------------------------------------------------------------------------
class TRespawnFrame : public QObject
{
    Q_OBJECT
public:
    explicit TRespawnFrame(QObject* parent = 0);
    ~TRespawnFrame();

    void Paint(QPainter& painter);

    QRect GetRect() const {
        return FrameRect;
    }
    QSize GetSize() const {
        return FrameRect.size();
    }
    QPoint GetPos() const {
        return FrameRect.topLeft();
    }
    int GetWidth() const {
        return FrameRect.width();
    }
    int GetHeight() const {
        return FrameRect.height();
    }
    int GetX() const {
        return FrameRect.left();
    }
    int GetY() const {
        return FrameRect.top();
    }
    void SetRect(const QRect& rect) {
        FrameRect = rect;
    }
    void SetSize(const QSize& size) {
        FrameRect.setSize(size);
    }
    void SetPos(const QPoint& pos) {
        FrameRect.setTopLeft(pos);
    }
    void SetX(int xPos) {
        FrameRect.setLeft(xPos);
    }
    void SetY(int yPos) {
        FrameRect.setTop(yPos);
    }
    void SetWidth(int width) {
        FrameRect.setWidth(width);
    }
    void SetHeight(int height) {
        FrameRect.setHeight(height);
    }

    void SetBackground(const QImage& image, bool resizeFrame = true);
    void SetBackgroundScaled(const QImage& image);

private:
    QImage* Background;
    QRect FrameRect;
};
//------------------------------------------------------------------------------
