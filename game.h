#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QPainter>
#include <QList>
#include <QTimer>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QDebug> //Для вывода информации в консоль
#include <QtMath> //Для работы с sin() и cos()

double DegToRad(double D); //Конструктор Функция для перевода градусов в радианы

class SnakeItem; //Элементы змейки
class Snake; //Змейка
class GameField; //Игровое поле

class SnakeItem
{
    friend Snake;
    friend GameField;

public:
    SnakeItem(int x, int y);

private:
    int m_x;
    int m_y;
};

class Snake
{
    friend GameField;
public:
    Snake();

private:
    QList <SnakeItem*> m_snakeBody; //Тело змейки
    int m_snakeBeginSize; //Первоначальная длина змейки
    enum SnakeDirection //Создание направлений
    {
        up,
        left,
        down,
        right
    };
    SnakeDirection m_snakeDirection; //Поле, которое хранит направления

};

class GameField : public QWidget //Наследует от другого класса
{
    Q_OBJECT //Необходимо для использования сигналов и слотов
public: //Создание конструктора класса
    GameField();

protected:
    void paintEvent(QPaintEvent *e) override; //Виртуальная функция для рисования границ поля
    void keyPressEvent(QKeyEvent *e) override;

private:
    Snake *m_snake;
    SnakeItem *m_food; //Еда для змейки
    int m_snakeItemSize;
    QTimer *m_moveSnakeTimer;
    int m_fieldSize;
    int m_score;
    bool m_isPouse;
    bool m_isMoveBlocked;
    bool m_isGameOver;
    void SetGameStatus(); //Функция, которая ставит игру на паузу
    void GameOver(); //Функция проигрыш
    void StartNewGame(); //Функция начала новой игры
    void CreateFood(); //Класс для создания еды

private slots: //Создание слотов
    void MoveSnakeSlot();

signals:
    void ChangeTextSignal(QString text);
};

#endif // GAME_H
