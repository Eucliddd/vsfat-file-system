#ifndef POTENTIAL_FIGURE_H
#define POTENTIAL_FIGURE_H
#include "all.h"
#include <QDialog>

namespace Ui {
class Potential_figure;
}

class Potential_figure : public QDialog
{
    Q_OBJECT

public:
    explicit Potential_figure(QWidget *parent = nullptr);
    ~Potential_figure();

private slots:
    void showTable();

private:
    Ui::Potential_figure *ui;
};

#endif // POTENTIAL_FIGURE_H
