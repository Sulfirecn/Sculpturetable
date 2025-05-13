#ifndef INSPIRATIONSYSTEM_H
#define INSPIRATIONSYSTEM_H

#include <QObject>
#include <QPainter> // 添加QPainter头文件，因为draw函数需要使用QPainter

class inspirationsystem : public QObject
{
    Q_OBJECT
public:
    explicit inspirationsystem(QObject *parent = nullptr);
    void addInspiration(int value);
    void setInspiration(int value);
    int getInspiration() const;
    void increaseInspiration();
    void resetInspiration();
    void draw(QPainter *painter); // 添加draw函数声明
signals:
    void inspirationChanged(int value);
private:
    int inspirationValue;
};

#endif // INSPIRATIONSYSTEM_H
