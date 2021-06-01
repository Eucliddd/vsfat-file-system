#ifndef TEXT_H
#define TEXT_H
#include "all.h"
#include <QDialog>
namespace Ui {
class Text;
}

class Text : public QDialog
{
    Q_OBJECT
private:
    bool create=false;
public:
    explicit Text(int id,int editable=1,QWidget *parent = nullptr);
    explicit Text(int id,int editable,bool create,QWidget *parent= nullptr);
    ~Text();
    void set(QString a);
    int id;
private slots:
    void on_save_clicked();

private:
    Ui::Text *ui;
};

#endif // TEXT_H
