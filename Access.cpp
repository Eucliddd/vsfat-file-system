#include "all.h"
extern FILE_SYSTEM System;
/*
 * 访问控制信息初始化
 */
void ACCESS::init(string name){
    owner=name;
    Read_User.clear();
    Write_User.clear();
}
/*
 * 判断用户是否有权访问
 */
bool ACCESS::access(string username, int fid, string mode){
    int id=System.Files[fid].Acc_disk;
    load(id);
    bool find = false;
    cout<<"Access:"<<"\nfid:"<<id<<" username:"<<username<<" owner:"<<owner<<endl;
    if(username==owner) return true;//文件创建者拥有一切权限
    if(mode=="acc") return false;
    if (mode == "w")
        for (int i = 0; i < Write_User.size(); i++)
            if (Write_User[i] == username)
                find = true;
    if(mode == "r")
        for (int i = 0; i < Read_User.size(); i++)
            if (Read_User[i] == username)
                find = true;
    cout<<"Acc judge:"<<mode<<" "<<find<<endl;
    return find;//判断用户有无权限，后期加，先假设有
}

/*
 * 加载访问控制信息
 * id:访问控制信息所在块
 */
void ACCESS::load(int id) {
    //清空
    Read_User.clear();
    Write_User.clear();

    QDir temDir("./disk.txt");
    QString filePath = temDir.absolutePath();
    ifstream read(filePath.toStdString());
    read.seekg(id*DISK_SIZE);
    int cnt;
    string username;
    read >>owner;//文件创建者
    //加载读权限用户
    read >> cnt;
    for (int i = 0; i < cnt; i++) {
        read >> username;
        this->Read_User.push_back(username);
    }
    //加载写权限用户
    read >> cnt;
    for (int i = 0; i < cnt; i++) {
        read >> username;
        Write_User.push_back(username);
    }
}
/*
 * 保存访问控制信息
 * id:访问控制信息所在磁盘块
 */
void ACCESS::save(int id) {
    stringstream ss;
    ss<<owner<<" ";
    ss << Read_User.size()<<" ";

    for (int i = 0; i < Read_User.size(); i++) ss << Read_User[i]<<" ";
    ss << Write_User.size() << " ";   /* cout<<ss.str()<<endl;*/
    for (int i = 0; i < Write_User.size(); i++) ss << Write_User[i] << " ";
    string str;
    cout<<"Access write:"<<ss.str()<<" "<<"id:"<<id<<endl;
    writeDisk(ss.str(),id);
}
