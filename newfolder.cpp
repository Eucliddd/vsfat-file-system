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
    if(str=="") {
        QMessageBox::warning(this, tr("Waring"),
                             tr("文件夹名不能为空!"),
                             QMessageBox::Yes);
        return;
    }
    int res=System.newFolder(str);
    if(res==-1){
        QMessageBox::warning(this, tr("Waring"),
                             tr("文件夹名重复!"),
                             QMessageBox::Yes);
        return;
    }
    if(res==-2){
        QMessageBox::warning(this, tr("Waring"),
                             tr("文件夹数量达到上限"),
                             QMessageBox::Yes);
        return;
    }
    accept();

}
