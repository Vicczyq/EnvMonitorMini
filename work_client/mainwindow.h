#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "debug.h"
#include <QSerialPort>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket* socket;
    Debug* DebugWindow;
    QSerialPort* serial_Get;
    QSerialPort* serial_Send;
    short LED_color;
    float temperature;
    float mcu_temperature;
};

#endif // MAINWINDOW_H
