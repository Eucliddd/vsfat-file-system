#ifndef ACC_H
#define ACC_H
#include "all.h"
#include <QDialog>

namespace Ui {
class Acc;
}

class Acc : public QDialog
{
    Q_OBJECT
    int id;
public:
    explicit Acc(int id,QWidget *parent = nullptr);
    ~Acc();

private slots:
    void on_BaddW_clicked();
    void on_BaddR_clicked();
    void on_BdelR_clicked();
    void on_BdelW_clicked();

    void on_save_clicked();

private:
    Ui::Acc *ui;
    vector<string> read;
    vector<string> write;
    void show();
};

#endif // ACC_H
