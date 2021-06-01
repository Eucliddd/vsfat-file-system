#ifndef CHANGEPASS_H
#define CHANGEPASS_H

#include <QDialog>

namespace Ui {
class Changepass;
}

class Changepass : public QDialog
{
    Q_OBJECT

public:
    explicit Changepass(QWidget *parent = nullptr);
    ~Changepass();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Changepass *ui;
};

#endif // CHANGEPASS_H
