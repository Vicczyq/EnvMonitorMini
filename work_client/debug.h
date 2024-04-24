#ifndef DEBUG_H
#define DEBUG_H

#include <QWidget>
#include <QString>
namespace Ui {
class Debug;
}

class Debug : public QWidget
{
    Q_OBJECT

public:
    explicit Debug(QWidget *parent = 0);
    ~Debug();
    void add_info_network(QString str);
    void add_info_serial(QString str);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Debug *ui;
};

#endif // DEBUG_H
