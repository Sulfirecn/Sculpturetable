#ifndef UPGRADESELECTIONWINDOW_H
#define UPGRADESELECTIONWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class UpgradeSelectionWindow : public QWidget
{
    Q_OBJECT
public:
    explicit UpgradeSelectionWindow(QObject *parent = nullptr);
    ~UpgradeSelectionWindow();

signals:
    void optionSelected(int option);

private slots:
    void onOption1Clicked();
    void onOption2Clicked();
    void onOption3Clicked();
    void onOption4Clicked();

private:
    QPushButton *option1Button;
    QPushButton *option2Button;
    QPushButton *option3Button;
    QPushButton *option4Button;
    QLabel *backgroundLabel;
};

#endif // UPGRADESELECTIONWINDOW_H
