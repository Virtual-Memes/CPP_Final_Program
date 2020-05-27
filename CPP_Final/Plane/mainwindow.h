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
using namespace std;
#define E_HEIGHT 70
#define E_WIDTH 70
#define P_HEIGHT 60
#define P_WIDTH 60
#define B_HEIGHT 24
#define B_WIDTH 24
#define BT_GAP 500
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *t) override;
    void CreateGP();
    void InitialP();
    void CreateScr();
    static int score;
    int P_X;
    int P_Y;
    int Self_BID;
    int Ener_BID;
    int Crashed;
    Ui::MainWindow *ui;
    QLabel *Plane;
    QFrame* GAMEPAGE;
    QLabel* Score;
    QImage* Normal;
    QImage* Damaged;
    QKeyEvent* PlaneCtrl;
signals:
public slots:
    void On_Exit();
private slots:
    void on_Start_released();
};
class Bullet:public QObject{
    Q_OBJECT
public:
    int P_CurrX;
    int P_CurrY;
    Bullet(MainWindow* win);
    void Crash(){
        timer->stop();
        delete bullet;
        delete bul_img;
        delete timer;
        delete this;
    }
public slots:
    void Timer();
private:
    QTimer* timer;
    QImage* bul_img;
    QLabel* bullet;
    int Curr_Id;
};
class Enermy:public QObject{
    Q_OBJECT
public:
    int E_CurrX;
    int E_CurrY;
    Enermy(MainWindow* win);
    void Crash(){
        timer->stop();
        delete enermy;
        delete E_img;
        delete timer;
        delete this;
    }
public slots:
    void Timer();
private:
    QTimer *timer;
    QImage* E_img;
    QLabel* enermy;
    int Curr_Id;
};
#endif // MAINWINDOW_H
