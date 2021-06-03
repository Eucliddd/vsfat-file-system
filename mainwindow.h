#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "all.h"
#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    friend class myButton;
    void contextMenuEvent(QContextMenuEvent *event);
    QMenu *m_menu;
    void flash();
    void initTree();
private slots:
    void on_flash_clicked();
    void actionNewFolder();
    void actionNewFile();
    void actionShowDisk();
    void actionPot();
    void actionFormat();
    void actionChdir();
    void actionChpass();
    void on_saveall_clicked();
    void actionPaste();
    void actionHardLink();
    void actionSoftLink();
    void on_pushButton_clicked();

    //void on_flash_clicked(bool checked);

    void on_MainWindow_destroyed();

private:
    Ui::MainWindow *ui;
    QAction *newFolder;
    QAction *newFile;
    QAction *showDisk;
    QAction *pot;
    QAction *format;
    QAction *chdir;
    QAction *chpass;
    QAction *paste;
    QAction *hlink;
    QAction *slink;
};
#endif // MAINWINDOW_H
