#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <QDebug>
#include <windows.h>
#include <list>
//初始化存储子弹的链表
list<Bullet*> My_Bullet;
//初始化存储敌机的链表
list<Enermy*> Enermies;
//主体窗口的构造函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //连接退出按键至对应的槽函数
    QObject::connect(ui->Exit,SIGNAL(triggered()),this,SLOT(On_Exit()));
    //连接关于内的按键至对应的槽函数
    QObject::connect(ui->Member,SIGNAL(triggered()),this,SLOT(OpenWeb()));
    QObject::connect(ui->Copyright,SIGNAL(triggered()),this,SLOT(OpenWeb()));
    //设置窗口大小不变
    setFixedSize(this->width(),this->height());
}
MainWindow::~MainWindow()
{
    delete ui;
}
//退出的槽函数
void MainWindow::On_Exit(){
    QApplication::quit();
}
//重新开始的槽函数
void MainWindow::on_Start_released(){
    delete ui->Begin;
    CreateGP();
}
//初始化背景窗口
void MainWindow::CreateGP(){
    GAMEPAGE=new QFrame(ui->Center);
    GAMEPAGE->setGeometry(0,0,800,600);
    GAMEPAGE->setStyleSheet("QFrame{background-color: '#e4e4e4';};");
    //连接重新开始按键至对应的槽函数
    QObject::connect(ui->Replay,SIGNAL(triggered()),this,SLOT(Restart()));
    //设置相应鼠标事件
    setEnabled(true);
    setFocusPolicy(Qt::StrongFocus);
    GAMEPAGE->show();
    //调用初始飞机函数
    InitialP();
    //调用初始得分表函数
    CreateScr();
}
//初始化飞机对象
void MainWindow::InitialP(){
    //设置飞机图像
    Normal=new QImage(":/myplane.png");//未受击
    Damaged=new QImage(":/myplane_damaged.png");//已受击
    Plane=new QLabel(ui->Center);
    //设置为对应图像，并自适应大小
    Plane->setPixmap(QPixmap::fromImage(*Normal));
    Plane->setScaledContents(true);
    Plane->setGeometry(350,400,P_HEIGHT,P_WIDTH);
    //获取飞机位置
    P_X=Plane->x();
    P_Y=Plane->y();
    Plane->show();
    //随即开始主界面的定时器事件
    Self_BID=startTimer(BT_GAP);
    Ener_BID=startTimer(5*BT_GAP);
    Crashed=startTimer(20);
}
//键盘事件，控制飞机的移动，同时设置移动边界
void MainWindow::keyPressEvent(QKeyEvent *event){
    //判定为上方向键，向上移动
    if(event->key()==Qt::Key_Up){
        if(P_Y>=0){
            P_Y-=5;
            Plane->setGeometry(P_X,P_Y,P_HEIGHT,P_WIDTH);
        }
    }
    //判定为下方向键，向下移动
    if(event->key()==Qt::Key_Down){
        if(P_Y<=520){
            P_Y+=5;
            Plane->setGeometry(P_X,P_Y,P_HEIGHT,P_WIDTH);
        }
    }
    //判定为左方向键，向左移动
    if(event->key()==Qt::Key_Left){
        if(P_X>=0){
            P_X-=5;
            Plane->setGeometry(P_X,P_Y,P_HEIGHT,P_WIDTH);
        }
    }
    //判定为右方向键，向右移动
    if(event->key()==Qt::Key_Right){
       if(P_X<=750){
           P_X+=5;
           Plane->setGeometry(P_X,P_Y,P_HEIGHT,P_WIDTH);
       }
    }
}
//设置得分记录器
void MainWindow::CreateScr(){
    Score=new QLabel(this->GAMEPAGE);
    //设置其样式
    Score->setStyleSheet("QLabel{background:rbga(0,0,0,0);border-color:'black';border-width:2px;border-style:solid;border-radius:5px;font-family: 'Microsoft Yahei UI';};");
    Score->setGeometry(0,0,100,25);
    //初始设为零分
    Score->setText("得分：0");
    Score->show();
}
//游戏界面的总定时器事件
void MainWindow::timerEvent(QTimerEvent *t){
    //生成子弹的定时器
    if(t->timerId()==Self_BID){
        //构建子弹类的对象指针，并加入存储子弹的链表
        Bullet *Bt=new Bullet(this);
        My_Bullet.push_back(Bt);
    }
    //生成敌机的定时器
    if(t->timerId()==Ener_BID){
        //构建敌机类的对象指针，并加入存储敌机的链表
        Enermy *Em=new Enermy(this);
        Enermies.push_back(Em);
    }
    //判定碰撞事件的定时器
    if(t->timerId()==Crashed){
        //判定是否已碰撞
        int Csh=false;
        //遍历存储子弹的链表
        for(auto M_B:My_Bullet){
            int PBx=M_B->P_CurrX+B_WIDTH/2;
            int PBy=M_B->P_CurrY+B_HEIGHT/2;
            //对每一个子弹对象，遍历所有敌机判定是否碰撞
            for(auto E:Enermies){
                //判定条件：若子弹与敌机的图片已重合，则视为已碰撞
                if(PBx>=E->E_CurrX&&PBx<=E->E_CurrX+E_WIDTH){
                    if(PBy>=E->E_CurrY&&PBy<=E->E_CurrY+E_HEIGHT){
                        //即刻从相应的链表中删除对应的对象指针
                        My_Bullet.remove(M_B);
                        Enermies.remove(E);
                        //触发碰撞事件的函数
                        M_B->Crash();
                        E->Crash();
                        //得分增加
                        score+=10;
                        Score->setText("得分："+QString::number(score));
                        //已碰撞，不再遍历，退出循环
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
//子弹类的构造函数，用于生成子弹
Bullet::Bullet(MainWindow* win):P_CurrX(win->P_X+(P_WIDTH-B_WIDTH)/2),P_CurrY(win->P_Y-B_HEIGHT){
    //初始化子弹对应的图像
    bul_img=new QImage(":/dot.png");
    bullet=new QLabel(win->GAMEPAGE);
    //设置子弹为改图，并自适应QLabel的对象大小
    bullet->setScaledContents(true);
    bullet->setPixmap(QPixmap::fromImage(*bul_img));
    bullet->setGeometry(P_CurrX,P_CurrY,B_HEIGHT,B_WIDTH);
    //设置子弹的QLabel对象背景为全透明
    bullet->setStyleSheet("QLabel{background:rbga(0,0,0,0);};");
    bullet->show();
    //初始化定时器，用于子弹的移动
    timer=new QTimer(this);
    timer->start(20);
    //连接定时器对象与与之对应的槽函数
    connect(timer,&QTimer::timeout,this,&Bullet::Timer);
}
//子弹timer对象的槽函数，用于控制子弹的移动
void Bullet::Timer(){
    //定时设置子弹的位置
    P_CurrY-=4;
    bullet->setGeometry(P_CurrX,P_CurrY,B_HEIGHT,B_WIDTH);
    //若子弹超出屏幕，则销毁该子弹
    if(P_CurrY<0){
        //从存储子弹的链表中删除对应子弹的对象指针
        My_Bullet.remove(this);
        //停止定时器
        timer->stop();
        //逐一销毁相关的对象指针释放内存，销毁子弹
        delete bullet;
        delete bul_img;
        delete timer;
        delete this;
    }
}
//敌机类的构造函数，用于生成敌机
Enermy::Enermy(MainWindow* win){
    //设置生成的位置
    E_CurrX=(rand()%726);
    E_CurrY=0;
    //初始化敌机的图片
    E_img=new QImage(":/enermy.png");
    enermy=new QLabel(win->GAMEPAGE);
    //设置为该图，并自动适配QLabel控件大小
    enermy->setPixmap(QPixmap::fromImage(*E_img));
    enermy->setScaledContents(true);
    enermy->setGeometry(E_CurrX,E_CurrY,E_HEIGHT,E_WIDTH);
    //背景置为全透明
    enermy->setStyleSheet("QLabel{background:rbga(0,0,0,0);};");
    enermy->show();
    //初始化timer对象，连接槽函数，设置敌机的移动状态
    timer=new QTimer(this);
    timer->start(20);
    connect(timer,&QTimer::timeout,this,&Enermy::Timer);
}
//敌机timer对象的槽函数，用于敌机的移动
void Enermy::Timer(){
    //定时设置敌机位置
    E_CurrY+=2;
    enermy->setGeometry(E_CurrX,E_CurrY,E_HEIGHT,E_WIDTH);
    //超出屏幕则销毁敌机单位对象
    if(E_CurrY>600){
        //从存储敌机的链表中删除敌机对象
        Enermies.remove(this);
        //停止定时器
        timer->stop();
        //逐一销毁相应的对象指针，释放内存，销毁子弹
        delete enermy;
        delete E_img;
        delete timer;
        delete this;
    }
}
//重新开始的槽函数
void MainWindow::Restart(){
    //遍历存储子弹对象指针的链表，移除所有的子弹
    for(auto B:My_Bullet){
        My_Bullet.remove(B);
        B->Crash();
    }
    //清空链表
    My_Bullet.clear();
    //遍历存储敌机对象指针的链表，移除所有敌机
    for(auto E:Enermies){
        Enermies.remove(E);
        E->Crash();
    }
    //清空链表
    Enermies.clear();
    //得分置零
    score=0;
    Score->setText("得分："+QString::number(score));
    //飞机位置置为初始位置
    Plane->setGeometry(350,400,P_HEIGHT,P_WIDTH);
    P_X=Plane->x();
    P_Y=Plane->y();
}
//打开相关选项的槽函数
void MainWindow::OpenWeb(){
    //设置指定的链接
    Url="https://virtual-memes.gitee.io/about/";
    //调用本地默认浏览器打开网页
    QDesktopServices::openUrl(QUrl(Url.toLatin1()));
}
