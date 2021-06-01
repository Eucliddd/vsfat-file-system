#ifndef PIECHART_H
#define PIECHART_H
#include "all.h"
#include <QDialog>

namespace Ui {
class piechart;
}

class piechart : public QDialog
{
    Q_OBJECT

public:
    explicit piechart(QWidget *parent = nullptr);
    ~piechart();

private slots:
    void drawChart();

private:
    Ui::piechart *ui;
};

#endif // PIECHART_H
