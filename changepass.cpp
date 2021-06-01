#include "changepass.h"
#include "ui_changepass.h"
#include "all.h"
extern FILE_SYSTEM System;
Changepass::Changepass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Changepass)
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

Changepass::~Changepass()
{
    delete ui;
}

void Changepass::on_pushButton_clicked()
{
    string old_pass=ui->lineEdit->text().toStdString();
    string new_pass=ui->lineEdit_2->text().toStdString();
    string new_pass2=ui->lineEdit_3->text().toStdString();
    map <string, string> up;
    string u, p;
    vector <string> str;

    QDir temDir("../login/user_pwd.txt");
    QString filePath = temDir.absolutePath();

    if (new_pass != new_pass2)
    {
        //警告窗口
        QMessageBox::warning(this, tr("警告"),
                tr("两次密码不一致!"),
                QMessageBox::Yes);
        //清空内容
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_2->setFocus();
    }
    if (old_pass == new_pass)
    {
        //警告窗口
        QMessageBox::warning(this, tr("警告"),
                tr("新密码不能与原密码相同!"),
                QMessageBox::Yes);
        //清空内容
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_2->setFocus();
    }
    ifstream read;
    read.open(filePath.toStdString(),ios_base::in);
    while (read >> u >> p)
    {
        up[u] = p;
        str.push_back(u);
    }
    read.close();
    cout<<"old "<<old_pass<<"usepass "<<up[System.username]<<endl;
    if (old_pass != up[System.username])
    {
        //警告窗口
        QMessageBox::warning(this, tr("警告"),
                tr("原密码错误!"),
                QMessageBox::Yes);
        //清空内容
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit->setFocus();
        up.erase(up.begin(), up.end());//清空up中元素
        str.erase(str.begin(), str.end());//清空str中元素，方便下次使用
    }
    up[System.username] = new_pass;
    ofstream write;
    write.open(filePath.toStdString());
    for (size_t i = 0; i < str.size(); i++)
        write << str[i] << " " << up[str[i]] << endl;
    write.close();
    up.erase(up.begin(), up.end());//清空up中元素
    str.erase(str.begin(), str.end());//清空str中元素，方便下次使用
    QMessageBox::question(this, " 提示 ",
                 " 修改成功，进入主界面 ", QMessageBox::Yes);
    this->close();
}
