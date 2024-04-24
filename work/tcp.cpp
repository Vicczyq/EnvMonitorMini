#include "tcp.h"
#include "ui_tcp.h"

TCP::TCP(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TCP)
{
    ui->setupUi(this);
    tel_count = 0;
    setWindowTitle("通信日志");
}

TCP::~TCP()
{
    delete ui;
}

void TCP::add_log(QString str){
   ui->log->addItem(str);
   ui->log->scrollToBottom();
}

void TCP::on_clear_log_clicked()
{
    ui->log->clear();
}

void TCP::add_newconnection(){
    tel_count++;
    ui->connected_count->setText(QString::number(tel_count));
}

void TCP::disconnection(){
    tel_count--;
    ui->connected_count->setText(QString::number(tel_count));
}
