#include "chess.h"



/*
 * Стандартный конструктор.
 * Закинул в него параметры что бы не могли изменить экран.
 * Цвет занего фона.
 * Ну и таймер
 *
 */
chess::chess(QWidget *parent) : QWidget(parent) {

    setStyleSheet("background-color:black;");
    setMinimumSize(512,512);
    setMaximumSize(512,512);

    loadTextures();

    startTimer(17);
}


/*
 * keyPress - функция для отслеживания сигналов с клавиатуры.
 * Space - выбрать
 * Alt - отменить
 * Escape - выход
 */
void chess::keyPressEvent(QKeyEvent *event) {


    /* Проверка на Пробел.
     * Есть два варианта ситуации
     * 1) когда не была выбрана шашка
     *  тогда мы просто запускаем функцию проверки на возможные ходы
     * 2) Когда шашка уже выбрана и выбирается ход
     *  тогда мы выбираем точку обнуляем точку где были, если сьели шашки то удаляем её
     *  и прибавляем очко к счетчику.
     */
    if(event->key() == Qt::Key_Space) {

        // Проверка чей ход.
        if (tirn == true) {

            // Проверка что фикша белых и сейчас мод не выбора.
            if(textureChess[yPos][xPos] == 1 && checkMod == false) {

                //Функция хода белых.....шашек
                goWhite();

                //Проверка на невозможность хода.
                whatMod();

            // Проверка что мод выбора, что мы выбрали место куда можно сходить или место сьедения.
            } else if(checkMod == true && ((textureCheck[yPos][xPos] == 2) || (textureCheck[yPos][xPos] == 3))) {

                // Если у нас месть сьедения, то мы обнуляем то место и увеличиваем соо- ий счетчик.
                if (textureCheck[yPos][xPos] == 3) {
                    textureChess[goToY + ((yPos - goToY) / 2)][goToX + ((xPos - goToX) / 2)] = 0;
                    ++whiteWin;
                }
                    // Перемещение в выбраную точку.
                    goCheck();

                    //Проверка что ходов больше нет.
                    whatMod();
            }
        }

        // Если ход черных.
        else {
            // Проверка что выбрана шашка черных и мод невыобра.
            if(textureChess[yPos][xPos] == 2 && checkMod == false) {

                //Функция хода черных.
                goBlack();

                //Проверка на возможность ходьбы.
                whatMod();

                //Аналогичкая ситуация с белыми.
            } else if(checkMod == true && ((textureCheck[yPos][xPos] == 2) || (textureCheck[yPos][xPos] == 3))) {
                if (textureCheck[yPos][xPos] == 3) {
                    textureChess[goToY + ((yPos - goToY) / 2)][goToX + ((xPos - goToX) / 2)] = 0;
                    ++blackWin;
                }
                goCheck();

                whatMod();
            }
        }
    }

    //Проверка того,что нажата клавиша Alt
    if (event->key() == Qt::Key_Alt) {

        //Функция отмены выбора шашки.
        endGo();
    }

    //Проверка того,что нажата клавиша Escape
    if (event->key() == Qt::Key_Escape) {

        //Систмная функция выхода.
        close();
    }

    //Проверка того,что нажата клавиша Up
    if (event->key() == Qt::Key_Up) {

        //Фонрмула перемещения модуль и абсолют чтобы не выходило за рамки массива.
        yPos = abs((yPos - 1) % 8);
    }

    //Проверка того,что нажата клавиша Left
    if (event->key() == Qt::Key_Left) {
        xPos = (abs(xPos - 1)) % 8;
    }

    //Проверка того,что нажата клавиша Right
    if (event->key() == Qt::Key_Right) {
        xPos = (abs(xPos + 1)) % 8;
    }

    // Обработка выхода из приложения.
    if (event->key() == Qt::Key_Down) {
        yPos = (abs(yPos + 1)) % 8;
    }
}


/*
 * Функция отрисовки всего
 * Работает так что отрисовывает все элементы
 * И функции отрисовки могут быть только в ней.
 */
void chess::paintEvent(QPaintEvent *pEvent) {

    // Если не используется pEvent - игнорировать.
    Q_UNUSED(pEvent);

    //рисуем карту
    doDrawMap();

    //Рисуем Шашки
    doDrawChess();

    //Если мод выбора, рисуем места хода.
    if(checkMod == 1) {
        doDrawCheck();
    }

    //Рисуем указатель.
    doDrawPointer();
}


/* Функция риcования карты.
 * Нужна для прорисовки черно-белого полотна
 * Пробегает массив данный в .h
 * И на его основе рисует карту.
 */
void chess::doDrawMap() {

    // Указатель для рисованиея.
    // Типо полотно на которым мы рисуем.
    QPainter PaintMap(this);

    //Индексы обхода.
    int indexX, indexY;

    //Сам обход.
    for (indexY = 0; indexY < 8;++indexY) {
        for (indexX = 0; indexX < 8;++indexX) {
            //Если мы нашли 1 то это будет черный квадрат.
            if (textureMap[indexY][indexX] == 1) {
                //drawImage - рисует изображение в x и y координате, и выбор изображения.
                PaintMap.drawImage(indexX * 64,indexY * 64,blackB);
            //Иначе белый.
            } else {

                PaintMap.drawImage(indexX * 64,indexY * 64,whiteB);
            }
        }
    }
}



//Отрисовка шашек.
void chess::doDrawCheck() {

    QPainter PaintMap(this);

    int indexX, indexY;

    for (indexY = 0; indexY < 8;++indexY) {
        for (indexX = 0; indexX < 8;++indexX) {
            if ((textureCheck[indexY][indexX] == 2) || (textureCheck[indexY][indexX] == 3)) {
                PaintMap.drawImage(indexX * 64,indexY * 64,yellow);
            }
        }
    }
}

//Аналогично с шашками.
void chess::doDrawChess() {

    QPainter PaintMap(this);

    int indexX, indexY;

    //Так же как и в предидущем если нашли индек определенной шашки то рисуем её.
    for (indexY = 0; indexY < 8;++indexY) {
        for (indexX = 0; indexX < 8;++indexX) {
            if (textureChess[indexY][indexX] == 1) {
                PaintMap.drawImage(indexX * 64,indexY * 64,whiteP);
            } else if(textureChess[indexY][indexX] == 2) {
                PaintMap.drawImage(indexX * 64,indexY * 64,blackP);
            }
        }
    }
}


//Функция загрузки текстур из Ресурсов приложения.
void chess::loadTextures() {

    blackB.load(":/TextFolder/black_block");
    blackP.load(":/TextFolder/black_point");
    whiteB.load(":/TextFolder/white_block");
    whiteP.load(":/TextFolder/white_point");
    point.load(":/TextFolder/point");
    yellow.load(":/TextFolder/yellow_block");

}

//Рисеум указатель по координатм его.
void chess::doDrawPointer() {

    QPainter PaintMap(this);

    PaintMap.drawImage(xPos * 64,yPos * 64,point);

}

// Тайм ивент - для сапуска его нужно установить ранее
// У меня почти в самом начале.
void chess::timerEvent(QTimerEvent *e) {
    Q_UNUSED(e);

    //Каждый ход проверяет не выигла ла ли какая нибудь команда.
    if(blackWin == 12 || whiteWin == 12) {
        close();
    }
    // И перерисовывает весь экран.
    repaint();
}


// Функция отмены отрисовки полей хода и режима выобра хода.
void chess::endGo() {

    checkMod = false;

    int index, indey;
    //Обнуляем массив хода.
    for (indey = 0; indey < 8; ++indey) {
        for (index = 0; index < 8; ++index) {
            textureCheck[indey][index] = 0;
        }
    }
}

//Функция хода белыъ
void chess::goWhite() {

    checkMod = true;

    //Запомнили где была шашка.
    goToX = xPos;
    goToY = yPos;

    /*Сложно написанное условие но по сути
     * Мы проверяем может ли шашка сходить наискосок и если может то если
     * там враг может ли она его сьесть.
     * Для белых свой зод вверх, для черных свой, в зависимости от их статуса.
     */
    if ((goToY - 1) >= 0 && (goToX - 1) >= 0) {

        if ((textureChess[goToY - 1][goToX - 1] == 2) && (((goToY - 2) >= 0 && (goToX - 2) >= 0))) {
            if (textureChess[goToY - 2][goToX - 2] == 0) {
                textureCheck[goToY - 2][goToX - 2] = 3;
            }
        } else if ((textureChess[goToY - 1][goToX - 1] == 0) && countGo) {
            textureCheck[goToY - 1][goToX - 1] = 2;
        }
    }
    if ((goToY - 1) >= 0 && (goToX + 1) <= 7) {
        if ((textureChess[goToY - 1][goToX + 1] == 2) && (((goToY - 2) >= 0 && (goToX + 2) <= 7))) {
            if (textureChess[goToY - 2][goToX + 2] == 0) {
                textureCheck[goToY - 2][goToX + 2] = 3;
            }
        } else if ((textureChess[goToY - 1][goToX + 1] == 0) && countGo) {
            textureCheck[goToY - 1][goToX + 1] = 2;
        }
    }
}


//Аналогия с черными.
void chess::goBlack() {

    checkMod = true;

    goToX = xPos;
    goToY = yPos;

    if ((goToY + 1) <= 7 && (goToX + 1) <= 7) {

        if ((textureChess[goToY + 1][goToX + 1] == 1) && (((goToY + 2) <= 7 && (goToX + 2) <= 7))) {
            if (textureChess[goToY + 2][goToX + 2] == 0) {
            textureCheck[goToY + 2][goToX + 2] = 3;
            }
        } else if ((textureChess[goToY + 1][goToX + 1] == 0) && countGo) {
            textureCheck[goToY + 1][goToX + 1] = 2;
        }
    }
    if ((goToY + 1) <= 7 && (goToX - 1) >= 0) {
        if ((textureChess[goToY + 1][goToX - 1] == 1) && (((goToY + 2) <= 7 && (goToX - 2) >= 0))) {
            if (textureChess[goToY + 2][goToX - 2] == 0) {
                textureCheck[goToY + 2][goToX - 2] = 3;
            }
        } else if ((textureChess[goToY + 1][goToX - 1] == 0) && countGo) {
            textureCheck[goToY + 1][goToX - 1] = 2;
        }
    }
}


// Функция когда мы идем в точку выбора
// Ну еще и меняем очередь хода.
void chess::goCheck() {

    textureChess[yPos][xPos] = textureChess[goToY][goToX];
    textureChess[goToY][goToX] = 0;

    tirn = !tirn;

    // Бесполезная переменная, просто позже может пригодиться.
    countGo = true;

    endGo();
}

// Функция проверки можно ли куда нибудь сходить.
void chess::whatMod() {

    int index, indey, chMod = 0;


    //Пробегает по массиву ходов и если он пуст то выключает режим выбора хода.
    for (indey = 0; indey < 8; ++indey) {
        for (index = 0; index < 8; ++index) {
            if ((textureCheck[indey][index] == 2) || (textureCheck[indey][index] == 3)) {
                ++chMod;
            }
        }
    }
    if (chMod == 0) {
        checkMod = false;
    }
}
