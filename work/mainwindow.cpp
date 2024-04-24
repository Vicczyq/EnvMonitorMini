#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include <QDateTime>
#include <QTime>
#include <QMessageBox>
#define port 5555
QString GetTime(){
   return  "[" + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "]";
}

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loginWindow = new Login();
    tel_count = 0;

    connect(loginWindow, SIGNAL(loginSuccess()),this, SLOT(showMainPage()));

    loginWindow->show();
}

MainWindow::~MainWindow()
{
    server->close();
    delete ui;
}

void MainWindow::showMainPage(){
    timer = new QTimer(this);
    End_Timer = new QTimer(this);
    ui->timeEdit_2->setMinimumTime(QTime::currentTime());
    ui->timeEdit->setMinimumTime(QTime::currentTime());

    connect(timer,&QTimer::timeout,this,[=](){
        QByteArray msg;
        if(ui->comboBox_2->currentIndex()==0){
            msg = "1";
        }else{
            msg = "0";
        }
        client_sockets[ui->comboBox->currentIndex()]->write(msg);
        ui->listWidget->addItem(GetTime()+"定时应用:"+msg);
        ui->pushButton_3->setText("确认");
        ui->timeEdit->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        timer->stop();
    });

    connect(End_Timer,&QTimer::timeout,this,[=](){
        client_sockets[ui->comboBox->currentIndex()]->write("E");
        ui->listWidget->addItem(GetTime()+"定时应用:关闭上位机");
        ui->pushButton_4->setText("确认");
        ui->timeEdit_2->setEnabled(true);
        End_Timer->stop();
    });

    server = new QTcpServer();
    if(!server->listen(QHostAddress::Any,port)){
        qDebug()<<server->errorString();
        return;
    }
    qDebug()<<"Listen succeed, Message from port:"<<port;
    QStringList colors = {"暗色", "红色", "黄色", "紫色", "白色", "绿色", "青色", "蓝色"};
    connect(server, &QTcpServer::newConnection , this,[=](){
        QTcpSocket* socket = server->nextPendingConnection();

        client_sockets.append(socket);
        ui->comboBox->addItem(QString::number(tel_count));

        ui->connect_count->setText(QString::number(++tel_count));
        connect(socket, &QTcpSocket::readyRead, this, [=](){

            QString buffer = socket->readAll();
            static int up = 0,now = 0;

            ui->listWidget->addItem(GetTime() + "Client: " +buffer);
            ui->listWidget->scrollToBottom();
            QStringList data = buffer.split(',');
            ui->temperature->setText("温度:"+data[1]+"℃");
            ui->humidity->setText("MCU:"+data[0]+"℃");

            ui->warn->setText("警告:"+data[3]);
            up = now;
            now = data[3].toInt();
            if(!up && now){
                QMessageBox::warning(this,"警告","本地温度过高，请查看详情！");
            }

            humility = data[1].toFloat();
            temperature = data[0].toFloat();
            LED = data[2].toInt();
            if(LED!=-1)ui->LED->setText("LED:" + colors[LED]);
            else ui->LED->setText("LED:关闭");

        });

        connect(socket, &QTcpSocket::disconnected, this, [=](){
            ui->connect_count->setText(QString::number(--tel_count));
            for(int i=0; i < client_sockets.size();i++){
                if( socket == client_sockets.at(i)){
                    client_sockets.erase(client_sockets.begin()+i);
                    ui->comboBox->removeItem(i);
                    break;
                }
            }
            ui->comboBox->clear();
            for(int i=0; i<client_sockets.size(); i++){
                ui->comboBox->addItem(QString::number(i));
            }

            qDebug()<<"client disconnected!";
            ui->listWidget->addItem(GetTime()+"Disconnected!");
            ui->listWidget->scrollToBottom();
            socket->close();
            socket->deleteLater();

        });
    });

    this->show();
}


void MainWindow::on_pushButton_2_clicked()
{
        if(LED!=-1){
            client_sockets[ui->comboBox->currentIndex()]->write("0");
        }else{
            client_sockets[ui->comboBox->currentIndex()]->write("1");
        }
}

void MainWindow::on_pushButton_3_clicked()
{
    if(ui->pushButton_3->text()=="确认")
    {
        QTime selectedTime = ui->timeEdit->time();
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QTime currentTime = currentDateTime.time();

       // 计算时间间隔
       int interval = currentTime.msecsTo(selectedTime);
       if (interval < 0) {
           interval += 24 * 60 * 60 * 1000;
       }
        qDebug()<<interval;
       // 设置定时器的单次定时时间
       timer->start(interval);
       ui->pushButton_3->setText("取消");
       ui->timeEdit->setEnabled(false);
       ui->comboBox_2->setEnabled(false);
    }else{
        timer->stop();
        ui->pushButton_3->setText("确认");
        ui->timeEdit->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    if(ui->pushButton_4->text()=="确认")
    {
        QTime selectedTime = ui->timeEdit_2->time();
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QTime currentTime = currentDateTime.time();
       // 计算时间间隔
       int interval = currentTime.msecsTo(selectedTime);
       if (interval < 0) {
           interval += 24 * 60 * 60 * 1000;
       }
       End_Timer->start(interval);
       ui->pushButton_4->setText("取消");
       ui->timeEdit_2->setEnabled(false);
    }else{
        timer->stop();
        ui->pushButton_4->setText("确认");
        ui->timeEdit_2->setEnabled(true);
    }
}
