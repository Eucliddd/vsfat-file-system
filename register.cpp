#include "register.h"
#include "ui_register.h"
#include "QMessageBox.h"
#include <fstream>
#include <iostream>
#include <all.h>
#include "all.h"
extern FILE_SYSTEM System;
using namespace std;
Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    //添加背景图片
    QDir temDir(":/pic/register_icon.jpg");
    QString filePath = temDir.canonicalPath();
    this->setWindowIcon(QIcon(filePath));
    ui->userLineEdit->setFocus();
    QPalette PAllbackground = this->palette();
    QDir temDir2(":/pic/register.jpg");
    QString filePath2 = temDir2.canonicalPath();
    QImage ImgAllbackground(filePath2);
    QImage fitimgpic=ImgAllbackground.scaled(this->width(),this->height(), Qt::IgnoreAspectRatio);
    PAllbackground.setBrush(QPalette::Window, QBrush(fitimgpic));
    this->setPalette(PAllbackground);

}

Register::~Register()
{
    delete ui;
}

bool Register::is_repeat(string str)
{
    int flag = 0;
    string user, pwd;
    ifstream read;
    QDir temDir("./user_pwd.txt");
    QString filePath = temDir.absolutePath();
    read.open(filePath.toStdString(), ios_base::in);
    while(read >> user >> pwd)
        if(str == user)
        {
            flag = 1;
            break;
        }
    read.close();
    if(flag)
        return true;
    else
        return false;
}

bool Register::is_right(string user,string pwd,string pwd_2)
{
    if(user.length() >= 2 && user.length() <= 10
               && pwd.length() >= 6 && pwd.length() <= 15
                  &&pwd_2.length() >= 6 && pwd_2.length() <= 15
                    &&pwd == pwd_2)
        return true;
    else
        return false;

}
void Register::on_submit_clicked()
{
    string user, pwd, pwd_2;
    user = ui->userLineEdit->text().toStdString();
    pwd = ui->pwdLineEdit->text().toStdString();
    pwd_2 = ui->pwdLineEdit_2->text().toStdString();
        if( !is_repeat(user) && is_right(user, pwd, pwd_2))
        {
            QMessageBox::question(this, "提示", "注册成功，进入登录界面", QMessageBox::Yes);
            //保存注册信息
            ofstream write;
            write.open(":/user_pwd.txt",ios::app);
            write  << user << " " << pwd << endl;
            write.close();
            //返回登录界面
            System.writelog(user+"register success");
            this->close();
        } else {
            //警告窗口
            if(is_repeat(user))
                QMessageBox::warning(this, tr("警告"),
                        tr("用户名已存在！"),
                        QMessageBox::Yes);
            else
                QMessageBox::warning(this, tr("警告"),
                       tr("用户名或密码不符合规范!"),
                       QMessageBox::Yes);
            //清空内容
            ui->pwdLineEdit->clear();
            ui->pwdLineEdit_2->clear();
            if(is_repeat(user))
            {
                ui->userLineEdit->clear();
                ui->userLineEdit->setFocus();
            }
            else
                ui->pwdLineEdit->setFocus();
        }
}
