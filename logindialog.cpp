#include "logindialog.h"
#include "ui_logindialog.h"
#include "QMessageBox.h"
#include "register.h"
#include <fstream>
#include <all.h>
extern FILE_SYSTEM System;
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    QDir temDir("../login/pic/login_icon.png");
    QString filePath = temDir.absolutePath();
    this->setWindowIcon(QIcon(filePath));
    //添加背景图片
    QPalette PAllbackground = this->palette();
    QDir temDir2("../login/pic/login.jpg");
    QString filePath2 = temDir2.absolutePath();
    QImage ImgAllbackground(filePath2);
    QImage fitimgpic=ImgAllbackground.scaled(this->width(),this->height(), Qt::IgnoreAspectRatio);
    PAllbackground.setBrush(QPalette::Window, QBrush(fitimgpic));
    this->setPalette(PAllbackground);

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

bool LoginDialog::is_success(string u, string p)
{
    int flag=0;//密码不匹配为0
    string ruser,rpwd;
    ifstream read;
    QDir temDir("../login/user_pwd.txt");
    QString filePath = temDir.absolutePath();
    read.open(filePath.toStdString(), ios_base::in);
    while(read >> ruser >> rpwd)
        if(u == ruser && p == rpwd)
        {
            flag = 1;
            System.username=ruser;
            System.writelog(u+"login in");
            break;
        }
    read.close();
    if(flag)
        return true;
    else
        return false;
}

void LoginDialog::on_loginBtn_clicked()
{
    user = ui->userLineEdit->text().toStdString();
    pwd = ui->pwdLineEdit->text().toStdString();
    if(is_success(user,pwd))
    {
        //进入主窗口
        accept();
    } else {
        //警告窗口
        QMessageBox::warning(this, tr("警告"),
                tr("用户名或密码输入错误!"),
                QMessageBox::Yes);
        //清空内容
        ui->userLineEdit->clear();
        ui->pwdLineEdit->clear();
        ui->userLineEdit->setFocus();
    }
}

void LoginDialog::on_registered_clicked()
{
    r= new Register();
    r->exec();
}
