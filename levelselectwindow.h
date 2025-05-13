#ifndef LEVELSELECTWINDOW_H
#define LEVELSELECTWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class LevelSelectWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LevelSelectWindow(QWidget *parent = nullptr);
    ~LevelSelectWindow();

signals:
    void levelAndDifficultySelected(int level, int difficulty);

private slots:
    void onLevel1EasyClicked();
    void onLevel1MediumClicked();
    void onLevel1HardClicked();

private:
    QPushButton *level1EasyButton;
    QPushButton *level1MediumButton;
    QPushButton *level1HardButton;
    QLabel *backgroundLabel;
};

#endif // LEVELSELECTWINDOW_H
