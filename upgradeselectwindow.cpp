#include "upgradeselectwindow.h"
#include <QPixmap>

UpgradeSelectionWindow::UpgradeSelectionWindow(QObject *parent) : QWidget(nullptr)
{
    setWindowTitle("Upgrade Selection");
    setFixedSize(400, 300);

    // 添加背景图片
    backgroundLabel = new QLabel(this);
    QPixmap backgroundPixmap("C:/Users/lenovo/Desktop/bighomework/v2-b247fdf4c050ac45db753c0b93db4166_r.jpg");
    backgroundLabel->setPixmap(backgroundPixmap.scaled(size(), Qt::KeepAspectRatioByExpanding));
    backgroundLabel->lower();

    option1Button = new QPushButton("Upgrade Bullet Damage", this);
    option2Button = new QPushButton("Increase Player Speed", this);
    option3Button = new QPushButton("Increase Zombie Spawn Interval", this);
    option4Button = new QPushButton("Increase Bullet Fire Rate", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(option1Button);
    layout->addWidget(option2Button);
    layout->addWidget(option3Button);
    layout->addWidget(option4Button);

    connect(option1Button, &QPushButton::clicked, this, &UpgradeSelectionWindow::onOption1Clicked);
    connect(option2Button, &QPushButton::clicked, this, &UpgradeSelectionWindow::onOption2Clicked);
    connect(option3Button, &QPushButton::clicked, this, &UpgradeSelectionWindow::onOption3Clicked);
    connect(option4Button, &QPushButton::clicked, this, &UpgradeSelectionWindow::onOption4Clicked);
}

UpgradeSelectionWindow::~UpgradeSelectionWindow()
{
    // 断开所有连接以防止悬垂指针
    disconnect(option1Button, nullptr, this, nullptr);
    disconnect(option2Button, nullptr, this, nullptr);
    disconnect(option3Button, nullptr, this, nullptr);
    disconnect(option4Button, nullptr, this, nullptr);

    delete option1Button;
    delete option2Button;
    delete option3Button;
    delete option4Button;
    delete backgroundLabel;
}

void UpgradeSelectionWindow::onOption1Clicked()
{
    emit optionSelected(1);
    this->hide();
}

void UpgradeSelectionWindow::onOption2Clicked()
{
    emit optionSelected(2);
    this->hide();
}

void UpgradeSelectionWindow::onOption3Clicked()
{
    emit optionSelected(3);
    this->hide();
}

void UpgradeSelectionWindow::onOption4Clicked()
{
    emit optionSelected(4);
    this->hide();
}
