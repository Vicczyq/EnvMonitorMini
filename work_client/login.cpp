#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("userinfo.sqlite");

    if (!db.open()) {
        qDebug() << "Error: Failed to connect database." << db.lastError();
    } else {
        qDebug() << "Database connected successfully.";
    }
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS users (username TEXT, password TEXT)")) {
        qDebug() << "Error: Failed to create table." << query.lastError().text();
    }
}

Login::~Login()
{
    delete ui;
}


void Login::on_loginButton_clicked()
{
    QString uid = ui->lineEdit->text();
    QString passwd = ui->lineEdit_2->text();
    if(!uid.size() || !passwd.size()){
        QMessageBox::warning(this,"错误","账号密码不可为空！",QMessageBox::Ok);
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", uid);
    query.bindValue(":password", passwd);

    if (query.exec()) {
        if (query.next()) {
            qDebug() << "Login successful.";
            // 登录成功后进行相应的操作
            emit loginSuccess();
            this->close();
        } else {
            qDebug() << "Username or password incorrect.";
            // 用户名或密码不正确的处理
            QMessageBox::warning(this, "错误", "用户名或密码不正确，请重试。", QMessageBox::Ok);
        }
    } else {
        qDebug() << "Query execution error:" << query.lastError().text();
        QMessageBox::warning(this, "错误", query.lastError().text(), QMessageBox::Ok);
    }
}

void Login::on_registerButton_clicked()
{
    QString uid = ui->lineEdit->text();
    QString passwd = ui->lineEdit_2->text();
    if(!uid.size() || !passwd.size()){
        QMessageBox::warning(this,"错误","账号密码不可为空！",QMessageBox::Ok);
        return;
    }
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM users WHERE username = :username");
    checkQuery.bindValue(":username", uid);
    if (checkQuery.exec() && checkQuery.next()) {
        qDebug() << "Username already exists.";
        QMessageBox::warning(this, "错误", "用户名已经存在，请选择一个不同的用户名。", QMessageBox::Ok);
        return;
    }

    // 插入新用户数据到数据库
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    insertQuery.bindValue(":username", uid);
    insertQuery.bindValue(":password", passwd);

    if (insertQuery.exec()) {
        qDebug() << "Registration successful.";
        QMessageBox::information(this, "成功", "注册成功！", QMessageBox::Ok);
    } else {
        qDebug() << "Registration failed:" << insertQuery.lastError().text();
        QMessageBox::critical(this, "错误", "注册失败，请重试。", QMessageBox::Ok);
    }
}
