#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game.h"
#include "helpfield.h"
#include <QMainWindow>
#include <QGridLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    GameField *m_gameField; //Добавления виджета в основое окно приложения
    HelpField *m_helpField; //Добавление в основное приложение
    QWidget *m_centralWidget;
    QGridLayout *m_gridLayout; //Необходимо для создания сетки
};
#endif // MAINWINDOW_H
