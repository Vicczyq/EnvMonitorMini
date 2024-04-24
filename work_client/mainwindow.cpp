#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include <QHostAddress>
#include <QMessageBox>
#include <QDateTime>
#define port 5555
#define ip "127.0.0.1"
#define SERIAL_name1 "COM8"
#define SERIAL_name2 "COM9"
QString GetTime(){
    return QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss]");
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Login* loginWindow = new Login();
    loginWindow->setWindowFlags(loginWindow->windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);
    DebugWindow = new Debug();
    DebugWindow->setWindowFlags(DebugWindow->windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);
    DebugWindow->hide();
    loginWindow->show();

    connect(loginWindow, &Login::loginSuccess, this, [=](){
       this->show();
       loginWindow->close();
    });

    //连接网络端
    socket = new QTcpSocket();
    socket->connectToHost(QHostAddress(ip),port);
    connect(socket, &QTcpSocket::connected, this, [=](){
        DebugWindow->add_info_network("连接成功！");
    });

    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, [=](QAbstractSocket::SocketError socketError) {
        // 输出连接错误信息
        DebugWindow->add_info_network(GetTime() +"连接错误：" + socket->errorString() + ",正在重新连接...");
        // 断开连接
        socket->disconnectFromHost();
        // 重新连接
        socket->connectToHost(QHostAddress(ip), port);
    });

    connect(socket, &QTcpSocket::readyRead, this, [=](){
        QByteArray data = socket->readAll();
        DebugWindow->add_info_network(GetTime() + "Remote Server："+data);
        if(data=="E"){
            exit(-1);
        }
        serial_Send->write(data);
    });

    //连接串口
    serial_Get = new QSerialPort();
    serial_Send = new QSerialPort();
    serial_Get->setPortName(SERIAL_name1);//【注意】修改此处端口为对应的，懒得写选项了
    serial_Send->setPortName(SERIAL_name2);//【注意】修改此处端口为对应的，懒得写选项了
    serial_Get->open(QIODevice::ReadWrite);
    serial_Get->setBaudRate(QSerialPort::Baud115200);
    serial_Get->setDataBits(QSerialPort::Data8);
    serial_Get->setParity(QSerialPort::NoParity);
    serial_Get->setStopBits(QSerialPort::OneStop);
    serial_Get->setFlowControl(QSerialPort::NoFlowControl);

    serial_Send->open(QIODevice::ReadWrite);
    serial_Send->setBaudRate(QSerialPort::Baud115200);
    serial_Send->setDataBits(QSerialPort::Data8);
    serial_Send->setParity(QSerialPort::NoParity);
    serial_Send->setStopBits(QSerialPort::OneStop);
    serial_Send->setFlowControl(QSerialPort::NoFlowControl);

    QStringList colors = {"暗色", "红色", "黄色", "紫色", "白色", "绿色", "青色", "蓝色"};
    connect(serial_Get, &QSerialPort::readyRead, this, [=](){
        QByteArray data = serial_Get->readAll();
        static QByteArray buffer;
        static int up = 0;
        static int now = 0;
//        qDebug() << data;
        for(int i=0;i<data.size();i++)
        {
            if(data[i]=='{'){
                buffer.clear();
            } else if(data[i] == '}'){
                DebugWindow->add_info_serial(GetTime()+"【INFO】"+buffer);
                QStringList dataList = QString(buffer).remove('\n').split(",");
                if(dataList.size() == 4){
                    ui->wd->setText(dataList[1]+"℃");
                    temperature = dataList[0].toFloat();
                    ui->sd->setText(dataList[0]+"℃");
                    mcu_temperature = dataList[1].toFloat();
                    LED_color = dataList[2].toInt();

                    if(LED_color==-1)ui->led->setText("关闭");
                    else ui->led->setText(colors[LED_color]);

                    up = now;
                    now = dataList[3].toInt();
                    if(~up & now){
                        QString message = "当前温度：";
                        message += QString::number(temperature);
                        message += "℃，MCU温度：";
                        message += QString::number(mcu_temperature);
                        message += "℃\n温度超过了设定值！";
                        on_pushButton_2_clicked();
                        QMessageBox::warning(this, "警告", message);
                    }
                    socket->write(buffer);
                }
            } else {
                buffer.append(data[i]);
            }
        }

    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_3_clicked()
{
    DebugWindow->show();
}

void MainWindow::on_pushButton_clicked()
{
    if(LED_color!=-1){
        serial_Send->write("0");//关灯
    }else{
        serial_Send->write("1");//开灯
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QByteArray msg = QString::number(temperature).toUtf8() + "," +
                         QString::number(mcu_temperature).toUtf8() + "," +
                         QString::number(LED_color).toUtf8() + ",1";
    socket->write(msg);
    DebugWindow->add_info_network("服务端:报警");
}
