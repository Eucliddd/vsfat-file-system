#include "chdir.h"
#include "ui_chdir.h"
#include "all.h"
#include "mainwindow.h"
extern FILE_SYSTEM System;
Chdir::Chdir(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Chdir)
{
    ui->setupUi(this);
}

Chdir::~Chdir()
{
    delete ui;
}

void Chdir::on_pushButton_clicked()
{
    string str=ui->textEdit->toPlainText().toStdString();
    vector <string> cur;
    vector <string> cdo;
    string s = "";
    string t = "";
    for(size_t i = 0;i < str.size();i++)
    {
        if(str[i] == '/' or str[i] == '.')
        {
            t += str[i];
            if(s!="")
                cur.push_back(s);
            s="";
        }
        else
        {
            s += str[i];
            cdo.push_back(t);
            t="";
        }
    }
    if(t != "")
            cdo.push_back(t);
    if(s != "")
            cur.push_back(s);

    if(cdo[0] == "//")
        System.cd("root");
    else if(cdo[0] == "./"){}
    else if(cdo[0] == "../")
        System.cd("..");
    for(size_t i = 0;i < cur.size();i++)
        System.cd(cur[i]);
    accept();
}
