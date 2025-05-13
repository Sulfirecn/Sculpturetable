#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <QObject>
#include <QMovie>
#include <QImage>
#include "player.h"

class player ;
class zombie : public QObject
{
    Q_OBJECT
public:
    enum ZombieType {
        NormalZombie,
        ConeheadZombie,
        BucketheadZombie
    };
    explicit zombie(ZombieType type, int difficulty, QObject *parent = nullptr);
    ~zombie();
    void setPos(int x, int y);
    int x() const;
    int y() const;
    int width() const;
    int height() const;
    void draw(QPainter *painter);
    void move(player *p);
    bool isOutOfBounds() const;
    void takeDamage(int damage);
    bool isDead() const;
    void showSpawnMark();
    void hideSpawnMark();
    void strengthen(int level, int elapsedTime);
    int getDamage() const;
    QImage getImage() const;
signals:
    void removeRequested(zombie* z); // 添加移除信号
private:
    QMovie *movie;
    int posX;
    int posY;
    int health;
    int baseHealth;
    // 调整成员变量声明顺序，减少-Wreorder警告
    QImage spawnMarkImage;
    int spawnMarkDisplayTime;
    int spawnMarkX;
    int spawnMarkY;
    QTimer *spawnMarkTimer;
    int damage;
    int baseDamage;
    int moveSpeed;
    int baseMoveSpeed;
    const int SPAWN_MARK_OFFSET_Y = 20;
};
#endif // ZOMBIE_H
