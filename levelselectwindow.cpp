#include "levelselectwindow.h"
#include <QPixmap>

LevelSelectWindow::LevelSelectWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Level Selection");
    setFixedSize(400, 300);

    // 添加背景图片
    backgroundLabel = new QLabel(this);
    QPixmap backgroundPixmap("C:/Users/lenovo/Desktop/bighomework/v2-81345c3d2017060f0993b192f40e5397_r.jpg");
    backgroundLabel->setPixmap(backgroundPixmap.scaled(size(), Qt::KeepAspectRatioByExpanding));
    backgroundLabel->lower();

    level1EasyButton = new QPushButton("Level 1 - Easy", this);
    level1MediumButton = new QPushButton("Level 1 - Medium", this);
    level1HardButton = new QPushButton("Level 1 - Hard", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(level1EasyButton);
    layout->addWidget(level1MediumButton);
    layout->addWidget(level1HardButton);

    connect(level1EasyButton, &QPushButton::clicked, this, &LevelSelectWindow::onLevel1EasyClicked);
    connect(level1MediumButton, &QPushButton::clicked, this, &LevelSelectWindow::onLevel1MediumClicked);
    connect(level1HardButton, &QPushButton::clicked, this, &LevelSelectWindow::onLevel1HardClicked);
}

LevelSelectWindow::~LevelSelectWindow()
{
    delete level1EasyButton;
    delete level1MediumButton;
    delete level1HardButton;
    delete backgroundLabel;
}

void LevelSelectWindow::onLevel1EasyClicked()
{
    emit levelAndDifficultySelected(1, 1);
    this->hide();
}

void LevelSelectWindow::onLevel1MediumClicked()
{
    emit levelAndDifficultySelected(1, 2);
    this->hide();
}

void LevelSelectWindow::onLevel1HardClicked()
{
    emit levelAndDifficultySelected(1, 3);
    this->hide();
}
