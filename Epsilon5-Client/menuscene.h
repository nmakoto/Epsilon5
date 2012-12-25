#pragma once
#include <QGraphicsScene>
//------------------------------------------------------------------------------
class QGraphicsTextItem;
class TApplication;
class TImageStorage;
class TObjects;
//------------------------------------------------------------------------------
namespace ui
{
class UIMenu;
}
//------------------------------------------------------------------------------
class TMenuScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum EMenuStates {
        MS_MainMenu,
        MS_Loading
    };

public:
    explicit TMenuScene(QObject *parent = 0);
    ~TMenuScene();

    void Init(const TApplication* application, const TImageStorage* images,
            const TObjects* objects);
    bool IsValid() const;
    void PrepareScene(EMenuStates state);
    void SetUiRect(const QRectF& rect);

signals:
    void ActionQuit();
    void ActionConnect();

public slots:

protected:
    void drawBackground(QPainter* painter, const QRectF& rect);
//    void drawForeground(QPainter* painter, const QRectF& rect);

private:

private slots:
    void MenuItemClicked(const QString& name);

private:
    const TApplication* Application;
    const TImageStorage* ResImages;
    const TObjects* ResObjects;
    ui::UIMenu* MainMenu;
    QGraphicsTextItem* LoadingItemText;
};
//------------------------------------------------------------------------------
