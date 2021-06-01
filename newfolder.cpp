#include "newfolder.h"
#include "ui_newfolder.h"
extern FILE_SYSTEM System;
NewFolder::NewFolder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFolder)
{
    ui->setupUi(this);
}

NewFolder::~NewFolder()
{
    delete ui;
}


void NewFolder::on_pushButton_clicked()
{
    string str=ui->text->toPlainText().toStdString();
    System.newFolder(str);
    accept();

}
