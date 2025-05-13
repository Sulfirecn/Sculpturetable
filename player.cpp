#include "player.h"
#include <QPainter>
#include <QDebug>
#include "zombie.h"

player::player(QObject *parent) : QObject(parent)
{
    movie = new QMovie("C:/Users/lenovo/Desktop/bighomework/res/Plants/SunShroom/SunShroom2.gif");
    if (!movie->isValid()) {
        qDebug() << "Failed to load player gif!";
    }
    movie->start();
    int newWidth = static_cast<int>(movie->currentImage().width() * 1.5);
    int newHeight = static_cast<int>(movie->currentImage().height() * 1.5);
    movie->setScaledSize(QSize(newWidth, newHeight));
    posX = 0;
    posY = 0;
    health = 100;
    bulletDamage = 50;
    attackSpeed = 500;
    moveSpeed = 5;
    bulletImage = QImage("path/to/default_bullet.png");
}

player::~player()
{
    delete movie;
}

void player::setPos(int x, int y)
{
    posX = x;
    posY = y;
}

int player::x() const
{
    return posX;
}

int player::y() const
{
    return posY;
}

int player::width() const
{
    return movie->currentImage().width();
}

int player::height() const
{
    return movie->currentImage().height();
}

void player::draw(QPainter *painter)
{
    painter->drawImage(posX, posY, movie->currentImage());
}

void player::updateMovement()
{
    if (isMovingUp && posY > 0) {
        posY -= moveSpeed;
    }
    if (isMovingDown && posY < SCENE_HEIGHT - height()) {
        posY += moveSpeed;
    }
    if (isMovingLeft && posX > 0) {
        posX -= moveSpeed;
    }
    if (isMovingRight && posX < SCENE_WIDTH - width()) {
        posX += moveSpeed;
    }
}

bool player::collidesWith(zombie *z)
{
    int zX = z->x();
    int zY = z->y();
    int zWidth = z->width();
    int zHeight = z->height();
    return (posX < zX + zWidth &&
            posX + width() > zX &&
            posY < zY + zHeight &&
            posY + height() > zY);
}

void player::checkBoundary()
{
    if (posX < 0) {
        posX = 0;
    }
    if (posX > SCENE_WIDTH - width()) {
        posX = SCENE_WIDTH - width();
    }
    if (posY < 0) {
        posY = 0;
    }
    if (posY > SCENE_HEIGHT - height()) {
        posY = SCENE_HEIGHT - height();
    }
}

QMovie* player::getMovie() const
{
    return movie;
}

void player::takeDamage(int damage)
{
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

bool player::isDead() const
{
    return health <= 0;
}

bool player::getIsMovingUp() const
{
    return isMovingUp;
}

bool player::getIsMovingDown() const
{
    return isMovingDown;
}

bool player::getIsMovingLeft() const
{
    return isMovingLeft;
}

bool player::getIsMovingRight() const
{
    return isMovingRight;
}

void player::setIsMovingUp(bool value)
{
    isMovingUp = value;
}

void player::setIsMovingDown(bool value)
{
    isMovingDown = value;
}

void player::setIsMovingLeft(bool value)
{
    isMovingLeft = value;
}

void player::setIsMovingRight(bool value)
{
    isMovingRight = value;
}

int player::getMoveStep() const
{
    return moveStep;
}

void player::setMoveStep(int newStep)
{
    moveStep = newStep;
}

int player::getBulletDamage() const
{
    return bulletDamage;
}

void player::setBulletDamage(int newDamage)
{
    bulletDamage = newDamage;
}

int player::getAttackSpeed() const
{
    return attackSpeed;
}

void player::setAttackSpeed(int newSpeed)
{
    attackSpeed = newSpeed;
}

void player::increaseDamage()
{
    bulletDamage += 5;
}

void player::increaseSpeed()
{
    moveSpeed += 2;
}

int player::getHealth() const
{
    return health;
}

int player::getMoveSpeed() const // 确保定义
{
    return moveSpeed;
}

void player::setMoveSpeed(int newSpeed)
{
    moveSpeed = newSpeed;
}

QImage player::getImage() const
{
    return movie->currentImage();
}
