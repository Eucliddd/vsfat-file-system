#include "acc.h"
#include "all.h"
#include "ui_acc.h"
#include <QListWidget>
#include <QListWidgetItem>
extern FILE_SYSTEM System;
Acc::Acc(int id,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Acc)
{
    ui->setupUi(this);
    this->id=System.Files[id].Acc_disk;
    ui->owner->setPlainText(QString::fromStdString(System.Acces.owner));
    for(int i=0;i<System.Acces.Read_User.size();i++){
        read.push_back(System.Acces.Read_User[i]);
    }
    for(int i=0;i<System.Acces.Write_User.size();i++){
        write.push_back(System.Acces.Write_User[i]);
    }
    show();
}

Acc::~Acc()
{
    delete ui;
}
void Acc::show(){
    ui->read->clear();
    ui->write_2->clear();
//    ui->read->reset();
//    ui->write_2->reset();
    cout<<"clear"<<endl;
    for(int i=0;i<read.size();i++){
        ui->read->addItem(QString::fromStdString(read[i]));
    }
    for(int i=0;i<write.size();i++){
        ui->write_2->addItem(QString::fromStdString(write[i]));
    }
}

void Acc::on_BaddW_clicked()
{
    QString a=ui->addW->toPlainText();
    if(a=="") return;
    write.push_back(a.toStdString());
    show();
}

void Acc::on_BaddR_clicked()
{
    QString a=ui->addR->toPlainText();
    ui->addR->clear();
    if(a=="") return;
    read.push_back(a.toStdString());
    show();
}

void Acc::on_BdelR_clicked()
{
    string a=ui->delR->toPlainText().toStdString();
    for(int i=0;i<read.size();i++){
        if(a==read[i]){
            read.erase(read.begin()+i);
            i--;
        }
   }
    show();
}

void Acc::on_BdelW_clicked()
{
    string a=ui->delW->toPlainText().toStdString();
    for(int i=0;i<write.size();i++){
        if(a==write[i]){
            write.erase(write.begin()+i);
            i--;
        }
   }
    show();
}

void Acc::on_save_clicked()
{
    System.Acces.init(ui->owner->toPlainText().toStdString());
    cout<<ui->read->count()<<endl;
    for(int i=0;i<read.size();i++){
        System.Acces.Read_User.push_back(read[i]);
    }
    for(int i=0;i<write.size();i++){
        System.Acces.Write_User.push_back(write[i]);
    }
    cout<<"finished"<<endl;
    System.Acces.save(id);
}
