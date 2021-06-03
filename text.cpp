#include "text.h"
#include "ui_text.h"
extern FILE_SYSTEM System;
Text::Text(int id,int editable,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Text)
{
    this->id=id;
    ui->setupUi(this);
    ui->filename->setPlainText(QString::fromStdString(System.Files[id].Name));
    System.Files[id].load();
    ui->text->setPlainText(QString::fromStdString(System.Files[id].Data));
    if (!editable){
        cout<<"read only"<<endl;
        ui->filename->setReadOnly(true);
        ui->text->setReadOnly(true);
    }
}
Text::Text(int id,int editable,bool __,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Text)
{
    this->create=true;
    ui->setupUi(this);
}

Text::Text(int link,int source, bool __,int editable,QWidget *parent):
    QDialog(parent),
    ui(new Ui::Text)
{
    this->soft_link=source;
    ui->setupUi(this);
    this->id=link;
    ui->filename->setPlainText(QString::fromStdString(System.Files[link].Name));
    System.Files[source].load();
    ui->text->setPlainText(QString::fromStdString(System.Files[source].Data));
    if (!editable){
        cout<<"read only"<<endl;
        ui->filename->setReadOnly(true);
        ui->text->setReadOnly(true);
    }
}
Text::~Text(){
    delete ui;
}

void Text::set(QString a){
    ui->text->setText(a);
}

void Text::on_save_clicked()
{
    if(create){
        string fname=ui->filename->toPlainText().toStdString();
        if(fname=="") {
            QMessageBox::warning(this, tr("Waring"),
                                 tr("文件名不能为空!"),
                                 QMessageBox::Yes);
            return;
        }
        for(int i=0;i<System.Folder[System.Cur_folder].File_list.size();i++){
            int id=System.Folder[System.Cur_folder].File_list[i];
            if(System.Files[id].Name==fname){
                QMessageBox::warning(this, tr("Waring"),tr("有重名文件!"),QMessageBox::Yes);
                return;
            }
        }
        int new_id;
        for (int i = 0; i < MAXN_FILE; i++) {
            if (!System.Files[i].isUsed()) {
                new_id = i;
                break;
            }
        }
        System.Files->Tot++;
        System.Files[new_id].id = new_id;
        System.Files[new_id].Update_time = getTime();
        System.Files[new_id].Has_load = true;
        System.Files[new_id].Is_update = true;
        System.Files[new_id].Acc_disk = System.Super.alloc();
        System.Folder[System.Cur_folder].File_list.push_back(new_id);
        std::cout<<"newfolder:\ncurfolder"<<System.Cur_folder<<" cnt:"<<System.Folder[System.Cur_folder].File_list.size()<<endl;
        //控制信息初始化和持久化
        System.Acces.init(System.username);
        System.Acces.save(System.Files[new_id].Acc_disk);

        System.writelog("new file");
        System.Files[new_id].Name=fname;
        System.Files[new_id].linkTimes=1;
        System.Files[new_id].Data=ui->text->toPlainText().toStdString();
        System.Files[new_id].save();
        //cout<<"file save:\n filename:"<<fname<<" "<<"DAta:"<<System.Files[id].Data<<endl;
        accept();
        return;
    }

    if(soft_link!=-1){
        string fname=ui->filename->toPlainText().toStdString();
        if(fname=="") {
            QMessageBox::warning(this, tr("Waring"),
                                  tr("文件名不能为空!"),
                                  QMessageBox::Yes);
            return;
        }
        for(int i=0;i<System.Folder[System.Cur_folder].File_list.size();i++){
            int id=System.Folder[System.Cur_folder].File_list[i];
            if(id==this->id) continue;
            if(System.Files[id].Name==fname){
                QMessageBox::warning(this, tr("Waring"),tr("有重名文件!"),QMessageBox::Yes);
                return;
            }
        }
        System.Files[id].Name=fname;
        System.Files[soft_link].Data=ui->text->toPlainText().toStdString();
        //System.Files[id].save();
        System.Files[soft_link].save();
        accept();
        return;
    }
    cout<<"enter save"<<endl;
    string fname=ui->filename->toPlainText().toStdString();
    if(fname=="") {
        QMessageBox::warning(this, tr("Waring"),
                             tr("文件名不能为空!"),
                             QMessageBox::Yes);
        return;
    }
    for(int i=0;i<System.Folder[System.Cur_folder].File_list.size();i++){
        int id=System.Folder[System.Cur_folder].File_list[i];
        if(id==this->id) continue;
        if(System.Files[id].Name==fname){
            QMessageBox::warning(this, tr("Waring"),tr("有重名文件!"),QMessageBox::Yes);
            return;
        }
    }
    System.Files[id].Name=fname;
    System.Files[id].Data=ui->text->toPlainText().toStdString();
    System.Files[id].save();
    cout<<"file save:\n filename:"<<fname<<" "<<"DAta:"<<System.Files[id].Data<<endl;
    accept();
}
