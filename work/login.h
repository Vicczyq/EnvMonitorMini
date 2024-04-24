#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

signals:
    void loginSuccess();

private slots:
    void on_loginButton_clicked();

    void on_registerButton_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
