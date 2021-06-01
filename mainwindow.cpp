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


    newFolder->setText("new Folder");
    newFile->setText("new File");
    showDisk->setText("show Disk");
    pot->setText(tr(" 位式图 "));
    format->setText("format");
    chdir->setText("change Directory");
    chpass->setText("change Password");

    m_menu->addAction(newFolder);
    m_menu->addAction(newFile);
    m_menu->addAction(showDisk);
    m_menu->addAction(pot);
    m_menu->addAction(format);
    m_menu->addAction(chdir);
    m_menu->addAction(chpass);

    connect(newFolder, SIGNAL(triggered()), this, SLOT(actionNewFolder()));
    connect(newFile, SIGNAL(triggered()), this, SLOT(actionNewFile()));
    connect(showDisk, SIGNAL(triggered()), this, SLOT(actionShowDisk()));
    connect(pot, SIGNAL(triggered()), this, SLOT(actionPot()));
    connect(format, SIGNAL(triggered()), this, SLOT(actionFormat()));
    connect(chdir, SIGNAL(triggered()), this, SLOT(actionChdir()));
    connect(chpass, SIGNAL(triggered()), this, SLOT(actionChpass()));

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    on_saveall_clicked();
    delete ui;
}
void MainWindow::actionFormat(){
    System.format();
    System.init();
    this->initTree();
    ui->flash->click();
    System.writelog("format disk");
}

void MainWindow::on_flash_clicked()
{
    QLayoutItem *child;
    QLayout* pLayout = (QLayout*)ui->box;
    while(child = pLayout->takeAt(0))
    {
        QWidget* pWidget = child->widget();
        pWidget->deleteLater();
        delete child;
    }

    cout<<"flash clicked\n";
    vector<string> tmp=System.getFile(1);
    ui->box->setRowStretch(5,10);
    int tot=tmp.size();
    int pos=0;
    for(int i=0;pos<tot;i++,pos++){
        myButton *p=new myButton(tmp[i],1,0);
        ui->box->addWidget(p,pos/5*2,pos%5);
        connect(p, &QPushButton::clicked, this, [=](){ //��������ʽ����ֱ����Ϊ�ۺ���
                    System.cd(p->name);
                    ui->flash->click();
         });
        QLabel *label;
        if(tmp[i]!="root"&&i!=0) label=new QLabel(QString::fromStdString(tmp[i]));
        else label=new QLabel(QString::fromStdString(".."));
        label->setAlignment(Qt::AlignHCenter);
        label->setFixedSize(50,50);
        ui->box->addWidget(label,pos/5*2+1,pos%5);

    }
    tmp=System.getFile(0);
    cout<<"filelist:"<<tmp.size()<<endl;
    for(int i=0;i<tmp.size();i++) cout<<tmp[i]<<" ";
    cout<<endl;
    tot+=tmp.size();

    for(int i=0;pos<tot;i++,pos++){
        cout<<"new button:"<<tmp[i]<<endl;
        myButton *p=new myButton(tmp[i],0,0);
        ui->box->addWidget(p,pos/5*2,pos%5);
        connect(p, &QPushButton::clicked, this, [=](){ //��������ʽ����ֱ����Ϊ�ۺ���
                    p->actionOpen();
        });
        QLabel *label=new QLabel(QString::fromStdString(tmp[i]));
        cout<<"tmp:"<<tmp[i]<<endl;
        label->setAlignment(Qt::AlignHCenter);
        label->setFixedSize(50,15);
        ui->box->addWidget(label,pos/5*2+1,pos%5);
    }
    QString path="/root";
    for(int i=0;i<System.path.size();i++){
        if(i==0&&System.path[i]=="root")
            continue;
        path.push_back('/');
        path+=QString::fromStdString(System.path[i]);
    }
    ui->path->setText(path);

}

void  MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
//    QMenu *menu = new QMenu(this);
//    menu->addAction(tr("rename"));
//    menu->addAction(tr("delete"));

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
    //����model
    QStandardItemModel *goodsModel = new QStandardItemModel(0, 1,this);
    goodsModel->setHeaderData(0, Qt::Horizontal, tr("Quick Access"));

    //��ʾĿ¼���ļ�
    queue <int> q;
    queue <int> s;
    QList<QStandardItem *> items;
    QStandardItem *item;
    vector <int> li;
    int m=0;
    /*root������*/
    QStandardItem *it = new QStandardItem("root");
    items.push_back(it);
    q.push(0);
    s.push(0);
    while(q.size())
    {
        int front = q.front();
        int last = s.front();
        for(int i = 1;i < System.Folder[front].Folder_list.size();i++)
        {
            li.push_back(System.Folder[front].Folder_list[i]);
            q.push(li[i-1]);
            item = new QStandardItem(QString::fromStdString(System.Folder[li[i-1]].Name));
            QDir temDir(":/pic/folder.png");
            QString filePath = temDir.canonicalPath();
            item->setIcon( QIcon(filePath));//�ǵø�·��������
            m++;
            s.push(m);
            QStandardItem *tmp = items[last];
            items.push_back(item);
            tmp->appendRow(item);
        }
        vector<int> t = System.Folder[front].File_list;
        for(int b = 0;b < System.Folder[front].File_list.size();b++)
        {
            QStandardItem *item = new QStandardItem(QString::fromStdString(System.Files[t[b]].Name));
            cout<<"name --:"<<System.Files[t[b]].Name<<endl;
            QDir temDir(":/pic/txt.png");
            QString filePath = temDir.canonicalPath();
            item->setIcon( QIcon(filePath));//�ǵø�·��������
            QStandardItem *lastItem =items[last];
            lastItem->appendRow(item);
        }
        q.pop();
        s.pop();
        li.clear();
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
}
void MainWindow::actionChpass()
{
    System.writelog("change password");
    Changepass cp;
    cp.exec();
}

void MainWindow::on_MainWindow_destroyed()
{
    this->on_saveall_clicked();
}

