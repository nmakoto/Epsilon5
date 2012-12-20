#pragma once
#include <QObject>
#include <QPainter>
//------------------------------------------------------------------------------
class TUiFrame : public QObject
{
    Q_OBJECT
public:
    explicit TUiFrame(QObject* parent);
    ~TUiFrame();

    virtual void Paint(QPainter& painter) = 0;

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
    void SetBackground(const QImage& image, quint8 alpha, bool resizeFrame = true);
    void SetBackgroundScaled(const QImage& image);
    void SetBackgroundScaled(const QImage& image, quint8 alpha);

protected:
    QPixmap GetTransparentImage(const QImage& image, quint8 alpha);

protected:
    QImage* Background;
    QRect FrameRect;
};
//------------------------------------------------------------------------------
