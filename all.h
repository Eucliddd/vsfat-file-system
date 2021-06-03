#pragma once
#define MAXN_DISK 128     //磁盘块数
#define DISK_SIZE 1024    //磁盘块大小
#define MAXN_FOLDER 1024  //最大目录数
#define MAXN_FILE 1024    //最大文件数
#define MAXN_ZU 8
#include <bits/stdc++.h>
#include <QDebug>
#include <QDir>
#include <QPushButton>
#include <QMenu>
#include <QApplication>
#include "mainwindow.h"
#include "logindialog.h"
#include "mybutton.h"
#include "newfolder.h"
#include "piechart.h"
#include "text.h"
#include "QMessageBox"
#include "potential_figure.h"
#include "acc.h"
#include "chdir.h"

using namespace std;

struct FOLDER {
    static int Tot;     /*Tot - 当前总文件目录数*/
    string Name;        /*Name - 当前文件目录名字*/
    int id;             /*id - 记录当前目录id*/

    vector<int> Folder_list;    /*当前目录的子目录，0号始终为上级*/
    vector<int> File_list;      /*当前目录的文件*/

    /*返回当前目录是否被用*/
    bool isUsed();
};

struct FILES {
    static int Tot;         /*当前文件总数*/
    string Name;            /*文件名*/
    int linkTimes;          /*文件链接次数*/
    string Update_time;         /*更新时间*/
    int id;                 /*文件id*/
    bool Has_load;          /*是否已读取标志位*/
    bool Is_update;         /*脏位标记*/
    string Data;            /*数据内容*/
    vector<int> Disk_list;  /*磁盘块列表*/
    int Acc_disk;			/*权限控制信息磁盘号*/

    bool isUsed();
    void load();
    void save();
};

struct SUPER {
    int rest;   /*剩余磁盘块数*/
    int cur;    /*当前超级块的磁盘号*/
    int next;   /*下一块超级块的磁盘号*/
    vector<int> Free_disk;  /*空闲磁盘队列*/

    /* 从cur块加载超级块
     * 返回-1：系统中没有可用的空闲磁盘
     * 成组链接，加载下一组超级块/系统启动，加载超级块
     */
    int  load();

    /*
     * 保存引导块以及当前的超级块，写回到对应磁盘
     * 系统退出，保存/成组链接，当前组已满，保存
     */
    void save();

    /*
     * 初始化整个磁盘以及超级块
     * isclean - 默认值为0 ;非0值则会将所有信息抹除后重新构建磁盘块管理
     */
    void format(int isclean=0);

    /*
     * 申请分配磁盘块
     * 返回值:为分配到的磁盘号，-1代表分配失败
     */
    int  alloc();

    /*
     * 释放指定磁盘块，加入到成组链接中
     * id:指定磁盘块号
     */
    void free(int id);


};

/*
 * 写磁盘
 * str:传入的需要写的字符串
 * id:需要写入的磁盘号
 */
void writeDisk(string str, int id);

/*获取当前时间 格式：年-月-日-时-分-秒*/
string getTime();

/*
 * 访问控制模块
 */
struct ACCESS {
    string owner;
    vector<string> Read_User;
    vector<string> Write_User;

    void load(int id);
    void save(int id);
    bool access(string uid, int fid, string mode);
    void init(string name="");
};

/*
 * 文件系统模块
 */
struct FILE_SYSTEM {
    int Cur_folder=0;
    string username;
    SUPER Super;
    FILES Files[MAXN_FILE];
    FOLDER Folder[MAXN_FOLDER];
    ACCESS Acces;
    //copy-paste
    int clipBoard=-1;

    vector<string> path;
    /*
     *目录跳转
     * add:相对目录地址
     */
    void cd(string add);

    /*
     * 显示目录
     */
    void ls();


    /*
     * 创建文件
     * user_name:用户名
     * filename:文件名
     */
    bool fcreate(string user_name, string filename);

    /*
     * 读取目录索引信息，保存在Folder中
     */
    void loadFolder();

    /*
     * 保存目录索引信息
     */
    void saveFolder();

    /*
     * debug用
     */
    void debug();


    /*
     * 获得当前文件夹下文件名对应的id
     * filename:当前目录下的文件名
     * 返回:文件id，-1：不存在该文件
     */
    int iget(string filename);

    /*
     * 从当前目录中查找指定文件/目录
     * name:文件名/目录名
     * is_Folder:1,目录名;0:文件名
     * 返回:文件id/目录id,-1:不存在
     */
    int  search(string name, int is_Folder);

    /*
     * 加载文件系统
     * 加载索引信息：Folder、Files,加载超级块：Super
     */
    void init();

    /*
     * 从文件系统安全退出
     * 保存索引信息：Folder、Files，保存引导块、超级块
     */
    void exit();

    /*
     * 格式化文件系统
     */
    void format();

    vector<string> getFile(int isFolder);

    int newFolder(string name);

    void delet(string name,int isFolder,int pos);

    void deletFile(int Folder,string name);

    void writelog(string op);
};

void flash();
