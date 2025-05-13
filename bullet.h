#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QImage>
#include "zombie.h"

class bullet : public QObject
{
    Q_OBJECT
public:
    explicit bullet(QObject *parent = nullptr);
    ~bullet();

    void setPos(int x, int y);
    int x() const;
    int y() const;
    int width() const;
    int height() const;
    void move();
    void draw(QPainter *painter);
    bool collidesWith(zombie *z);
    bool isOutOfBounds();
    zombie* getTarget() const;
    void setTarget(zombie* newTarget);
    void upgradeDamage();
    int getDamage() const;
    void setDamage(int newDamage);
    QPixmap getSplashPixmap() const;
    QImage getImage() const;

signals:
    void removeRequested(bullet* b); // 添加移除信号

private:
    QImage image;
    int posX;
    int posY;
    zombie* target;
    int damage;
    int moveStep; // 添加缺失的成员变量
    QPixmap splashPixmap;
};

#endif // BULLET_H
