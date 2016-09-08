#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "defines.h"
#include "sharpgame.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QPixmap chessPic[3];
    const int DRAW_ST_X = 0, DRAW_ST_Y = 0;
    const int CHESS_SIZE = 100;
    int player;
    SharpGame sharpGame;

private slots:
    void Update();

private:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

};

#endif // MAINWINDOW_H
