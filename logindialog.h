#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "register.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    string user, pwd;
    Register *r;
    bool is_success(string u,string p);

private slots:
    void on_loginBtn_clicked();

    void on_registered_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
