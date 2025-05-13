#include "bullet.h"
#include <QPainter>
#include <QDebug>
#include <cmath>

bullet::bullet(QObject *parent) : QObject(parent)
{
    image = QImage("C:/Users/lenovo/Desktop/bighomework/res/bullets/PeaIce/PeaIce_0.png");
    if (image.isNull()) {
        qDebug() << "Failed to load bullet image!";
    }
    posX = 0;
    posY = 0;
    target = nullptr;
    damage = 10;
    moveStep = 5; // 添加缺失的初始化
}

bullet::~bullet()
{
    // 确保不引用已删除的目标
    target = nullptr;
}

void bullet::setPos(int x, int y)
{
    posX = x;
    posY = y;
}

int bullet::x() const
{
    return posX;
}

int bullet::y() const
{
    return posY;
}

int bullet::width() const
{
    return image.width();
}

int bullet::height() const
{
    return image.height();
}

void bullet::move()
{
    // 检查目标有效性和是否被删除
    if (target && !target->isDead()) {
        int dx = target->x() + target->width() / 2 - posX;
        int dy = target->y() + target->height() / 2 - posY;
        double distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 0) {
            double stepX = moveStep * dx / distance;
            double stepY = moveStep * dy / distance;

            posX += static_cast<int>(stepX);
            posY += static_cast<int>(stepY);
        }
    } else {
        // 如果没有目标或目标已死亡，继续向上移动
        posY -= moveStep;
    }

    // 边界检查
    if (posX < 0 || posX > SCENE_WIDTH || posY < 0 || posY > SCENE_HEIGHT) {
        // 超出边界，标记为需要移除
        emit removeRequested(this); // 发出移除请求
    }
}

void bullet::draw(QPainter *painter)
{
    if (painter && !image.isNull()) {
        painter->drawImage(posX, posY, image);
    }
}

bool bullet::collidesWith(zombie *z)
{
    if (!z) return false; // 修复：添加空指针检查

    int bulletLeft = posX;
    int bulletRight = posX + width();
    int bulletTop = posY;
    int bulletBottom = posY + height();

    int zombieLeft = z->x();
    int zombieRight = z->x() + z->width();
    int zombieTop = z->y();
    int zombieBottom = z->y() + z->height();

    return !(bulletRight < zombieLeft || bulletLeft > zombieRight || bulletBottom < zombieTop || bulletTop > zombieBottom);
}

bool bullet::isOutOfBounds()
{
    return posX < 0 || posX > SCENE_WIDTH || posY < 0 || posY > SCENE_HEIGHT;
}

zombie* bullet::getTarget() const
{
    return target;
}

void bullet::setTarget(zombie* newTarget)
{
    target = newTarget;
}

void bullet::upgradeDamage()
{
    damage += 5;
}

int bullet::getDamage() const
{
    return damage;
}

void bullet::setDamage(int newDamage)
{
    damage = newDamage;
}

QPixmap bullet::getSplashPixmap() const
{
    return splashPixmap;
}

QImage bullet::getImage() const
{
    return image;
}
