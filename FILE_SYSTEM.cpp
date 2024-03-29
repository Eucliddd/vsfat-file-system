#include "all.h"
#include <algorithm>
int FILES::Tot;
int FOLDER::Tot;

/*

>> add - 相对路径
>> 
*/
void FILE_SYSTEM::cd(string add) {
    int tar = search(add, 1);
    if (add == "..") {
        tar = Folder[Cur_folder].Folder_list[0];
    }
    if (tar == -1) {
        cout << "ERROR PATH!" << endl;
        return;
    }
    cout<<"cd: "<<"now:"<<Cur_folder<<" "<<"to:"<<tar<<" "<<"name:"<<add<<endl;
    if(path.size()&&tar==Folder[Cur_folder].Folder_list[0])
        path.pop_back();
    else if(tar!=0)
        path.push_back(Folder[tar].Name);
    Cur_folder = tar;
    cout<<Cur_folder<<endl;
}

/*ls*/
void FILE_SYSTEM::ls() {
    cout << "Last Change Date |  File  Type  | Filename " << endl;
    for (int i = 0; i < Folder[Cur_folder].Folder_list.size(); i++) {
        if (i == 0) {
            cout << "---------- --:--" << " " << "|    Folder    |" << " " << ".." << endl;
            continue;
        }
        FOLDER p = Folder[Folder[Cur_folder].Folder_list[i]];
        cout << "---------- --:--" << " " << "|    Folder    |" << " " << p.Name << endl;
    }
    for (int i = 0; i < Folder[Cur_folder].File_list.size(); i++) {
        FILES p = Files[Folder[Cur_folder].File_list[i]];
        cout << p.Update_time << " " << "|    Files     |" << " " << p.Name << endl;
    }
}

/*

>> user_name - 用户名
>> filename - 文件名
*/
bool FILE_SYSTEM::fcreate(string user_name, string filename) {
    string mode = "WRITE";

    if (Files->Tot == MAXN_FILE){
        cout << "文件数量以达上限" << endl;
        return false;
    }
    if (iget(filename) != -1) //
    {
        cout << "文件名已存在" << endl;
        return false;
    }

    //

    string data=Files[Files->Tot].Data;
    int need = ceil(1.0*data.size() / DISK_SIZE);
    if (need + 1 >=Super.rest)
    {
        cout << "磁盘已满" << endl;
        return false;
    }
    int new_id;
    for (int i = 0; i < MAXN_FILE; i++) {  /*id*/
        if (!Files[i].isUsed()) {
            new_id = i;
            break;
        }
    }
    Files->Tot++;
    Files[new_id].id = new_id;
    Files[new_id].Name = filename;
    Files[new_id].linkTimes=1;
    Files[new_id].Update_time = getTime();
    Files[new_id].Has_load = true;
    Files[new_id].Is_update = true;
    Files[new_id].Acc_disk = Super.alloc();

    //
    for (int k = 0; k < need; k++) {
        int id = Super.alloc();
        Files[new_id].Disk_list.push_back(id);
        writeDisk(data.substr(k, DISK_SIZE), id);
    }
    stringstream ss;
    ss << 1 << " " << user_name << " " << 1<<" "<<user_name;
    writeDisk(ss.str(), Files[new_id].Acc_disk);
    //Folder[folder_id].addFile_Folder(Files[Files->Tot].id);
    cout << "" << endl;

}

/**/
void FILE_SYSTEM::loadFolder() {
    QDir temDir("../login/index.txt");
    QString filePath = temDir.absolutePath();
    fstream r(filePath.toStdString());
    int id, sub;
    /*
    >> r - 索引文件
    >> id - id
    >> sub - /id
    */
    qDebug()<<filePath;;
    r >> FOLDER::Tot;                     //
    int num;
    for (int i = 0; i < FOLDER::Tot; i++) {
        r >> id;

        r >> Folder[id].Name >> num;     //id,
        Folder[id].id = id;
        cout<<"Folder:"<<" "<<"id:"<<id<<" "<<"Name:"<<Folder[id].Name<<endl;
        for (int j = 0; j < num; j++) {
            r >> sub;                     //id
            Folder[id].Folder_list.push_back(sub);
        }
        r >> num;                      //
        for (int j = 0; j < num; j++) {
            r >> sub;                     //id
            Folder[id].File_list.push_back(sub);
        }
    }

    r >> FILES::Tot;
    for (int i = 0; i < FILES::Tot; i++) {
        r >> id;
        Files[id].id=id;
        r >>Files[id].Name>>Files[id].linkTimes>>Files[id].Update_time >>Files[id].Acc_disk >> num;
        cout<<"load file:\n";
        cout <<Files[id].Name<<" "<<Files[id].linkTimes<<" "<<Files[id].Update_time <<" "<<Files[id].Acc_disk <<" "<< num<<endl;;
        for (int j = 0; j < num; j++) {
            r >> sub;
            Files[id].Disk_list.push_back(sub);
        }
    }
    r.close();


}

/*保存index*/
void FILE_SYSTEM::saveFolder() {
    QDir temDir("../login/index.txt");
    QString filePath = temDir.absolutePath();
    ofstream w(filePath.toStdString());

    w << FOLDER::Tot << endl;
    for (int i = 0; i < MAXN_FOLDER; i++) {
        FOLDER p = Folder[i];
        if (!Folder[i].isUsed()) continue;
        w << p.id << " " << p.Name << " " << p.Folder_list.size();
        for (int i = 0; i < p.Folder_list.size(); i++) w << " " << p.Folder_list[i];
        w << " " << p.File_list.size();
        for (int i = 0; i < p.File_list.size(); i++) w << " " << p.File_list[i];
        w << endl;
    }
    w << FILES::Tot << endl;
    for (int i = 0; i < MAXN_FILE; i++) {
        if (!Files[i].isUsed()) continue;
        FILES p = Files[i];
        w << p.id << " " << p.Name << " " <<p.linkTimes<< " " <<p.Update_time <<" "<<p.Acc_disk<< " " << p.Disk_list.size();
        for (int j = 0; j < p.Disk_list.size(); j++) w << " " << p.Disk_list[j];
        w << endl;
    }
    w.close();
}


/*
>> filename 
*/
int FILE_SYSTEM::iget(string filename) {//λ
    for (int i = 0; i < Folder[Cur_folder].File_list.size(); i++)//
    {
        FILES p = Files[Folder[Cur_folder].File_list[i]];
        if (p.Name == filename)//
        {
            return i;//id
        }
    }
    return -1;
}


/**/
void FILE_SYSTEM::debug() {
    for (int i = 0; i < FOLDER::Tot; i++) {
        FOLDER p = Folder[i];
        cout << "filename:" << p.Name << " Folderid:" << p.id << endl;
        cout << "sub-Folder number:" << p.Folder_list.size() << endl;
        for (int j = 0; j < p.Folder_list.size(); j++) cout << p.Folder_list[j] << endl;
        cout << "sub-File number:" << p.File_list.size() << endl;
        for (int j = 0; j < p.File_list.size(); j++) cout << p.File_list[j] << endl;

    }
}


/*
/
>> name - 
>> is_Folder - 10
>> id
*/
int FILE_SYSTEM::search(string name, int is_Folder) {

    if (is_Folder) {
        for (int i = 0; i < Folder[Cur_folder].Folder_list.size(); i++) {
            FOLDER p = Folder[Folder[Cur_folder].Folder_list[i]];
            if (p.Name == name) return p.id;
        }
    }
    else {
        for (int i = 0; i < Folder[Cur_folder].File_list.size(); i++) {
            FILES p = Files[Folder[Cur_folder].File_list[i]];
            cout<<"search file:"<<p.Name<<" "<<name<<" "<<p.id<<endl;
            if (p.Name == name) return p.id;
        }
    }
    return -1;
}

/**/
void FILE_SYSTEM::init(){

    Cur_folder=0;
    loadFolder();
    QDir temDir("../login/disk.txt");
    QString filePath = temDir.absolutePath();
    ifstream r(filePath.toStdString());
    r>>Super.cur>>Super.rest;
    Super.load();
    qDebug()<<Super.cur<<endl;
}

void FILE_SYSTEM::exit(){
    saveFolder();
    Super.save();
}

void FILE_SYSTEM::format(){
    Super.format(1);
    for (int i=0;i<MAXN_FOLDER;i++){
        Folder[i].Tot=0;
        Folder[i].File_list.clear();
        Folder[i].Folder_list.clear();
    }
    QDir temDir("../login/index.txt");
    QString filePath = temDir.absolutePath();
    ofstream w(filePath.toStdString());

    w<<1<<endl<<0<<" "<<"root"<<" "<<1<<" "<<0<<" "<<0<<endl;
    w<<0<<endl;
    w.close();
    //     QDir dir;
    //    qDebug() <<  dir.currentPath() << endl;
}
/*
 * 获取当前文件夹下所有文件/子文件夹的名字
 * isFolder:是否为文件夹
 */
vector<string> FILE_SYSTEM::getFile(int isFolder){
    vector<string> ret;
    FOLDER p=Folder[Cur_folder];
    if(isFolder){
        for(int i=0;i<p.Folder_list.size();i++) ret.push_back(Folder[p.Folder_list[i]].Name);
    }else {
        for(int i=0;i<p.File_list.size();i++) {
            ret.push_back(Files[p.File_list[i]].Name);
        }
    }
    return ret;
}
/*
 * name:新建文件夹的名字
 * return:-1,文件夹名重复;-2,文件夹数量达到上限
 */
int FILE_SYSTEM::newFolder(string name){
    FOLDER p=Folder[Cur_folder];//当前文件夹下的子文件夹
    int flag=0;
    //检测重名
    for(int i=0;i<p.Folder_list.size();i++){
        int pp=p.Folder_list[i];
        if(Folder[pp].Name==name) {
            flag=1;
            break;
        }
    }
    //重名
    if(flag){
        return -1;
    }
    //文件夹的数量达到上限
    if(FOLDER::Tot==MAXN_FOLDER-1){
        return -2;
    }
    //插入文件夹数组并加入当前文件夹
    for(int i=0;i<MAXN_FOLDER;i++){
        if(Folder[i].isUsed()) continue;
        FOLDER::Tot++;
        cout<<"id:"<<i<<" "<<Cur_folder<<endl;
        Folder[Cur_folder].Folder_list.push_back(i);//加入当前文件夹
        Folder[i].Folder_list.push_back(Cur_folder);//新文件夹下加入上级文件夹
        Folder[i].Name=name;
        Folder[i].File_list.clear();
        Folder[i].id=i;
        return i;
    }
}

/*
 * name:文件夹的名字
 * isFolder:删除的是否未为文件夹
 * pos:当前文件夹的id
 */
void FILE_SYSTEM::delet(string name,int isFolder,int pos){
    if(isFolder){
        //
        for(int i=1;i<Folder[pos].Folder_list.size();i++){
            FOLDER f=Folder[Folder[pos].Folder_list[i]];
            cout<<"check folder:"<<name<<" "<<Folder[Folder[pos].Folder_list[i]].Name<<endl;
            if(name==Folder[Folder[pos].Folder_list[i]].Name) {
                cout<<"delete folder:"<<name<<endl;
                delet(name,isFolder,f.id);//递归删除
                Folder[pos].Folder_list.erase(Folder[pos].Folder_list.begin()+i);
                FOLDER::Tot--;
            }

        }

        if(name==Folder[pos].Name){
            while(!Folder[pos].File_list.empty()) {
                deletFile(pos,Files[Folder[pos].File_list.back()].Name);
            }
            while(Folder[pos].Folder_list.size()!=1){
                int back=Folder[pos].Folder_list.back();
                delet(Folder[back].Name,isFolder,back);
                Folder[pos].Folder_list.pop_back();
                cout<<"delete:"<<Folder[back].Name<<" "<<pos<<endl;
                FOLDER::Tot--;
            }
            Folder[pos].Folder_list.clear();//格式化清空
        }
    }else deletFile(pos,name);

}
//pos:文件所在的文件夹
void FILE_SYSTEM::deletFile(int pos,string name){
    int id=-1;//文件id
    int index=-1;
    for(int i=0;i<Folder[pos].File_list.size();i++){
        if(Files[Folder[pos].File_list[i]].Name==name){
            id = Folder[pos].File_list[i];
            index=i;
            break;
        }
    }
    if(id!=-1){//找到文件
        Files[id].linkTimes--;
        //从当前文件夹删除
        Folder[pos].File_list.erase(Folder[pos].File_list.begin()+index);
        //释放文件磁盘空间
        if(Files[id].linkTimes==0){
            FILES::Tot--;
            //释放控制信息块
            Super.free(Files[id].Acc_disk);
            //标记为未使用
            Files[id].Acc_disk=0;
            while(Files[id].Disk_list.size()){
                Super.free(Files[id].Disk_list.back());
                Files[id].Disk_list.pop_back();
            }
        }

    }
}

void FILE_SYSTEM::writelog(string op){

    QDir temDir("../login/log.txt");
    QString filePath = temDir.absolutePath();
    ofstream w;
    w.open(filePath.toStdString(),ios::app);

    string log=getTime()+" "+username+" " + op;
    w<<log<<endl;
    w.close();
}
