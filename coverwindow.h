#ifndef COVERWINDOW_H
#define COVERWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QAudioOutput>

class coverwindow : public QWidget
{
    Q_OBJECT
public:
    explicit coverwindow(QWidget *parent = nullptr);
    ~coverwindow();

public:
    QMediaPlayer *coverBGMPlayer;

signals:
    void difficultySelected(int level, int difficulty);

private slots:
    void onEasyClicked();
    void onMediumClicked();
    void onHardClicked();
    void updateBackgroundImage();
    void handleEscapePress(QKeyEvent* event);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QPushButton *easyButton;
    QPushButton *mediumButton;
    QPushButton *hardButton;
    QAudioOutput *audioOutput;
};

#endif // COVERWINDOW_H
