#ifndef GAMEUNIT_H
#define GAMEUNIT_H

#include <QWidget>
#include <QPaintEvent>
#include <QVector>
#include <QKeyEvent>
#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QTime>

using ull = unsigned long long;

//Константа для отрисовки тектсур.
static const int PONUNIT = 64;

class chess : public QWidget
{
    Q_OBJECT
public:
    explicit chess(QWidget *parent = nullptr);

    // Массив поля.
    // Вен харнятся черно белые поля.
    QVector<QVector<int>> textureMap = {
        {0,1,0,1,0,1,0,1},
        {1,0,1,0,1,0,1,0},
        {0,1,0,1,0,1,0,1},
        {1,0,1,0,1,0,1,0},
        {0,1,0,1,0,1,0,1},
        {1,0,1,0,1,0,1,0},
        {0,1,0,1,0,1,0,1},
        {1,0,1,0,1,0,1,0}
    };

    //Массив выбора.
    //Куда можно пойти.
    QVector<QVector<int>> textureCheck = {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0}
    };

    // Массив шашек.
    // Где соят шашки.
    QVector<QVector<int>> textureChess = {
        {0,2,0,2,0,2,0,2},
        {2,0,2,0,2,0,2,0},
        {0,2,0,2,0,2,0,2},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {1,0,1,0,1,0,1,0},
        {0,1,0,1,0,1,0,1},
        {1,0,1,0,1,0,1,0}
    };

private:

    // Координаты точек курсора.
    int xPos = 0, yPos = 0;

    // Режим игры либо выбор хода либо выбор шашки.
    bool checkMod = false;

    // Переменная для функции goWhite/black
    int goToX,goToY;

    // Переменная очереди хода.
    bool tirn = true;

    // Счетчик убитых.
    int whiteWin = 0, blackWin = 0;

    // Useless
    bool countGo = true;

    //Текстуры.
    QImage blackB;
    QImage whiteB;
    QImage blackP;
    QImage whiteP;
    QImage point;
    QImage yellow;

    //Функция проверки на количестов ходов.
    void whatMod();

    void keyPressEvent(QKeyEvent *event);

    // Загрузка текстур.
    void loadTextures();

    // Отрисовка текстур.
    void paintEvent(QPaintEvent *pEvent);

    // Отрисовка поля.
    void doDrawMap();

    // Отрисовка поля.
    void doDrawChess();

    // Отрисовка поля выбора.
    void doDrawCheck();

    // Отрисовка поля.
    void doDrawPointer();

    void goBlack();

    void goWhite();

    void goCheck();

    void endGo();

    void timerEvent(QTimerEvent *e);
};

#endif // GAMEUNIT_H
