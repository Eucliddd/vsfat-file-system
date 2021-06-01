#ifndef CHDIR_H
#define CHDIR_H

#include <QDialog>

namespace Ui {
class Chdir;
}

class Chdir : public QDialog
{
    Q_OBJECT

public:
    explicit Chdir(QWidget *parent = nullptr);
    ~Chdir();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Chdir *ui;
};

#endif // CHDIR_H
