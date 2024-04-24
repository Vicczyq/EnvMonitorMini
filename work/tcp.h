#ifndef TCP_H
#define TCP_H

#include <QWidget>
namespace Ui {
class TCP;
}

class TCP : public QWidget
{
    Q_OBJECT
public:
    explicit TCP(QWidget *parent = 0);
    ~TCP();
    void add_log(QString str);
    void add_newconnection();
    void disconnection();
private slots:
    void on_clear_log_clicked();

private:
    Ui::TCP *ui;
    int tel_count;
};

#endif // TCP_H
