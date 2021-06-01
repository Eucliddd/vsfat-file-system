#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>

using namespace std;
namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private slots:

    bool is_repeat(string str);
    bool is_right(string str1,string str2,string str3);
    void on_submit_clicked();

private:
    Ui::Register *ui;
};

#endif // REGISTER_H
