#include "mainwindow.h"
#include "ui_mainwindow.h"

const string PlayerName[3] = {"Null", "Little Yellow", "Little Green"};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for (int i = 0; i < 3;++i){
        char filename[64];
        sprintf(filename, "chess%d.png", i);
        chessPic[i] = QPixmap(GetFileDir(filename)).scaled(100,100,Qt::KeepAspectRatioByExpanding);
    }
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Update()));
    timer -> start(1000 / 20);
    ui->winLabel->setText("小Y回合");
    player = 1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Update(){
    this -> update();//更新画面
}

int test[3][3];

void MainWindow::mousePressEvent(QMouseEvent *event){
    QPoint p = event->pos();
    int r = (p.y() - DRAW_ST_Y) / CHESS_SIZE;
    int c = (p.x() - DRAW_ST_X) / CHESS_SIZE;
    if (sharpGame.InBoard(r,c) && sharpGame.CanPut(r,c) && !sharpGame.GetWinner()){
        sharpGame.Set(r,c,player);
        player += 1;
        if (player > 2)player = 1;
        char str[64];
        sprintf(str, "小%c回合", PlayerName[player][7]);
        ui->winLabel->setText(str);
    }
    int w = sharpGame.GetWinner();
    if (w){
        string str;
        str += PlayerName[w];
        str += " 胜利！";
        ui->winLabel->setText(QString::fromStdString(str));
    }else if (sharpGame.IsFull()){
        ui->winLabel->setText("平局了:-(");
    }
    event->accept();
    this -> update();
}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter;

    painter.begin(this);

    for (int r = 0;r < 3;++r){
        for (int c = 0;c < 3;++c){
            painter.drawPixmap(DRAW_ST_X + c * CHESS_SIZE, DRAW_ST_Y + r * CHESS_SIZE,chessPic[sharpGame.Get(r,c)]);
        }
    }

    painter.end();
}
