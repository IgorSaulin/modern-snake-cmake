#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(620, 670); //Размер виджета - Самого окна.  первоначальное значение setFixedSize(320, 370)
    m_gameField = new GameField(); //Инициализация поля в конструкторе
    m_helpField = new HelpField(); //Инициализация поля в конструкторе
    connect(m_gameField, &GameField::ChangeTextSignal, m_helpField, &HelpField::ChangeTextSlot); //Сигнально-слотовое соединение
    m_centralWidget = new QWidget();
    m_gridLayout = new QGridLayout(); //Инициализация
    m_gridLayout -> addWidget(m_gameField, 0, 0); //Добавление кастомных виджетов
    m_gridLayout -> addWidget(m_helpField, 1, 0);
    m_centralWidget -> setLayout(m_gridLayout);
    setCentralWidget(m_centralWidget); //Добавление виджета в окно
    setStyleSheet("background: rgb(90, 100, 112);"); //Изменение цвета игрового поля
}

MainWindow::~MainWindow() {}
