#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <iostream>
#include <QFrame>
#include <QLabel>
#include <QMainWindow>
#include <QKeyEvent>
#include <QEvent>
#include <QTimer>
#include <QThread>
#include <QFileInfo>
#include <QDesktopServices>
using namespace std;
//敌机单位的大小
#define E_HEIGHT 70
#define E_WIDTH 70
//我放飞机的大小
#define P_HEIGHT 60
#define P_WIDTH 60
//子弹的大小
#define B_HEIGHT 24
#define B_WIDTH 24
//子弹发射时间间隔
#define BT_GAP 500
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
//主体窗口类，继承自QMainWindow
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    //主体窗口的构造函数
    MainWindow(QWidget *parent = nullptr);
    //主体窗口的析构函数
    ~MainWindow();
    //重写keypressEvent设置键盘事件
    void keyPressEvent(QKeyEvent *event) override;
    //重写timerEvent设置游戏界面的定时器事件
    void timerEvent(QTimerEvent *t) override;
    //创建游戏背景界面函数接口
    void CreateGP();
    //创建飞机函数接口
    void InitialP();
    //创建得分表函数接口
    void CreateScr();
    //相关信息的链接变量
    QString Url;
    //记录分数的变量
    int score=0;
    //记录飞机坐标的变量
    int P_X;
    int P_Y;
    //区分定时器的变量
    int Self_BID;
    int Ener_BID;
    int Crashed;
    //主窗口对象指针
    Ui::MainWindow *ui;
    //飞机的QLabel指针对象
    QLabel *Plane;
    //游戏背景界面的QFrame对象指针
    QFrame* GAMEPAGE;
    //显示得分的QLabel对象指针
    QLabel* Score;
    //加载飞机图形的QImage指针对象
    QImage* Normal;
    QImage* Damaged;
signals:
public slots:
    //程序退出的槽函数接口
    void On_Exit();
    //重新开始的槽函数接口
    void Restart();
    //打开网页的槽函数接口
    void OpenWeb();
private slots:
    //开始游戏的槽函数接口
    void on_Start_released();
};
//子弹类
class Bullet:public QObject{
    Q_OBJECT
public:
    //存储子弹的当前坐标
    int P_CurrX;
    int P_CurrY;
    //子弹类的构造函数
    Bullet(MainWindow* win);
    //子弹的碰撞事件
    void Crash(){
        //逐一销毁，释放内存，销毁子弹
        timer->stop();
        delete bullet;
        delete bul_img;
        delete timer;
        delete this;
    }
public slots:
    //定时器的槽函数接口
    void Timer();
private:
    //定时器的指针对象
    QTimer* timer;
    //初始化子弹图像的指针对象
    QImage* bul_img;
    //放置指针图片的指针对象
    QLabel* bullet;
    int Curr_Id;
};
//敌机类
class Enermy:public QObject{
    Q_OBJECT
public:
    //存储敌机对象的当前坐标
    int E_CurrX;
    int E_CurrY;
    //敌机类的构造函数
    Enermy(MainWindow* win);
    //敌机的碰撞事件
    void Crash(){
        //逐一销毁，释放内存，销毁敌机
        timer->stop();
        delete enermy;
        delete E_img;
        delete timer;
        delete this;
    }
public slots:
    //定时器的槽函数接口
    void Timer();
private:
    //定时器指针对象
    QTimer *timer;
    //图片初始化指针对象
    QImage* E_img;
    //显示敌机的控件
    QLabel* enermy;
    int Curr_Id;
};
#endif // MAINWINDOW_H
