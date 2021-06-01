#ifndef NEWFOLDER_H
#define NEWFOLDER_H
#include "all.h"
#include <QDialog>

namespace Ui {
class NewFolder;
}

class NewFolder : public QDialog
{
    Q_OBJECT

public:
    explicit NewFolder(QWidget *parent = nullptr);
    ~NewFolder();

private slots:
    void on_pushButton_clicked();

private:
    Ui::NewFolder *ui;
};

#endif // NEWFOLDER_H
