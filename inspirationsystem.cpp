#include "inspirationsystem.h"
#include <QPainter>
#include <QFont>

inspirationsystem::inspirationsystem(QObject *parent) : QObject(parent)
{
    inspirationValue = 0;
}

void inspirationsystem::addInspiration(int value)
{
    inspirationValue += value;
    emit inspirationChanged(inspirationValue);
}

void inspirationsystem::setInspiration(int value)
{
    inspirationValue = value;
    emit inspirationChanged(inspirationValue);
}

int inspirationsystem::getInspiration() const
{
    return inspirationValue;
}

void inspirationsystem::increaseInspiration()
{
    addInspiration(10);
}

void inspirationsystem::resetInspiration()
{
    setInspiration(0);
}

void inspirationsystem::draw(QPainter *painter)
{
    QFont font = painter->font();
    font.setPointSize(12);
    painter->setFont(font);
    painter->drawText(10, 10, QString("Inspiration: %1").arg(inspirationValue));
}
