#include "debug.h"
#include "ui_debug.h"

Debug::Debug(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Debug)
{
    ui->setupUi(this);
}

Debug::~Debug()
{
    delete ui;
}

void Debug::add_info_network(QString str){
    ui->listWidget->addItem(str);
    ui->listWidget->scrollToBottom();
}

void Debug::add_info_serial(QString str){
    ui->listWidget_2->addItem(str);
    ui->listWidget_2->scrollToBottom();
}

void Debug::on_pushButton_clicked()
{
    ui->listWidget->clear();
}

void Debug::on_pushButton_2_clicked()
{
    ui->listWidget_2->clear();
}
