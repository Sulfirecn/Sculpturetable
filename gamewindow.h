#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include <QGraphicsScene>
#include <QWidget>
#include <QTimer>
#include <QList>
#include <QImage>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include "constants.h"
#include "upgradeselectwindow.h"
#include "levelselectwindow.h"
#include "zombie.h"

class player;
class zombie;
class bullet;
class inspirationsystem;

class gamewindow : public QWidget
{
    Q_OBJECT
public:
    explicit gamewindow(QWidget *parent = nullptr);
    ~gamewindow();
    void setLevelAndDifficulty(int level, int difficulty);
signals:
    void gameStarted();
    // 声明gameEnded信号
    void gameEnded();
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
private slots:
    void spawnZombie();
    void startGame();
    void moveBullets();
    void moveZombies();
    void checkCollisions();
    void moveBackground();
    void autoFire();
    void updatePlayerMovement();
    void playHitSound();
    void onInspirationChanged(int value);
    void onUpgradeOptionSelected(int option);
    void pauseGame();
    void resumeGame();
    void checkPlayerZombieCollisions();
    void updateInfoDisplay();
    void shakeScreen();
    void showGameOver();
    bool pixelCollisionWithTolerance(QObject *obj1, QObject *obj2, int tolerance);
public slots:
    void startGameAfterDifficultySelection(int level, int difficulty);
private:
    // 新增：查找最近僵尸的函数声明
    zombie* findNearestZombie(int x, int y);
    template <typename T>
    void removeObjects(QList<T*> &objectList, QList<T*> &objectsToRemove);
    // 添加removeZombie和removeBullet函数声明
    void removeZombie(zombie* z);
    void removeBullet(bullet* b);
    // 原有成员变量...
    QList<QGraphicsItem*> pausedItems;
    QTimer *zombieSpawnTimer;
    QTimer *bulletMoveTimer;
    QTimer *zombieMoveTimer;
    QTimer *collisionCheckTimer;
    QTimer *autoFireTimer;
    QTimer *playerMovementTimer;
    player *playerCharacter;
    QList<zombie*> zombies;
    QList<bullet*> bullets;
    inspirationsystem *inspirationSystem;
    QImage background;
    int backgroundX;
    int backgroundY;
    bool isGameOver;
    bool isPaused;
    bool isDifficultySelected;
    QMediaPlayer *musicPlayer;
    QAudioOutput *audioOutput;
    QMediaPlayer *hitSoundPlayer;
    QMediaPlayer *preGameBGMPlayer;
    int currentLevel;
    int currentDifficulty;
    UpgradeSelectionWindow *upgradeSelectionWindow;
    LevelSelectWindow *levelSelectWindow;
    QGraphicsScene *scene;
    const int bulletWidth = 10; // 假设子弹宽度
    const int bulletHeight = 10; // 假设子弹高度
    const int COLLISION_TOLERANCE = 20;
    QGraphicsPixmapItem *infoBackground;
    QGraphicsTextItem *healthText;
    QGraphicsTextItem *damageText;
    QGraphicsTextItem *attackSpeedText;
    QGraphicsTextItem *moveSpeedText;
};
#endif // GAMEWINDOW_H
