#ifndef MYBUTTON_H
#define MYBUTTON_H
#include "all.h"
#include <QtWidgets/QPushButton>
#include <QAction>
#include <QMenu>
#include <QDebug>
#include <bits/stdc++.h>
using namespace std;
class myButton : public QPushButton
{
    Q_OBJECT

public:
     myButton(string name,int isFolder,QWidget *parent = 0);
     ~myButton();
    string name;
    int isFolder;
    void contextMenuEvent(QContextMenuEvent *event);
    QMenu *m_menu;

public slots:
    void actionOpen();
    void actionDelete();
    void actionRename();
    void actionAcc();

private:
    QAction *open;
    QAction *delet;
    QAction *rename;
    QAction *acc;

};

#endif // MYBUTTON_H
