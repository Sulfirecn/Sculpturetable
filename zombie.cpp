#include "zombie.h"
#include <QPainter>
#include <QDebug>
#include <cmath>
const int SPAWN_MARK_OFFSET_Y = 20;
zombie::zombie(ZombieType type, int difficulty, QObject *parent)
    : QObject(parent), movie(nullptr), posX(0), posY(0), health(0), spawnMarkDisplayTime(0),
    spawnMarkTimer(nullptr), spawnMarkImage(), damage(0), moveSpeed(0), spawnMarkX(0), spawnMarkY(0)
{
    switch (type) {
    case BucketheadZombie:
        movie = new QMovie("C:/Users/lenovo/Desktop/bighomework/res/Zombies/BucketheadZombie/BucketheadZombie.gif");
        baseHealth = 300;
        baseDamage = 20;
        baseMoveSpeed = 1;
        break;
    case ConeheadZombie:
        movie = new QMovie("C:/Users/lenovo/Desktop/bighomework/res/Zombies/ConeheadZombie/ConeheadZombie.gif");
        baseHealth = 200;
        baseDamage = 15;
        baseMoveSpeed = 2;
        break;
    case NormalZombie:
        movie = new QMovie("C:/Users/lenovo/Desktop/bighomework/res/Zombies/Zombie/Zombie.gif");
        baseHealth = 100;
        baseDamage = 10;
        baseMoveSpeed = 3;
        break;
    default:
        movie = new QMovie("C:/Users/lenovo/Desktop/bighomework/res/Zombies/Zombie/Zombie.gif");
        baseHealth = 100;
        baseDamage = 10;
        baseMoveSpeed = 3;
        break;
    }
    // 根据难度调整初始属性
    health = baseHealth * (1 + 0.2 * difficulty);
    damage = baseDamage * (1 + 0.2 * difficulty);
    moveSpeed = baseMoveSpeed * (1 + 0.1 * difficulty);
    if (!movie->isValid()) {
        qDebug() << "Failed to load zombie gif!";
    }
    movie->start();
    int newWidth = static_cast<int>(movie->currentImage().width() * 0.8);
    int newHeight = static_cast<int>(movie->currentImage().height() * 0.8);
    movie->setScaledSize(QSize(newWidth, newHeight));
    // 初始化生成标记
    spawnMarkImage = QImage("C:/Users/lenovo/Desktop/bighomework/0f796de68a5e02bee4418d3a83f83f7.png");
    if (!spawnMarkImage.isNull()) {
        // 获取图片中白色部分的实际颜色值
        QRgb whitePixelColor = spawnMarkImage.pixel(0, 0);
        QColor exactWhiteColor(whitePixelColor);
        QImage mask = spawnMarkImage.createMaskFromColor(exactWhiteColor.rgb(), Qt::MaskOutColor);
        spawnMarkImage = spawnMarkImage.convertToFormat(QImage::Format_ARGB32);
        spawnMarkImage.setAlphaChannel(mask);
        if (spawnMarkImage.hasAlphaChannel()) {
            qDebug() << "Spawn mark image has valid alpha channel.";
        } else {
            qDebug() << "Failed to set alpha channel properly for spawn mark image.";
        }
    } else {
        qDebug() << "Failed to load spawn mark image!";
    }
    spawnMarkTimer = new QTimer(this);
    connect(spawnMarkTimer, &QTimer::timeout, this, &zombie::hideSpawnMark);
}
zombie::~zombie()
{
    // 停止定时器避免访问已删除对象
    if (spawnMarkTimer) {
        spawnMarkTimer->stop();
        delete spawnMarkTimer;
        spawnMarkTimer = nullptr;
    }
    delete movie;
}
void zombie::setPos(int x, int y)
{
    posX = x;
    posY = y;
}
int zombie::x() const { return posX; }
int zombie::y() const { return posY; }
int zombie::width() const { return movie->currentImage().width(); }
int zombie::height() const { return movie->currentImage().height(); }
void zombie::draw(QPainter *painter)
{
    // 绘制生成标记（若处于显示状态）
    if (spawnMarkDisplayTime > 0 &&!spawnMarkImage.isNull()) {
        // 计算标记位置：居中对齐
        spawnMarkX = posX + (width() - spawnMarkImage.width()) / 2;
        spawnMarkY = posY - SPAWN_MARK_OFFSET_Y; // 向上偏移
        painter->drawImage(spawnMarkX, spawnMarkY, spawnMarkImage);
    }
    // 绘制僵尸本体
    if (movie &&!movie->currentImage().isNull()) {
        painter->drawImage(posX, posY, movie->currentImage());
    }
}
void zombie::move(player *p)
{
    if (p &&!isDead()) { // 修复：添加isDead()检查
        int dx = p->x() - posX;
        int dy = p->y() - posY;
        double distance = std::sqrt(dx * dx + dy * dy);
        if (distance > 0) {
            // 限制最大移动步长，避免速度过快
            int step = qMin(moveSpeed, static_cast<int>(distance));
            posX
                += dx * step / distance;
            posY
                += dy * step / distance;

            // 边界处理，避免僵尸卡在边缘
            if (posX < 0) posX = 0;
            if (posX > SCENE_WIDTH - width()) posX = SCENE_WIDTH - width();
            if (posY < 0) posY = 0;
            if (posY > SCENE_HEIGHT - height()) posY = SCENE_HEIGHT - height();
        }
    }
}
bool zombie::isOutOfBounds() const
{
    return posX < -width() || posX > SCENE_WIDTH || posY < -height() || posY > SCENE_HEIGHT;
}
void zombie::takeDamage(int damage)
{
    health = qMax(0, health - damage);
    if (isDead()) {
        emit removeRequested(this); // 发出移除请求
    }
}
bool zombie::isDead() const { return health <= 0; }
void zombie::showSpawnMark()
{
    spawnMarkDisplayTime = 1000; // 显示 1 秒（1000ms）
    if (spawnMarkTimer) {
        spawnMarkTimer->start(spawnMarkDisplayTime); // 启动定时器自动隐藏
    }
}
// 确保hideSpawnMark函数只有一个定义
void zombie::hideSpawnMark()
{
    spawnMarkDisplayTime = 0; // 隐藏标记
    if (spawnMarkTimer) {
        spawnMarkTimer->stop();
    }
}
void zombie::strengthen(int level, int elapsedTime)
{
    // 随等级和时间增强僵尸
    health = baseHealth * (1 + 0.2 * (level + elapsedTime / 60));
    damage = baseDamage * (1 + 0.2 * (level + elapsedTime / 60));
    moveSpeed = baseMoveSpeed * (1 + 0.1 * (level + elapsedTime / 60));
}
int zombie::getDamage() const
{
    return damage;
}
QImage zombie::getImage() const
{
    return movie->currentImage();
}
