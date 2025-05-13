#include "coverwindow.h"
#include <QPalette>
#include <QPixmap>
#include <QVBoxLayout>
#include <QDebug>
#include <QKeyEvent>

coverwindow::coverwindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Game Cover");
    setFixedSize(1400, 800);

    updateBackgroundImage();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);
    mainLayout->addStretch();

    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->setSpacing(10);

    easyButton = new QPushButton("Easy", this);
    mediumButton = new QPushButton("Medium", this);
    hardButton = new QPushButton("Hard", this);

    QString buttonStyle = "QPushButton {"
                          "    background-color: rgba(76, 175, 80, 0.8);"
                          "    border: none;"
                          "    color: white;"
                          "    padding: 15px 30px;"
                          "    text-align: center;"
                          "    text-decoration: none;"
                          "    display: inline-block;"
                          "    font-size: 24px;"
                          "    margin: 8px 4px;"
                          "    border-radius: 12px;"
                          "}"
                          "QPushButton:hover {"
                          "    background-color: rgba(69, 160, 73, 0.9);"
                          "    transform: scale(1.05);"
                          "    box-shadow: 0 0 20px rgba(0, 0, 0, 0.3);"
                          "}";

    easyButton->setStyleSheet(buttonStyle);
    mediumButton->setStyleSheet(buttonStyle);
    hardButton->setStyleSheet(buttonStyle);

    buttonLayout->addWidget(easyButton);
    buttonLayout->addWidget(mediumButton);
    buttonLayout->addWidget(hardButton);

    mainLayout->addLayout(buttonLayout);

    coverBGMPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    coverBGMPlayer->setAudioOutput(audioOutput);
    coverBGMPlayer->setSource(QUrl::fromLocalFile("C:/Users/lenovo/Desktop/linggangu.mp3.MP3"));
    if (coverBGMPlayer->error() != QMediaPlayer::NoError) {
        qDebug() << "Failed to load cover BGM: " << coverBGMPlayer->errorString();
    }
    coverBGMPlayer->play();

    connect(easyButton, &QPushButton::clicked, this, &coverwindow::onEasyClicked);
    connect(mediumButton, &QPushButton::clicked, this, &coverwindow::onMediumClicked);
    connect(hardButton, &QPushButton::clicked, this, &coverwindow::onHardClicked);

    connect(this, &coverwindow::keyPressEvent, this, &coverwindow::handleEscapePress);
}

coverwindow::~coverwindow()
{
    // Qt会自动管理子控件的内存，不需要手动delete
}

void coverwindow::updateBackgroundImage()
{
    QPalette palette = this->palette();
    QPixmap backgroundImage("C:/Users/lenovo/Desktop/bighomework/v2-b247fdf4c050ac45db753c0b93db4166_r.jpg");

    if (!backgroundImage.isNull()) {
        palette.setBrush(QPalette::Window, QBrush(backgroundImage.scaled(
                                               1400, 800, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation)));
        this->setPalette(palette);
        this->setAutoFillBackground(true);
    } else {
        qDebug() << "Failed to load background image!";
        this->setStyleSheet("background-color: #333333;");
    }
}

void coverwindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateBackgroundImage();
}

void coverwindow::onEasyClicked()
{
    emit difficultySelected(1, 1);
    coverBGMPlayer->stop();
    this->hide();
}

void coverwindow::onMediumClicked()
{
    emit difficultySelected(1, 2);
    coverBGMPlayer->stop();
    this->hide();
}

void coverwindow::onHardClicked()
{
    emit difficultySelected(1, 3);
    coverBGMPlayer->stop();
    this->hide();
}

void coverwindow::handleEscapePress(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape) {
        coverBGMPlayer->stop();
        close();
    }
}
