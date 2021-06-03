#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QMenu>
#include <QStandardItemModel>
#include "chdir.h"
#include "changepass.h"

extern FILE_SYSTEM System;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setContextMenuPolicy(Qt::DefaultContextMenu);
    m_menu = new QMenu(this);
    newFolder = new QAction(this);
    newFile = new QAction(this);
    showDisk = new QAction(this);
    pot = new QAction(this);
    format = new QAction(this);
    chdir = new QAction(this);
    chpass = new QAction(this);
    paste = new QAction(this);
    hlink = new QAction(this);
    slink = new QAction(this);

    newFolder->setText("new Folder");
    newFile->setText("new File");
    showDisk->setText("show Disk");
    pot->setText(tr("bitmap"));
    format->setText("format");
    chdir->setText("change Directory");
    chpass->setText("change Password");
    paste->setText("paste");
    hlink->setText("hard link");
    slink->setText("soft link");

    m_menu->addAction(newFolder);
    m_menu->addAction(newFile);
    m_menu->addAction(showDisk);
    m_menu->addAction(pot);
    m_menu->addAction(format);
    m_menu->addAction(chdir);
    m_menu->addAction(chpass);
    m_menu->addAction(paste);
    m_menu->addAction(hlink);
    m_menu->addAction(slink);

    connect(newFolder, SIGNAL(triggered()), this, SLOT(actionNewFolder()));
    connect(newFile, SIGNAL(triggered()), this, SLOT(actionNewFile()));
    connect(showDisk, SIGNAL(triggered()), this, SLOT(actionShowDisk()));
    connect(pot, SIGNAL(triggered()), this, SLOT(actionPot()));
    connect(format, SIGNAL(triggered()), this, SLOT(actionFormat()));
    connect(chdir, SIGNAL(triggered()), this, SLOT(actionChdir()));
    connect(chpass, SIGNAL(triggered()), this, SLOT(actionChpass()));
    connect(paste, SIGNAL(triggered()), this, SLOT(actionPaste()));
    connect(hlink, SIGNAL(triggered()), this, SLOT(actionHardLink()));
    connect(slink, SIGNAL(triggered()), this, SLOT(actionSoftLink()));

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    on_saveall_clicked();//保存
    delete ui;
}
void MainWindow::actionFormat(){
    System.format();
    System.init();
    this->initTree();
    ui->flash->click();
    System.writelog("format disk");
}

//刷新界面
void MainWindow::on_flash_clicked()
{
    QLayout* pLayout = (QLayout*)ui->box;
    QLayoutItem *child;
    //清空组件
    while(child = pLayout->takeAt(0))
    {
        QWidget* pWidget = child->widget();
        pWidget->deleteLater();//延迟删除
        delete child;//删除组件
    }

    cout<<"flash clicked\n";
    //添加文件夹显示
    vector<string> nameList=System.getFile(1);
    ui->box->setRowStretch(5,10);
    int tot=nameList.size();
    int pos=0;
    for(int i=0;pos<tot;i++,pos++){
        myButton *p=new myButton(nameList[i],1,this);
        ui->box->addWidget(p,pos/5*2,pos%5);
        connect(p, &QPushButton::clicked, this, [=](){
            System.cd(p->name);
            flash();
        });
        QLabel *label;
        if(nameList[i]!="root"&&i!=0) label=new QLabel(QString::fromStdString(nameList[i]));
        else label=new QLabel(QString::fromStdString(".."));//上级目录特殊显示
        label->setAlignment(Qt::AlignHCenter);
        label->setFixedSize(50,50);
        ui->box->addWidget(label,pos/5*2+1,pos%5);

    }
    //添加文件显示
    nameList=System.getFile(0);
    cout<<"filelist:"<<nameList.size()<<endl;
    for(int i=0;i<nameList.size();i++) cout<<nameList[i]<<" ";
    cout<<endl;
    tot+=nameList.size();

    for(int i=0;pos<tot;i++,pos++){
        cout<<"new button:"<<nameList[i]<<endl;
        myButton *p=new myButton(nameList[i],0,this);
        ui->box->addWidget(p,pos/5*2,pos%5);
        connect(p, &QPushButton::clicked, this, [=](){
            p->actionOpen();
            flash();
        });
        QLabel *label=new QLabel(QString::fromStdString(nameList[i]));
        cout<<"tmp:"<<nameList[i]<<endl;
        label->setAlignment(Qt::AlignHCenter);
        label->setFixedSize(50,15);
        ui->box->addWidget(label,pos/5*2+1,pos%5);
    }
    //显示路径
    QString path="/root";
    for(int i=0;i<System.path.size();i++){
        if(i==0&&System.path[i]=="root")
            continue;
        path.push_back('/');
        path+=QString::fromStdString(System.path[i]);
    }
    this->initTree();
    ui->path->setText(path);
    this->initTree();
}

//右键槽函数
void  MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    m_menu->exec(QCursor::pos());
}


void MainWindow::actionNewFolder()
{
    System.writelog("NewFolder");
    NewFolder w;
    w.exec();
    flash();
    cout<<"newfloder"<<endl;
    qDebug() << "actionOne";

}

void MainWindow::actionNewFile()
{
    Text w(-1,true,true);
    w.exec();
    this->flash();
    qDebug() << "actionTwo";

}

void MainWindow::actionShowDisk()
{
    piechart w;
    qDebug() << "actionThree";

}

void MainWindow::actionPot(){
    Potential_figure w;
}

void MainWindow::flash(){
    ui->flash->click();
    initTree();
}

void MainWindow::initTree()
{

    QStandardItemModel *goodsModel = new QStandardItemModel(0, 1,this);
    goodsModel->setHeaderData(0, Qt::Horizontal, tr("Quick Access"));



    queue <int> q;//文件夹id队列
    QList<QStandardItem *> items;//文件夹对应的Item队列
    int itemsIndex = 0;//当前文件夹在items对应的index

    items.push_back(new QStandardItem("root"));
    q.push(0);
    QStandardItem *item;
    while(!q.empty())
    {
        int front = q.front();

        //加入子文件夹
        for(int i = 1;i < System.Folder[front].Folder_list.size();i++)
        {
            int floderId=System.Folder[front].Folder_list[i];
            q.push(floderId);

            item = new QStandardItem(QString::fromStdString(System.Folder[floderId].Name));
            QDir temDir("../login/pic/folder.png");
            QString filePath = temDir.absolutePath();
            item->setIcon( QIcon(filePath));//设置文件夹的图片

            items.push_back(item);
            items[itemsIndex]->appendRow(item);
        }
        //加入子文件
        vector<int> t = System.Folder[front].File_list;
        for(int b = 0;b < System.Folder[front].File_list.size();b++)
        {
            QStandardItem *item = new QStandardItem(QString::fromStdString(System.Files[t[b]].Name));
            cout<<"name --:"<<System.Files[t[b]].Name<<endl;
            QDir temDir("../login/pic/txt.png");
            QString filePath = temDir.absolutePath();
            item->setIcon( QIcon(filePath));//设置文件的图
            items[itemsIndex]->appendRow(item);
        }
        itemsIndex++;
        q.pop();
    }

    QStandardItem *tmp = items[0];
    goodsModel->appendRow(tmp);
    ui->treeView->setModel(goodsModel);
}

void MainWindow::on_saveall_clicked()
{
    System.writelog("save disk");
    System.Super.save();
    System.saveFolder();
}

void MainWindow::on_pushButton_clicked()
{
    System.writelog("log out");
    LoginDialog dlg;
    this->hide();
    if(dlg.exec() == QDialog::Accepted)
    {
        this->flash();
        this->show();
    }
}

void MainWindow::actionChdir()
{
    Chdir c;
    c.exec();
    this->flash();
}

void MainWindow::actionPaste(){
    if(System.clipBoard==-1){
        return;
    }
    string fname=System.Files[System.clipBoard].Name;
    //当前文件夹下有重名文件，即不能本地粘贴
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
    System.Files[new_id].Data=System.Files[System.clipBoard].Data;
    System.Files[new_id].save();
    //    cout<<"file save:\n filename:"<<fname<<" "<<"DAta:"<<System.Files[id].Data<<endl;
    //    accept();
    this->flash();
    return;
}

void MainWindow::actionChpass()
{
    System.writelog("change password");
    Changepass cp;
    cp.exec();
}

void MainWindow::actionHardLink(){
    if(System.clipBoard==-1){
        return;
    }
    string fname=System.Files[System.clipBoard].Name;
    //当前文件夹下有重名文件，即不能建立硬链接
    for(int i=0;i<System.Folder[System.Cur_folder].File_list.size();i++){
        int id=System.Folder[System.Cur_folder].File_list[i];
        if(System.Files[id].Name==fname){
            QMessageBox::warning(this, tr("Waring"),tr("有重名文件!"),QMessageBox::Yes);
            return;
        }
    }
    System.Folder[System.Cur_folder].File_list.push_back(System.clipBoard);
    //链接次数++
    System.Files[System.clipBoard].linkTimes++;
    this->flash();
    return;
}

void MainWindow::actionSoftLink(){
    if(System.clipBoard==-1){
        return;
    }
    string fname=System.Files[System.clipBoard].Name+".ink";
    //检测重名
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
    System.Files[new_id].Data=to_string(System.clipBoard);
    System.Files[new_id].save();
    //    cout<<"file save:\n filename:"<<fname<<" "<<"DAta:"<<System.Files[id].Data<<endl;
    //    accept();
    this->flash();
    return;
}

void MainWindow::on_MainWindow_destroyed()
{
    this->on_saveall_clicked();
}

