#include "helpfield.h"

HelpField::HelpField() //Изменена одна из букв
{
    setFixedSize(600, 50); //Расположение надписи - счетчика и информации о текущем счете игры
    m_text = "Счет : 0 \n Пауза - ПРОБЕЛ";
}

void HelpField::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter; //Подключение модулей для рисования границ
    painter.begin(this);
    painter.setPen(QColor(250, 250, 250));
    //painter.drawRect(0, 0, width()-1, height()-1); //Рисование прямоугольника
    painter.setFont(QFont("Arial", 10, 700)); //Установка типа шрифта, высоты, жирность(табличное значение)
    painter.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, m_text); //Добавление текста
    painter.end();
}

void HelpField::ChangeTextSlot(QString text)
{
    m_text = text; //Изменение текста на экране
    repaint(); //Переписывание текста
}
