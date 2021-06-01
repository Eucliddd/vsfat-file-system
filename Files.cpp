#include "all.h"
#include <QDialog>
extern FILE_SYSTEM System;
bool FILES::isUsed() {
    return Acc_disk != 0;
}

void FILES::load() {
    Data.clear();
    QDir temDir("./disk.txt");
    QString filePath = temDir.absolutePath();
    FILE * read = fopen(filePath.toStdString().c_str(), "r+");
    stringstream ss;
    char tmp[DISK_SIZE];
    for(int i=0;i<Disk_list.size();i++){
        fseek(read,DISK_SIZE*Disk_list[i],0);
        fread(tmp, DISK_SIZE, 1,read);
        //tmp[DISK_SIZE-1]='\0';
        if(tmp[DISK_SIZE-1]!='\0')
            Data.append(tmp,tmp+DISK_SIZE);
        else
            Data+=tmp;
        cout<<"read pos:"<<DISK_SIZE*Disk_list[i]<<endl;
        //cout<<"read con:"<<tmp<<" "<<Data.size()<<" "<<strlen(tmp)<<endl;
    }

    fclose(read);
}
int min(int a,int b){
    return a<b?a:b;
}

void FILES::save(){
    for(int i=0;i<Disk_list.size();i++)
        System.Super.free(Disk_list[i]);
    Disk_list.clear();

    int need = ceil(1.0*Data.size()/DISK_SIZE);
    if (need + 1 >=System.Super.rest)
    {
        cout << "磁盘已满" << endl;
        QMessageBox::warning(nullptr,QString("Waring"),QString("磁盘已满!"),QMessageBox::Yes);
        return;
    }
    for(int i=0;i<need;i++){
        int id=System.Super.alloc();
        Disk_list.push_back(id);
        writeDisk(Data.substr(i*DISK_SIZE,DISK_SIZE),id);
    }
}
