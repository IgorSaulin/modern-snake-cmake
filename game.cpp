#include "game.h"

double angle = 0;
double counter_x = 0;
double counter_y = 0;

double DegToRad(double D) //Функция для перевода градусов в радианы
{
    double M=3.14/180;
    return D*M;
};

GameField::GameField()
{
    setFixedSize(600, 600); //Размер самого игрового поля. Первоначальное значение - setFixedSize(300, 300)
    setFocusPolicy(Qt::StrongFocus); //Активация управления стрелками
    m_snakeItemSize = 10; //Размер одного звена змейки. Первоначальное значение - m_snakeItemSize = 10
    m_fieldSize = width()/m_snakeItemSize;
    StartNewGame(); //Запуск новой игры
}

void GameField::paintEvent(QPaintEvent *e) //Отрисовка самой змейки
{
    Q_UNUSED(e);
    QBrush gameFieldBrush (QColor(139, 144, 163), Qt::SolidPattern); //Цвет для заливки игрового поля
    QBrush snakeBrush (QColor(140, 206, 147), Qt::SolidPattern); //Цвет для заливки змейки
    QBrush foodBrush (QColor(247, 163, 123), Qt::SolidPattern); //Цвет для заливки еды
    QPainter painter; //Подключение модулей для рисования границ
    painter.begin(this);
    if(m_isGameOver)
    {
        painter.setPen(QColor(250, 250, 250)); //Изменение цвета для надписи "ПОРАЖЕНИЯ"
        painter.setFont(QFont("Arial", 15, 700)); //Установка типа шрифта, высоты, жирность(табличное значение)
        painter.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, "ИГРА ПРОИГРАНА \n счет :" + QString::number(m_score)); //Добавление текста
        return;
    }
    //Отрисовка игрового поля
    painter.setBrush(gameFieldBrush); //Кисть игрового поля
    painter.setPen(QColor(50, 50, 50));//Высветление контура самого квадрата
    painter.drawRect(0, 0, width()-1, height()-1); //Рисование прямоугольника
    //Отрисовка змейки
    painter.setBrush(snakeBrush); //Перед отрисовкой змейки (кисть для змейки)
    painter.setPen(QColor(0, 0, 0));//контуру возвращается черый цвет
    for(int i = 0; i < m_snake->m_snakeBody.size(); i++)
    {
        painter.drawEllipse(m_snake->m_snakeBody[i] -> m_x * m_snakeItemSize, m_snake->m_snakeBody[i] -> m_y * m_snakeItemSize, m_snakeItemSize, m_snakeItemSize);
        //painter.drawText(m_snake->m_snakeBody[0] -> m_x * m_snakeItemSize, m_snake->m_snakeBody[0] -> m_y * m_snakeItemSize, "0");
        //painter.drawText(m_snake->m_snakeBody[1] -> m_x * m_snakeItemSize, m_snake->m_snakeBody[1] -> m_y * m_snakeItemSize, "1");
        //painter.drawText(m_snake->m_snakeBody[2] -> m_x * m_snakeItemSize, m_snake->m_snakeBody[2] -> m_y * m_snakeItemSize, "2");
    }
    //Отрисовка еды
    painter.setBrush(foodBrush); //Кисть для отрисовки еды
    painter.drawEllipse(m_food->m_x * m_snakeItemSize, m_food->m_y * m_snakeItemSize, m_snakeItemSize, m_snakeItemSize);

    painter.end();
    m_isMoveBlocked = false; //Блокировка снимается после каждой смены кадра
}

void GameField::keyPressEvent(QKeyEvent *e)
{
    //Реализация управления клавишами
    if(e->key() == Qt::Key_Space)
    {
        if(m_isGameOver)
        {
            StartNewGame();
            return;
        }
        m_isPouse = !m_isPouse;
        SetGameStatus();
    }
    if(m_isMoveBlocked) //Выход из функции при попытки быстрой смены клавиш
    {
        return;
    }
    if(e->key() == Qt::Key_Left && m_snake->m_snakeDirection != Snake::SnakeDirection::right)
    {
        m_snake->m_snakeDirection = Snake::SnakeDirection::left;
    }
    if(e->key() == Qt::Key_Right && m_snake->m_snakeDirection != Snake::SnakeDirection::left)
    {
        m_snake->m_snakeDirection = Snake::SnakeDirection::right;

        angle = angle + 10; //Увеличение угла на градус
        if (angle == 360)
        {
            angle = 0;
        }
    }
    if(e->key() == Qt::Key_Down && m_snake->m_snakeDirection != Snake::SnakeDirection::up)
    {
        m_snake->m_snakeDirection = Snake::SnakeDirection::down;
    }
    if(e->key() == Qt::Key_Up && m_snake->m_snakeDirection != Snake::SnakeDirection::down)
    {
        m_snake->m_snakeDirection = Snake::SnakeDirection::up;
    }
    m_isMoveBlocked = true;
}

void GameField::SetGameStatus()
{
    QString text;
    if(m_isPouse)
    {
        m_moveSnakeTimer->stop(); //Остановка игры, при нажатии пробела
        text = "Счет : " + QString::number(m_score) + "\n продолжить - ПРОБЕЛ";
    }
    else
    {
        m_moveSnakeTimer->start(100); //Таймер, отвечает за время обновления картинки после нажатия "ПАУЗА"
        text = "Счет : " + QString::number(m_score) + "\n пауза - ПРОБЕЛ";
    }

    emit ChangeTextSignal(text);
}

void GameField::GameOver()
{
    m_isGameOver = true;
    QString text = "начать заново - ПРОБЕЛ";
    emit ChangeTextSignal(text);
    m_moveSnakeTimer->stop();
    //delete m_snake; /*При активации программа вылетает!!!*/
    delete m_moveSnakeTimer;
    delete m_food;
}

void GameField::StartNewGame()
{
    m_isPouse = false; //Логическая переменная, отвечает за остановку игры
    m_isMoveBlocked = false; //Блокирует поворот змейки на 180 градусов при быстром нажатии клавиш
    m_isGameOver = false;
    m_snake = new Snake();
    m_food = new SnakeItem(m_fieldSize/2, m_fieldSize/2); //Еда всегда инициализируется в центре игрового поля
    m_moveSnakeTimer = new QTimer(); //Установка таймера
    connect(m_moveSnakeTimer, &QTimer::timeout, this, &GameField::MoveSnakeSlot); //Связывание таймера
    m_moveSnakeTimer->start(100); //Таймер, отвечает за время обновления картинки
    m_score = 0;
    QString text = "Счет : " + QString::number(m_score) + "\n пауза - ПРОБЕЛ";
    emit ChangeTextSignal(text);
}

void GameField::CreateFood()
{
    m_food->m_x = QRandomGenerator::global()->bounded(0, m_fieldSize - 1);
    m_food->m_y = QRandomGenerator::global()->bounded(0, m_fieldSize - 1);
    //Проверка не совпадают ли координаты фрукта с каким-нибудь элементом змейки
    for(int i = 0; i < m_snake->m_snakeBody.size(); i++)
    {
        if(m_food->m_x == m_snake->m_snakeBody[i]->m_x && m_food->m_y == m_snake->m_snakeBody[i]->m_y)
        {
            return CreateFood();
        }
    }
}

void GameField::MoveSnakeSlot()
{
    SnakeItem *newSnakeItem; //Голова змейки
    if(m_snake->m_snakeDirection == Snake::SnakeDirection::right)
    {
        int Buffer_x = 0;
        counter_x = counter_x + qCos(DegToRad(angle));
        if ((counter_x > 1)&&(counter_x < 2))
        {
            qInfo() << "NEW x";
            Buffer_x = 1; // qRound(counter_x);
            counter_x = 0;
        }
        qInfo() << "Counter x = " << counter_x;
        qInfo() << "Buffer x = " << Buffer_x;

        int Buffer_y = 0;
        counter_y = counter_y + qSin(DegToRad(angle));
        if ((counter_y > 1)&&(counter_y < 2))
        {
            qInfo() << "NEW y";
            Buffer_y = 1;// qRound(counter_y);
            counter_y = 0;
        }
        qInfo() << "Counter y = " << counter_y;
        qInfo() << "Buffer y = " << Buffer_y;

        newSnakeItem = new SnakeItem(m_snake->m_snakeBody[0]->m_x + 1, m_snake->m_snakeBody[0]->m_y);

    }

    else if(m_snake->m_snakeDirection == Snake::SnakeDirection::left)
    {
        newSnakeItem = new SnakeItem(m_snake->m_snakeBody[0]->m_x - 1, m_snake->m_snakeBody[0]->m_y);
        qInfo() << "Left";
    }

    else if(m_snake->m_snakeDirection == Snake::SnakeDirection::up)
    {
        newSnakeItem = new SnakeItem(m_snake->m_snakeBody[0]->m_x, m_snake->m_snakeBody[0]->m_y - 1);
        qInfo() << "Up";
    }

    else
    {
        newSnakeItem = new SnakeItem(m_snake->m_snakeBody[0]->m_x, m_snake->m_snakeBody[0]->m_y + 1);
        qInfo() << "Down";
    }
    //Ограничение игрового поля
    /*Первоначально предполагалось, что змейка сможет перемещаться за пределы игрового поля:
     т.е. голова змейки выходит за пределы игрового поля и возникает на противоположенной стороне
     ниже данные строки помечены как комментарии, чтобы, при необходимости, можно было вернуться
     к первоначальному варианту*/

    if(newSnakeItem->m_x >= m_fieldSize)
    {
        //newSnakeItem->m_x = 0;
        GameOver();
    }
    else if(newSnakeItem->m_x < 0)
    {
        //newSnakeItem->m_x = m_fieldSize - 1;
        GameOver();
    }
    else if(newSnakeItem->m_y < 0)
    {
        //newSnakeItem->m_y = m_fieldSize - 1;
        GameOver();
    }
    else if(newSnakeItem->m_y >= m_fieldSize)
    {
        //newSnakeItem->m_y = 0;
        GameOver();
    }
    //Описание случая, когда игра проиграна
    for(int i = 1; i < m_snake->m_snakeBody.size(); i++)
    {
        //Если голова змеи пересекается со своим телом, то игра прекращается
        if(m_snake->m_snakeBody[0]->m_x == m_snake->m_snakeBody[i]->m_x && m_snake->m_snakeBody[0]->m_y == m_snake->m_snakeBody[i]->m_y)
        {
            GameOver();
        }
    }
    //Получение еды
    if(newSnakeItem->m_x == m_food->m_x && newSnakeItem->m_y == m_food->m_y)
    {
        m_score++;
        CreateFood();
        QString text = "Счет : " + QString::number(m_score) + "\n пауза - ПРОБЕЛ";
        emit ChangeTextSignal(text);
    }
    else
    {
        m_snake->m_snakeBody.removeLast(); //Удаление последнего элемента змейки
    }

    m_snake->m_snakeBody.insert(0, newSnakeItem); //Добавление змейки в начало
    repaint(); //Вызов таймера через функцию
}

SnakeItem::SnakeItem(int x, int y)
{
    m_x = x;
    m_y = y;
}

Snake::Snake() //Класс отвечает за создание математической модели туловища змейки
{
    m_snakeBeginSize = 3; //Начальный размер змейки (3)
    for (int i =0; i < m_snakeBeginSize; i++) //Заполнение тела змейки
    {
        m_snakeBody.insert(0, new SnakeItem(i, 0));
    }
    m_snakeDirection = SnakeDirection::right;
}
