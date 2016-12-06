#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sharpgame.h"

ChessBoard sharpGame;
ChessBoard minicb;
const string PlayerName[3] = {"Null", "Little Yellow", "Little Green"};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for (int i = 0; i < 3;++i){
        char filename[64];
        sprintf(filename, "chess%d.png", i);
        QPixmap pic = GetFileDir(filename);
        chessPic[i] = pic.scaled(CHESS_SIZE,CHESS_SIZE,Qt::KeepAspectRatioByExpanding);
        miniChessPic[i] = pic.scaled(MINI_CHESS_SIZE,MINI_CHESS_SIZE,Qt::KeepAspectRatioByExpanding);
    }
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Update()));
    timer -> start(1000 / 20);
    ui->winLabel->setText("Y Turn");
    player = 1;
    ui->tableWidget->resizeRowsToContents();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Update(){
    if (player == 2){
        POS p = AI(sharpGame, ui->spinBox->value(), ui->checkBox->checkState() == Qt::Checked);
        int r = p.first;
        int c = p.second;
        if (sharpGame.InBoard(r,c) && sharpGame.CanPut(r,c) && !sharpGame.get_winner()){
            sharpGame.Set(r,c,player);
        }
        player = 3 - player;
    }
    int cut = 0;
    for (auto &nd : abcut_nodes){
        cut += nd.cut;
    }
    char count_text[64];
    sprintf(count_text, "#Node: %d\n#Cut: %d\nScore: %d", abcut_count, cut, get_score(sharpGame));
    ui->countLabel->setText(count_text);
    Judge();

    ui->tableWidget->setRowCount(abcut_nodes.size());
    for (size_t i = 0;i < abcut_nodes.size();++i){
        Node &nd = abcut_nodes[i];
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(nd.ID)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(nd.type.c_str()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString("%1").arg(nd.ab)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString("%1").arg(nd.score)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString("%1").arg(nd.cut)));
        QString strbo;
        for (int r = 0; r < 3;++r){
            for (int c = 0;c < 3;++c){
                strbo += (nd.board.data[r][c] + '0');
                strbo += " ";
            }
            strbo += '\n';
        }
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(strbo));
    }


    int row = ui->tableWidget->currentIndex().row();
    if (row >= 0 && row < int(abcut_nodes.size())){
        minicb = abcut_nodes[row].board;
    }

    this -> update();//更新画面
}

void MainWindow::Judge(){
    char str[64];
    sprintf(str, "Turn: %c\n\nPerson is Yellow(Y)\nAI is Green(G)", PlayerName[player][7]);
    ui->winLabel->setText(str);
    int w = sharpGame.get_winner();
    if (w){
        string str;
        str += PlayerName[w];
        str += " Win";
        ui->winLabel->setText(QString::fromStdString(str));
    }else if (sharpGame.IsFull()){
        ui->winLabel->setText("Tied");
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    QPoint p = event->pos();
    int r = (p.y() - DRAW_ST_Y) / CHESS_SIZE;
    int c = (p.x() - DRAW_ST_X) / CHESS_SIZE;
    if (sharpGame.InBoard(r,c) && sharpGame.CanPut(r,c) && !sharpGame.get_winner()){
        sharpGame.Set(r,c,player);
        player = 3-player;
    }
    Judge();
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

    for (int r = 0;r < 3;++r){
        for (int c = 0;c < 3;++c){
            painter.drawPixmap(MINI_DRAW_ST_X + c * MINI_CHESS_SIZE, MINI_DRAW_ST_Y + r * MINI_CHESS_SIZE,miniChessPic[minicb.Get(r,c)]);
        }
    }

    painter.end();
}

void MainWindow::on_btn_start_clicked()
{
    player = ui->comboBox->currentIndex() + 1;
    sharpGame.restart();
    abcut_count = 0;
    abcut_nodes.clear();
    Judge();
}
