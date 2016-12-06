#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "defines.h"

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
    QPixmap miniChessPic[3];
    const int DRAW_ST_X = 0, DRAW_ST_Y = 0;
    const int MINI_DRAW_ST_X = 418, MINI_DRAW_ST_Y = 340;
    const int CHESS_SIZE = 100;
    const int MINI_CHESS_SIZE = 16;
    int player;

private slots:
    void Update();

    void on_btn_start_clicked();

private:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void Judge();
};

#endif // MAINWINDOW_H
