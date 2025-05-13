#include "gamewindow.h"
#include "player.h"
#include "bullet.h"
#include "zombie.h"
#include "inspirationsystem.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDebug>
#include <QtMath>
#include <cstdlib>
#include <QImage>
#include <QRgb>
#include <QGraphicsDropShadowEffect>

gamewindow::gamewindow(QWidget *parent) : QWidget(parent) {

    setFixedSize(SCENE_WIDTH, SCENE_HEIGHT);
    setWindowTitle("linggangguliguli"); // 设置窗口标题
    setWindowFlags(windowFlags() | Qt::Window); // 确保是顶级窗口
    setFocusPolicy(Qt::StrongFocus); // 设置强焦点策略


    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);

    playerCharacter = new player(this);
    playerCharacter->setPos(SCENE_WIDTH / 2, SCENE_HEIGHT / 2);

    inspirationSystem = new inspirationsystem(this);
    connect(inspirationSystem, &inspirationsystem::inspirationChanged, this, &gamewindow::onInspirationChanged);

    background = QImage("C:/Users/lenovo/Desktop/bighomework/v2-81345c3d2017060f0993b192f40e5397_r.jpg");
    backgroundX = 0;
    backgroundY = 0;

    isGameOver = false;
    isPaused = false;
    isDifficultySelected = false;

    musicPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    musicPlayer->setAudioOutput(audioOutput);
    musicPlayer->setSource(QUrl::fromLocalFile("C:/Users/lenovo/Desktop/bgm111111.MP3"));

    hitSoundPlayer = new QMediaPlayer(this);
    QAudioOutput *hitAudioOutput = new QAudioOutput(this);
    hitSoundPlayer->setAudioOutput(hitAudioOutput);
    hitSoundPlayer->setSource(QUrl::fromLocalFile("C:/Users/lenovo/Desktop/splat1.mp3"));

    preGameBGMPlayer = new QMediaPlayer(this);
    QAudioOutput *preGameAudioOutput = new QAudioOutput(this);
    preGameBGMPlayer->setAudioOutput(preGameAudioOutput);
    preGameBGMPlayer->setSource(QUrl::fromLocalFile("C:/Users/lenovo/Desktop/linggangu.mp3.MP3"));

    zombieSpawnTimer = new QTimer(this);
    connect(zombieSpawnTimer, &QTimer::timeout, this, &gamewindow::spawnZombie);

    bulletMoveTimer = new QTimer(this);
    connect(bulletMoveTimer, &QTimer::timeout, this, &gamewindow::moveBullets);

    zombieMoveTimer = new QTimer(this);
    connect(zombieMoveTimer, &QTimer::timeout, this, &gamewindow::moveZombies);

    collisionCheckTimer = new QTimer(this);
    connect(collisionCheckTimer, &QTimer::timeout, this, &gamewindow::checkCollisions);

    autoFireTimer = new QTimer(this);
    connect(autoFireTimer, &QTimer::timeout, this, &gamewindow::autoFire);

    playerMovementTimer = new QTimer(this);
    connect(playerMovementTimer, &QTimer::timeout, this, &gamewindow::updatePlayerMovement);

    upgradeSelectionWindow = new UpgradeSelectionWindow(this);
    connect(upgradeSelectionWindow, &UpgradeSelectionWindow::optionSelected, this, &gamewindow::onUpgradeOptionSelected);

    // infoBackground = new QGraphicsPixmapItem(QPixmap("C:/Users/lenovo/Desktop/bighomework/v2-b247fdf4c050ac45db753c0b93db4166_r.jpg"));
    // infoBackground->setPos(10, 10);
    // scene->addItem(infoBackground);

    healthText = new QGraphicsTextItem();
    healthText->setFont(QFont("Arial", 12));
    healthText->setPos(20, 20);
    scene->addItem(healthText);

    damageText = new QGraphicsTextItem();
    damageText->setFont(QFont("Arial", 12));
    damageText->setPos(20, 40);
    scene->addItem(damageText);

    attackSpeedText = new QGraphicsTextItem();
    attackSpeedText->setFont(QFont("Arial", 12));
    attackSpeedText->setPos(20, 60);
    scene->addItem(attackSpeedText);

    moveSpeedText = new QGraphicsTextItem();
    moveSpeedText->setFont(QFont("Arial", 12));
    moveSpeedText->setPos(20, 80);
    scene->addItem(moveSpeedText);

    updateInfoDisplay();
}

// gamewindow的析构函数示例
gamewindow::~gamewindow() {
    // 停止所有定时器
    if (zombieSpawnTimer->isActive()) zombieSpawnTimer->stop();
    if (bulletMoveTimer->isActive()) bulletMoveTimer->stop();
    if (zombieMoveTimer->isActive()) zombieMoveTimer->stop();
    if (collisionCheckTimer->isActive()) collisionCheckTimer->stop();
    if (autoFireTimer->isActive()) autoFireTimer->stop();
    if (playerMovementTimer->isActive()) playerMovementTimer->stop();

    // 清空对象列表
    qDeleteAll(zombies);
    zombies.clear();
    qDeleteAll(bullets);
    bullets.clear();

    // 删除其他资源
    delete playerCharacter;
    delete inspirationSystem;
    delete musicPlayer;
    delete audioOutput;
    delete hitSoundPlayer;
    delete preGameBGMPlayer;
    delete zombieSpawnTimer;
    delete bulletMoveTimer;
    delete zombieMoveTimer;
    delete collisionCheckTimer;
    delete autoFireTimer;
    delete playerMovementTimer;
    delete upgradeSelectionWindow;
    delete scene;
}

void gamewindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    // 绘制背景
    if (!background.isNull()) {
        // 绘制滚动背景
        painter.drawImage(backgroundX, backgroundY, background);
        painter.drawImage(backgroundX + background.width(), backgroundY, background);
    }

    // 绘制玩家
    if (playerCharacter) {
        playerCharacter->draw(&painter);
    }

    // 绘制子弹
    for (bullet* b : bullets) {
        if (b) {
            b->draw(&painter);
        }
    }

    // 绘制僵尸
    for (zombie* z : zombies) {
        if (z) {
            z->draw(&painter);
        }
    }

    // 绘制其他UI元素
    scene->render(&painter);
}
void gamewindow::setLevelAndDifficulty(int level, int difficulty) {
    currentLevel = level;
    currentDifficulty = difficulty;
    isDifficultySelected = true;
    startGame();
}

void gamewindow::startGame() {
    if (isDifficultySelected) {
        preGameBGMPlayer->stop();
        musicPlayer->play();

        zombieSpawnTimer->start(2000);
        bulletMoveTimer->start(30);
        zombieMoveTimer->start(30);
        collisionCheckTimer->start(30);
        autoFireTimer->start(playerCharacter->getAttackSpeed());
        playerMovementTimer->start(30);

        emit gameStarted();
    }
}

void gamewindow::startGameAfterDifficultySelection(int level, int difficulty) {
    setLevelAndDifficulty(level, difficulty);
}

void gamewindow::spawnZombie() {
    if (isDifficultySelected &&!isPaused) {
        // 调整僵尸生成位置，使其更靠近屏幕中间
        int randomX = std::rand() % (SCENE_WIDTH - 200) + 100;
        int randomY = std::rand() % (SCENE_HEIGHT - 200) + 100;

        zombie::ZombieType randomType = static_cast<zombie::ZombieType>(std::rand() % 3);
        zombie *newZombie = new zombie(randomType, currentDifficulty, this);
        newZombie->setPos(randomX, randomY);
        newZombie->showSpawnMark();
        zombies.append(newZombie);

        // 连接僵尸删除信号
        connect(newZombie, &zombie::removeRequested, this, &gamewindow::removeZombie);

        // 加载灵感图标图片
        QImage inspirationIconImage("C:/Users/lenovo/Desktop/light.png");
        if (!inspirationIconImage.isNull()) {
            // 获取图片中白色部分的实际颜色值
            QRgb whitePixelColor = inspirationIconImage.pixel(0, 0);
            QColor exactWhiteColor(whitePixelColor);
            QImage mask = inspirationIconImage.createMaskFromColor(exactWhiteColor.rgb(), Qt::MaskOutColor);
            inspirationIconImage = inspirationIconImage.convertToFormat(QImage::Format_ARGB32);
            inspirationIconImage.setAlphaChannel(mask);
        }

        // 将处理后的图片转换为 QPixmap
        QPixmap inspirationIconPixmap = QPixmap::fromImage(inspirationIconImage);

        QGraphicsPixmapItem *inspirationIcon = new QGraphicsPixmapItem(inspirationIconPixmap);
        inspirationIcon->setPos(randomX, randomY);
        inspirationIcon->setScale(0.5);
        inspirationIcon->setOpacity(1.0);
        scene->addItem(inspirationIcon);

        QTimer::singleShot(500, [this, inspirationIcon]() {
            inspirationIcon->setOpacity(0.0);
            QTimer::singleShot(500, [this, inspirationIcon]() {
                if (scene) {
                    scene->removeItem(inspirationIcon);
                }
                delete inspirationIcon;
            });
        });
    }
}

void gamewindow::moveBullets() {
    QList<bullet*> bulletsToRemove;
    for (bullet *b : bullets) {
        b->move();
        if (b->isOutOfBounds()) {
            bulletsToRemove.append(b);
        }
    }

    // 使用安全的移除方法
    removeObjects(bullets, bulletsToRemove);
    update();
}

void gamewindow::moveZombies() {
    QList<zombie*> zombiesToRemove;
    for (zombie *z : zombies) {
        z->move(playerCharacter);
        if (z->isOutOfBounds()) {
            zombiesToRemove.append(z);
        }
    }

    // 使用安全的移除方法
    removeObjects(zombies, zombiesToRemove);
    update();
}

// void gamewindow::checkPlayerZombieCollisions() {
//     if (!playerCharacter) return;
//     for (zombie* z : zombies) {
//         if (z->isDead()) continue;

//         // 使用像素级碰撞检测，带20像素容错
//         if (pixelCollisionWithTolerance(playerCharacter, z, 20)) {
//             // 触发屏幕震动效果
//             shakeScreen();
//             // 处理玩家受伤逻辑
//             playerCharacter
//                 ->takeDamage(z->getDamage());
//             if (playerCharacter->getHealth() <= 0) {
//                 showGameOver();
//             }
//         }
//     }
// }
bool gamewindow::pixelCollisionWithTolerance(QObject *obj1, QObject *obj2, int tolerance)
{
    bullet
        *b = qobject_cast<bullet*>(obj1);
    player
        *p = qobject_cast<player*>(obj1);
    zombie
        *z = qobject_cast<zombie*>(obj2);

    if ((!b &&!p) ||!z) return false;

    int left1, right1, top1, bottom1;
    if (b) {
        left1
            = b->x();
        right1
            = b->x() + b->width();
        top1
            = b->y();
        bottom1
            = b->y() + b->height();
    } else if (p) {
        left1
            = p->x();
        right1
            = p->x() + p->width();
        top1
            = p->y();
        bottom1
            = p->y() + p->height();
    }

    int zombieLeft = z->x();
    int zombieRight = z->x() + z->width();
    int zombieTop = z->y();
    int zombieBottom = z->y() + z->height();

    // 计算重叠区域
    int overlapLeft = qMax(left1, zombieLeft);
    int overlapTop = qMax(top1, zombieTop);
    int overlapRight = qMin(right1, zombieRight);
    int overlapBottom = qMin(bottom1, zombieBottom);

    // 检查重叠区域是否满足容错要求
    int overlapWidth = overlapRight - overlapLeft;
    int overlapHeight = overlapBottom - overlapTop;
    if (overlapWidth < tolerance || overlapHeight < tolerance) {
        return false;
    }

    // 获取图像
    QImage image1
        ;
    if (b) {
        image1
            = b->getImage();
    } else if (p) {
        image1
            = p->getImage();
    }
    QImage zombieImage
        = z->getImage();

    // 像素级碰撞检测
    for (int y = overlapTop; y < overlapBottom; y++) {
        for (int x = overlapLeft; x < overlapRight; x++) {
            int x1 = x - left1;
            int y1 = y - top1;
            int zombieX = x - zombieLeft;
            int zombieY = y - zombieTop;

            // 检查两个图像在该点的像素是否都不透明（alpha > 0）
            if (image1.pixelColor(x1, y1).alpha() > 0 &&
                zombieImage
                        .pixelColor(zombieX, zombieY).alpha() > 0) {
                return true;
            }
        }
    }

    return false;
}

void gamewindow::checkCollisions() {
    if (!playerCharacter) return; // 确保玩家存在
    QList<bullet*> bulletsToRemove;
    QList<zombie*> zombiesToRemove;
    // 复制zombies列表以避免迭代过程中修改容器
    QList<zombie*> zombiesCopy = zombies;
    for (bullet *b : bullets) {
        for (zombie *z : zombiesCopy) {
            // 检查对象是否仍然有效
            if (!bullets.contains(b) ||!zombies.contains(z)) continue;
            // 使用像素级碰撞检测，带20像素容错
            if (pixelCollisionWithTolerance(b, z, 20)) {
                z->takeDamage(b->getDamage());
                if (z->isDead()) {
                    zombiesToRemove.append(z);
                    inspirationSystem->addInspiration(10);
                }
                QGraphicsPixmapItem *splashItem = new QGraphicsPixmapItem(b->getSplashPixmap());
                splashItem->setPos(b->x(), b->y());
                scene->addItem(splashItem);
                QTimer::singleShot(500, [splashItem, this]() {
                    if (scene) { // 修复：检查scene是否存在
                        scene->removeItem(splashItem);
                    }
                    delete splashItem;
                });
                bulletsToRemove.append(b);
                playHitSound();
                break;
            }
        }
    }
    // 玩家与僵尸的碰撞检测部分也使用修改后的函数
    for (zombie *z : zombiesCopy) {
        if (!zombies.contains(z) ||!playerCharacter) continue;
        if (pixelCollisionWithTolerance(playerCharacter, z, 20)) {
            playerCharacter->takeDamage(z->getDamage());
            if (playerCharacter->getHealth() <= 0) {
                showGameOver();
            }
        }
    }
    // 使用安全的移除方法
    removeObjects(bullets, bulletsToRemove);
    removeObjects(zombies, zombiesToRemove);
    update();
}
void gamewindow::moveBackground() {
    backgroundX -= 1;
    if (backgroundX <= -background.width()) {
        backgroundX = 0;
    }
    update();
}

void gamewindow::autoFire() {
    if (!isPaused && playerCharacter) {
        bullet *newBullet = new bullet(this);
        newBullet->setPos(playerCharacter->x() + playerCharacter->width() / 2,
                          playerCharacter->y() + playerCharacter->height() / 2);
        newBullet->setDamage(playerCharacter->getBulletDamage());

        zombie *nearestZombie = findNearestZombie(playerCharacter->x(), playerCharacter->y());
        if (nearestZombie) {
            newBullet->setTarget(nearestZombie);
        }

        bullets.append(newBullet);

        // 连接子弹删除信号
        connect(newBullet, &bullet::removeRequested, this, &gamewindow::removeBullet);
    }
}

void gamewindow::updatePlayerMovement() {
    if (playerCharacter) { // 修复：检查playerCharacter是否存在
        playerCharacter->updateMovement();
        playerCharacter->checkBoundary();
    }
    update();
}

void gamewindow::playHitSound() {
    if (hitSoundPlayer) {
        if (hitSoundPlayer->playbackState() == QMediaPlayer::PlayingState) {
            hitSoundPlayer->stop();
        }
        hitSoundPlayer->play();
    }
}

void gamewindow::onInspirationChanged(int value) {
    if (value >= 50) {
        inspirationSystem->resetInspiration();
        pauseGame();
        this->setEnabled(false); // 禁用游戏窗口输入
        upgradeSelectionWindow->show();
    }
}

void gamewindow::onUpgradeOptionSelected(int option) {
    if (playerCharacter) {
        switch (option) {
        case 1:
            playerCharacter->increaseDamage();
            break;
        case 2:
            playerCharacter->increaseSpeed();
            break;
        case 3:
            zombieSpawnTimer->setInterval(zombieSpawnTimer->interval() + 500);
            break;
        case 4:
            autoFireTimer->setInterval(autoFireTimer->interval() - 50);
            break;
        default:
            break;
        }
        updateInfoDisplay();
    }

    // 恢复游戏
    this->setEnabled(true); // 恢复游戏窗口输入
    resumeGame();
}

void gamewindow::pauseGame() {
    isPaused = true;
    zombieSpawnTimer->stop();
    bulletMoveTimer->stop();
    zombieMoveTimer->stop();
    collisionCheckTimer->stop();
    autoFireTimer->stop();
    playerMovementTimer->stop();
    musicPlayer->pause();

    // 可选：显示暂停界面
    QGraphicsTextItem *pauseText = new QGraphicsTextItem("Game Paused");
    pauseText->setFont(QFont("Arial", 24));
    pauseText->setPos(SCENE_WIDTH / 2 - pauseText->boundingRect().width() / 2,
                      SCENE_HEIGHT / 2 - pauseText->boundingRect().height() / 2);
    scene->addItem(pauseText);

    // 存储暂停界面元素以便恢复时删除
    pausedItems.append(pauseText);
}

void gamewindow::resumeGame() {
    isPaused = false;

    // 恢复定时器
    zombieSpawnTimer->start(2000);
    bulletMoveTimer->start(30);
    zombieMoveTimer->start(30);
    collisionCheckTimer->start(30);
    autoFireTimer->start(playerCharacter->getAttackSpeed());
    playerMovementTimer->start(30);
    musicPlayer->play();

    // 移除暂停界面元素
    foreach(QGraphicsItem* item, pausedItems) {
        scene->removeItem(item);
        delete item;
    }
    pausedItems.clear();
}

void gamewindow::removeBullet(bullet* b) {
    if (b && bullets.contains(b)) {
        bullets.removeOne(b);
        delete b;
    }
}

void gamewindow::removeZombie(zombie* z) {
    if (z && zombies.contains(z)) {
        zombies.removeOne(z);
        delete z;
    }
}

zombie* gamewindow::findNearestZombie(int x, int y) {
    zombie *nearest = nullptr;
    int minDistance = INT_MAX;

    // 使用副本进行查找，避免迭代过程中修改容器
    QList<zombie*> zombiesCopy = zombies;

    for (zombie *z : zombiesCopy) {
        // 确保只考虑有效僵尸
        if (!zombies.contains(z)) continue;

        int dx = z->x() - x;
        int dy = z->y() - y;
        int distance = dx * dx + dy * dy;

        if (distance < minDistance) {
            minDistance = distance;
            nearest = z;
        }
    }
    return nearest;
}

template <typename T>
void gamewindow::removeObjects(QList<T*> &objectList, QList<T*> &objectsToRemove) {
    for (T *obj : objectsToRemove) {
        if (objectList.contains(obj)) {
            objectList.removeOne(obj);
            delete obj;
        }
    }
}

void gamewindow::updateInfoDisplay() {
    if (playerCharacter) {
        // 设置字体大小为16，颜色为红色
        QFont font("Arial", 16);
        QColor redColor(Qt::red);

        healthText->setFont(font);
        healthText->setDefaultTextColor(redColor);
        healthText->setPlainText("生命值: " + QString::number(playerCharacter->getHealth()));

        damageText->setFont(font);
        damageText->setDefaultTextColor(redColor);
        damageText->setPlainText("攻击力: " + QString::number(playerCharacter->getBulletDamage()));

        attackSpeedText->setFont(font);
        attackSpeedText->setDefaultTextColor(redColor);
        attackSpeedText->setPlainText("攻击速度: " + QString::number(playerCharacter->getAttackSpeed()));

        moveSpeedText->setFont(font);
        moveSpeedText->setDefaultTextColor(redColor);
        moveSpeedText->setPlainText("移动速度: " + QString::number(playerCharacter->getMoveSpeed()));
    }
}
void gamewindow::showGameOver() {
    isGameOver = true;
    pauseGame();

    QGraphicsTextItem *gameOverText = new QGraphicsTextItem("Game Over!");
    gameOverText->setFont(QFont("Arial", 36));
    gameOverText->setPos(SCENE_WIDTH / 2 - gameOverText->boundingRect().width() / 2,
                         SCENE_HEIGHT / 2 - gameOverText->boundingRect().height() / 2);
    scene->addItem(gameOverText);

    QTimer::singleShot(3000, this, [this]() {
        emit gameEnded();
    });
}

// void gamewindow::paintEvent(QPaintEvent *event) {
//     Q_UNUSED(event);
//     QPainter painter(this);

//     // 绘制背景
//     painter.drawImage(backgroundX, backgroundY, background);
//     painter.drawImage(backgroundX + background.width(), backgroundY, background);

//     // 绘制场景中的所有项
//     scene->render(&painter);
// }

void gamewindow::keyPressEvent(QKeyEvent *event) {
    if (!playerCharacter || isGameOver) return;

    switch (event->key()) {
    case Qt::Key_W:
        playerCharacter->setIsMovingUp(true);
        break;
    case Qt::Key_S:
        playerCharacter->setIsMovingDown(true);
        break;
    case Qt::Key_A:
        playerCharacter->setIsMovingLeft(true);
        break;
    case Qt::Key_D:
        playerCharacter->setIsMovingRight(true);
        break;
    case Qt::Key_Escape:
        if (isPaused) {
            resumeGame();
        } else {
            pauseGame();
        }
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void gamewindow::keyReleaseEvent(QKeyEvent *event) {
    if (!playerCharacter) return;

    switch (event->key()) {
    case Qt::Key_W:
        playerCharacter->setIsMovingUp(false);
        break;
    case Qt::Key_S:
        playerCharacter->setIsMovingDown(false);
        break;
    case Qt::Key_A:
        playerCharacter->setIsMovingLeft(false);
        break;
    case Qt::Key_D:
        playerCharacter->setIsMovingRight(false);
        break;
    default:
        QWidget::keyReleaseEvent(event);
    }
}

// 检查玩家与僵尸的像素级碰撞（带20像素容错）
void gamewindow::checkPlayerZombieCollisions() {
    if (!playerCharacter) return;
    for (zombie* z : zombies) {
        if (z->isDead()) continue;

        // 获取玩家和僵尸的位置及尺寸
        int playerLeft = playerCharacter->x();
        int playerRight = playerCharacter->x() + playerCharacter->width();
        int playerTop = playerCharacter->y();
        int playerBottom = playerCharacter->y() + playerCharacter->height();
        int zombieLeft = z->x();
        int zombieRight = z->x() + z->width();
        int zombieTop = z->y();
        int zombieBottom = z->y() + z->height();

        // 计算重叠区域
        int overlapLeft = qMax(playerLeft, zombieLeft);
        int overlapTop = qMax(playerTop, zombieTop);
        int overlapRight = qMin(playerRight, zombieRight);
        int overlapBottom = qMin(playerBottom, zombieBottom);

        // 检查重叠区域是否满足20像素容错
        int overlapWidth = overlapRight - overlapLeft;
        int overlapHeight = overlapBottom - overlapTop;
        if (overlapWidth < 20 || overlapHeight < 20) {
            continue;
        }

        // 进行像素级碰撞检测（假设获取像素颜色的函数为getPixelColor，需根据实际图像存储方式调整）
        QImage playerImage = playerCharacter->getImage(); // 需要在player类中添加获取图像的函数
        QImage zombieImage = z->getImage(); // 需要在zombie类中添加获取图像的函数
        for (int y = overlapTop; y < overlapBottom; y++) {
            for (int x = overlapLeft; x < overlapRight; x++) {
                int playerX = x - playerLeft;
                int playerY = y - playerTop;
                int zombieX = x - zombieLeft;
                int zombieY = y - zombieTop;

                if (playerImage.pixelColor(playerX, playerY).alpha() > 0 &&
                    zombieImage.pixelColor(zombieX, zombieY).alpha() > 0) {
                    // 触发屏幕震动效果
                    shakeScreen();
                    // 处理玩家受伤逻辑
                    playerCharacter->takeDamage(z->getDamage());
                    if (playerCharacter->getHealth() <= 0) {
                        showGameOver();
                    }
                    break;
                }
            }
        }
    }
}

// 实现屏幕震动和边框变红特效
void gamewindow::shakeScreen() {
    QPoint originalPos = this->pos();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(500); // 震动持续500毫秒
    animation->setStartValue(originalPos);

    for (int i = 0; i < 10; i++) {
        int offsetX = (rand() % 20) - 10; // -10到10的随机偏移
        int offsetY = (rand() % 20) - 10;
        animation->setKeyValueAt(i / 10.0, originalPos + QPoint(offsetX, offsetY));
    }
    animation->setEndValue(originalPos);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    // 创建边框变红特效
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setColor(Qt::red);
    effect->setBlurRadius(20);
    effect->setOffset(0);
    this->setGraphicsEffect(effect);

    // 500毫秒后恢复正常边框
    QTimer::singleShot(500, [this, effect]() {
        this->setGraphicsEffect(nullptr);
        delete effect;
    });
}

