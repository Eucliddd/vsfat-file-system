#include "mybutton.h"
extern FILE_SYSTEM System;
myButton::myButton(string name,int isFolder,QWidget *parent): QPushButton(parent)
{
    this->name=name;
    this->isFolder=isFolder;

    QDir temDir("../login/pic/folder.png");
    QDir temDir2("../login/pic/txt.png");
    QString filePath = temDir.absolutePath();

    if(isFolder)this->setIcon(QIcon( temDir.absolutePath()));
    else this->setIcon(QIcon( temDir2.absolutePath()));

    this->setFixedSize(50,50);
    this->setIconSize(QSize(50,50));

    setContextMenuPolicy(Qt::DefaultContextMenu);

    m_menu = new QMenu(this);
    open = new QAction(this);
    delet = new QAction(this);
    rename = new QAction(this);
    if(!isFolder) acc = new QAction(this);
    if(!isFolder) copy=new QAction(this);//构造复制按钮

    open->setText("open");
    delet->setText("delete");
    rename->setText("rename");
    if(!isFolder) acc->setText("权限管理");
    if(!isFolder) copy->setText("copy");

    m_menu->addAction(open);
    m_menu->addAction(delet);
    m_menu->addAction(rename);
    if(!isFolder) m_menu->addAction(acc);
    if(!isFolder) m_menu->addAction(copy);

    connect(open, SIGNAL(triggered()), this, SLOT(actionOpen()));
    connect(delet, SIGNAL(triggered()), this, SLOT(actionDelete()));
    connect(rename, SIGNAL(triggered()), this, SLOT(actionRename()));
    if(!isFolder) connect(acc, SIGNAL(triggered()), this, SLOT(actionAcc()));
    if(!isFolder) connect(copy,SIGNAL(triggered()),this,SLOT(actionCopy()));
}

myButton::~myButton()
{

}

void myButton::actionOpen()
{
    if (isFolder) System.cd(name);
    else {
        int id=System.search(name,0);
        if(not System.Acces.access(System.username,id,"r")){
            QMessageBox::warning(this, tr("Warning"),
                                 tr("无读取权限!"),
                                 QMessageBox::Yes);
            return;
        }
        cout<<"open file:"<<id<<endl;
        System.Files[id].load();
        Text w(id,System.Acces.access(System.username,id,"w"));
        w.exec();
    }
    System.writelog("open file "+name);
    qDebug() << "actionOne";
    //this->click();
}

void myButton::actionDelete()
{
    cout<<"check ghh:"<<endl;
    int id=System.search(name,0);
    cout<<"check ghh: "<<id<<endl;
    if(not isFolder &&not System.Acces.access(System.username,id,"w")){
        QMessageBox::warning(this, tr("Warning"),
                             tr("无修改权限!"),
                             QMessageBox::Yes);
        return;
    }

    System.delet(name,isFolder,System.Cur_folder);
    System.writelog("delete file "+name);
    qDebug() << "actionTwo";
    this->click();
}

void myButton::actionRename()
{
    int id=System.search(name,0);
    if(not System.Acces.access(System.username,id,"w")){
        QMessageBox::warning(this, tr("Warning"),
                             tr("无修改权限!"),
                             QMessageBox::Yes);
        return;
    }
    System.writelog("rename file "+name);
    qDebug() << "actionThree";
    this->actionOpen();
    this->click();
}

void myButton::actionAcc(){
    int id=System.search(name,0);
    System.Acces.load(id);
    if(not System.Acces.access(System.username,id,"acc")){
        QMessageBox::warning(this, tr("Warning"),
                             tr("无所有权限!"),
                             QMessageBox::Yes);
        return;
    }
    System.writelog("change access "+name);
    cout<<"ACC:"<<System.Acces.owner<<endl;
    Acc w(id);
    w.exec();
}

void myButton::actionCopy(){
    int id=System.search(name,0);
    System.clipBoard=id;
}

void  myButton::contextMenuEvent(QContextMenuEvent *event)
{
    //    QMenu *menu = new QMenu(this);
    //    menu->addAction(tr("rename"));
    //    menu->addAction(tr("delete"));

    m_menu->exec(QCursor::pos());
}

