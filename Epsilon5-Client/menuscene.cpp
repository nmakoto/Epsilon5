#include <QDebug>
#include <QGraphicsTextItem>

#include "ui/uimenu.h"
#include "application.h"
#include "imagestorage.h"
#include "objects.h"
#include "menuscene.h"
//------------------------------------------------------------------------------
TMenuScene::TMenuScene(QObject* parent)
    : QGraphicsScene(parent)
    , Application(nullptr)
    , ResImages(nullptr)
    , ResObjects(nullptr)
    , MainMenu(new ui::UIMenu)
    , LoadingItemText(new QGraphicsTextItem())
{
    MainMenu->setParent(this);

    LoadingItemText->setFont(QFont("Ubuntu", 28));
    LoadingItemText->setHtml("<span style='background:rgba(0,0,0,0.6);"
            "color:#eee;border-radius:40px;'>" + tr("Loading...") + "</span>");
    LoadingItemText->hide();
    this->addItem(LoadingItemText);

    connect(MainMenu, SIGNAL(clicked(QString)), SLOT(MenuItemClicked(QString)));
}
//------------------------------------------------------------------------------
TMenuScene::~TMenuScene()
{
}
//------------------------------------------------------------------------------
void TMenuScene::Init(const TApplication *application,
        const TImageStorage *images, const TObjects *objects)
{
    Application = application;
    ResImages = images;
    ResObjects = objects;

    MainMenu->addMenuItem("quit",
            QPixmap::fromImage(ResImages->GetImage("menu-exit")),
            QPixmap::fromImage(ResImages->GetImage("menu-exit-h")));
    MainMenu->addMenuItem("connect",
            QPixmap::fromImage(ResImages->GetImage("menu-connect")),
            QPixmap::fromImage(ResImages->GetImage("menu-connect-h")));

    this->addItem(MainMenu);
    MainMenu->grabChildrenEvents();
}
//------------------------------------------------------------------------------
bool TMenuScene::IsValid() const
{
    return Application && ResImages && ResObjects;
}
//------------------------------------------------------------------------------
void TMenuScene::PrepareScene(EMenuStates state)
{
    switch( state ) {
    case MS_MainMenu:
        MainMenu->show();
        LoadingItemText->hide();
        return;
    case MS_Loading:
        MainMenu->hide();
        LoadingItemText->show();
        return;
    }
}
//------------------------------------------------------------------------------
void TMenuScene::MenuItemClicked(const QString& name)
{
    if (name == "quit") {
        emit ActionQuit();
    }
    if (name == "connect") {
        emit ActionConnect();
    }
}
//------------------------------------------------------------------------------
void TMenuScene::SetUiRect(const QRectF &rect)
{
    this->setSceneRect(rect);
    MainMenu->setPos(rect.center() - MainMenu->rect().center());
    LoadingItemText->setPos(rect.bottomRight()
        - LoadingItemText->boundingRect().bottomRight());
}
//------------------------------------------------------------------------------
void TMenuScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect, Qt::black);
}
//------------------------------------------------------------------------------
