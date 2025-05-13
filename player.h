#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QMovie>
#include <QKeyEvent>
#include "gamewindow.h"

class zombie;

class player : public QObject
{
    Q_OBJECT
public:
    explicit player(QObject *parent = nullptr);
    ~player();
    void setPos(int x, int y);
    int x() const;
    int y() const;
    int width() const;
    int height() const;
    void draw(QPainter *painter);
    void updateMovement();
    bool collidesWith(zombie *z);
    void checkBoundary();
    QMovie* getMovie() const;
    void takeDamage(int damage);
    bool isDead() const;
    bool isFiring = false;
    bool getIsMovingUp() const;
    bool getIsMovingDown() const;
    bool getIsMovingLeft() const;
    bool getIsMovingRight() const;
    void setIsMovingUp(bool value);
    void setIsMovingDown(bool value);
    void setIsMovingLeft(bool value);
    void setIsMovingRight(bool value);
    int getMoveStep() const;
    void setMoveStep(int newStep);
    // 新增升级函数
    void increaseDamage();
    void increaseSpeed();
    int getBulletDamage() const;
    QImage getBulletImage() const;
    int getAttackSpeed() const;
    int getHealth() const;
    int getMoveSpeed() const; // 确保声明
    void setMoveSpeed(int newSpeed);
    void setBulletDamage(int newDamage);
    void setAttackSpeed(int newSpeed);
    QImage getImage() const;

private:
    QMovie *movie;
    int posX;
    int posY;
    int moveStep = 10;
    int health;
    bool isMovingUp = false;
    bool isMovingDown = false;
    bool isMovingLeft = false;
    bool isMovingRight = false;
    int bulletDamage = 10;
    QImage bulletImage;
    int bulletUpgradeLevel = 0;
    int moveSpeed;
    int attackSpeed;
};

#endif // PLAYER_H
