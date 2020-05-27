#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <QDebug>
#include <windows.h>
#include <list>
int MainWindow::score=0;
list<Bullet*> My_Bullet;
list<Enermy*> Enermies;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->Exit,SIGNAL(triggered()),this,SLOT(On_Exit()));
    setFixedSize(this->width(),this->height());

}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::On_Exit(){
    QApplication::quit();
}
void MainWindow::on_Start_released(){
    delete ui->Begin;

    CreateGP();
}
void MainWindow::CreateGP(){
    GAMEPAGE=new QFrame(ui->Center);
    GAMEPAGE->setGeometry(0,0,800,600);
    GAMEPAGE->setStyleSheet("QFrame{background-color: '#e4e4e4';};");
    setEnabled(true);
    setFocusPolicy(Qt::StrongFocus);
    GAMEPAGE->show();
    InitialP();
    CreateScr();
}
void MainWindow::InitialP(){
    Normal=new QImage(":/myplane.png");
    Damaged=new QImage(":/myplane_damaged.png");
    Plane=new QLabel(ui->Center);
    Plane->setPixmap(QPixmap::fromImage(*Normal));
    Plane->setScaledContents(true);
    Plane->setGeometry(350,400,P_HEIGHT,P_WIDTH);
    P_X=Plane->x();
    P_Y=Plane->y();
    Plane->show();
    Self_BID=startTimer(BT_GAP);
    Ener_BID=startTimer(5*BT_GAP);
    Crashed=startTimer(20);
}
void MainWindow::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Up){
        if(P_Y>=0){
            P_Y-=5;
            Plane->setGeometry(P_X,P_Y,P_HEIGHT,P_WIDTH);
        }
    }
    if(event->key()==Qt::Key_Down){
        if(P_Y<=520){
            P_Y+=5;
            Plane->setGeometry(P_X,P_Y,P_HEIGHT,P_WIDTH);
        }
    }
    if(event->key()==Qt::Key_Left){
        if(P_X>=0){
            P_X-=5;
            Plane->setGeometry(P_X,P_Y,P_HEIGHT,P_WIDTH);
        }
    }

    if(event->key()==Qt::Key_Right){

       if(P_X<=750){
           P_X+=5;
           Plane->setGeometry(P_X,P_Y,P_HEIGHT,P_WIDTH);
       }
    }
}
void MainWindow::CreateScr(){
    Score=new QLabel(this->GAMEPAGE);
    Score->setStyleSheet("QLabel{background:rbga(0,0,0,0);border-color:'black';border-width:2px;border-style:solid;border-radius:5px;font-family: 'Microsoft Yahei UI';};");
    Score->setGeometry(0,0,100,25);
    Score->setText("得分：0");
    Score->show();
}
void MainWindow::timerEvent(QTimerEvent *t){
    if(t->timerId()==Self_BID){
        Bullet *Bt=new Bullet(this);
        My_Bullet.push_back(Bt);
    }
    if(t->timerId()==Ener_BID){
        Enermy *Em=new Enermy(this);
        Enermies.push_back(Em);
    }
    if(t->timerId()==Crashed){
        int Csh=false;
        for(auto M_B:My_Bullet){
            int PBx=M_B->P_CurrX+B_WIDTH/2;
            int PBy=M_B->P_CurrY+B_HEIGHT/2;
            for(auto E:Enermies){
                if(PBx>=E->E_CurrX&&PBx<=E->E_CurrX+E_WIDTH){
                    if(PBy>=E->E_CurrY&&PBy<=E->E_CurrY+E_HEIGHT){
                        My_Bullet.remove(M_B);
                        Enermies.remove(E);
                        M_B->Crash();
                        E->Crash();
                        score+=10;
                        Score->setText("得分："+QString::number(score));
                        Csh=true;
                    }
                }
                if(Csh){
                    break;
                }
            }
            if(Csh){
                break;
            }
        }
    }
}
Bullet::Bullet(MainWindow* win):P_CurrX(win->P_X+(P_WIDTH-B_WIDTH)/2),P_CurrY(win->P_Y-B_HEIGHT){
    bul_img=new QImage(":/dot.png");
    bullet=new QLabel(win->GAMEPAGE);
    bullet->setScaledContents(true);
    bullet->setPixmap(QPixmap::fromImage(*bul_img));
    bullet->setGeometry(P_CurrX,P_CurrY,B_HEIGHT,B_WIDTH);
    bullet->setStyleSheet("QLabel{background:rbga(0,0,0,0);};");
    bullet->show();
    timer=new QTimer(this);
    timer->start(20);
    connect(timer,&QTimer::timeout,this,&Bullet::Timer);
}
void Bullet::Timer(){
    P_CurrY-=4;
    bullet->setGeometry(P_CurrX,P_CurrY,B_HEIGHT,B_WIDTH);
    if(P_CurrY<0){
        My_Bullet.remove(this);
        timer->stop();
        delete bullet;
        delete bul_img;
        delete timer;
        delete this;
    }
}
Enermy::Enermy(MainWindow* win){
    E_CurrX=(rand()%726);
    E_CurrY=0;
    E_img=new QImage(":/enermy.png");
    enermy=new QLabel(win->GAMEPAGE);
    enermy->setPixmap(QPixmap::fromImage(*E_img));
    enermy->setScaledContents(true);
    enermy->setGeometry(E_CurrX,E_CurrY,E_HEIGHT,E_WIDTH);
    enermy->setStyleSheet("QLabel{background:rbga(0,0,0,0);};");
    enermy->show();
    timer=new QTimer(this);
    timer->start(20);
    connect(timer,&QTimer::timeout,this,&Enermy::Timer);
}
void Enermy::Timer(){
    E_CurrY+=2;
    enermy->setGeometry(E_CurrX,E_CurrY,E_HEIGHT,E_WIDTH);
    if(E_CurrY>600){
        Enermies.remove(this);
        timer->stop();
        delete enermy;
        delete E_img;
        delete timer;
        delete this;
    }
}
