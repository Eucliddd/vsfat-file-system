#include "all.h"
FILE_SYSTEM System;
int main(int argc, char *argv[])
{
        QApplication a(argc, argv);
        MainWindow w;
//      QWidget w;
//      myButton b();
//      b.setText("Button");
//      w.show();

//    MainWindow w;
//        System.format();
//        cout<<1<<endl;
    System.init();
    w.initTree();
    LoginDialog dlg;
    if(dlg.exec() == QDialog::Accepted)
//    System.username="guanhuhao";
    {
        w.flash();
        w.show();
        return a.exec();
    }
//    else return 0;
//        w.show();
//    return a.exec();
}
