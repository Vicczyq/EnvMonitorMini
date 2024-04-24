#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "login.h"
#include "tcp.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void showMainPage();



    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    int tel_count;
    Ui::MainWindow *ui;
    Login *loginWindow;
    TCP * tcpWindow;
    QTcpServer *server;
    QVector<QTcpSocket*> client_sockets;
    float temperature;
    float humility;
    int LED;
    QTimer *timer;
    QTimer *End_Timer;
};

#endif // MAINWINDOW_H
