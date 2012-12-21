#pragma once
#include <QGLWidget>
#include <QPainter>
//------------------------------------------------------------------------------
class TUiFrame : public QGLWidget
{
    Q_OBJECT
public:
    explicit TUiFrame(QWidget* parent, QGLWidget* widget);
    ~TUiFrame();

    virtual void Paint(QPainter& painter) = 0;

    void SetBackground(const QImage& image, bool resizeFrame = true);
    void SetBackground(const QImage& image, quint8 alpha, bool resizeFrame = true);
    void SetBackgroundScaled(const QImage& image);
    void SetBackgroundScaled(const QImage& image, quint8 alpha);

protected:
    QPixmap GetTransparentImage(const QImage& image, quint8 alpha);

    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
//    bool event(QEvent* event);

protected:
    QImage* Background;
    QPainter* painter;
};
//------------------------------------------------------------------------------
