#pragma once
#include <QObject>
#include <QHash>
#include <QImage>
//------------------------------------------------------------------------------
class TObjects : public QObject
{
    Q_OBJECT
    typedef QHash<QString, QImage*> TImageHash;
    typedef QHash<size_t, QImage*> TImageIdHash;
public:
    explicit TObjects(QObject* parent = 0);
    ~TObjects();
    void LoadObjects(const QString& fileName);
    QImage* GetImageById(size_t id) const;
    QImage* GetImageByName(const QString& imageName) const;
private:
    TImageHash Images;
    TImageIdHash ImagesById;
};
//------------------------------------------------------------------------------
