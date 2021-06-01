#include "piechart.h"
#include "ui_piechart.h"
#include "all.h"
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
extern FILE_SYSTEM System;
QT_CHARTS_USE_NAMESPACE
piechart::piechart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::piechart)
{
    ui->setupUi(this);
    this->drawChart();
}

piechart::~piechart()
{
    delete ui;
}

void piechart::drawChart()
{
    //获取饼图的值
    int Fonum = 0;//文件所占磁盘块数
    int Fpnum = 0;//文件权限占用磁盘块数
    int Bonum = 1;
    for(int i = 0;i < MAXN_FILE;i++)
    {
        if(System.Files[i].isUsed())
        {
            Fonum += System.Files[i].Disk_list.size();
            Fpnum ++;
        }
    }
    int Freenum = MAXN_DISK - Fonum - Fpnum - Bonum;
    //绘制饼图
    QPieSeries *series = new QPieSeries();
    //添加饼图切片的标签和值
    series->append("Free space", Freenum);
    series->append("File occupancy", Fonum);
    series->append("Permission occupancy", Fpnum);
    series->append("Boot sector", Bonum);
    float totalValue =MAXN_DISK;
    series->setHoleSize(0.0);//孔大小0.0-1.0
    series->setHorizontalPosition(0.5);//水平位置，默认0.5，0.0-1.0
    series->setLabelsPosition(QPieSlice::LabelOutside);
    series->setLabelsVisible(true);
    series->setPieSize(0.5);//饼图尺寸，默认0.7
    series->setPieStartAngle(0);//0度为12点钟方向
    series->setVerticalPosition(0.5);
    series->setVisible(true);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Disk space");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTheme(QChart::ChartThemeBlueCerulean);//设置系统主题
    chart->setAnimationOptions(QChart::AllAnimations);//设置启用或禁用动画
    chart->setLocalizeNumbers(true);//数字是否本地化

    chart->setTitleBrush(QBrush(QColor(255,255,255)));//设置标题Brush
    chart->setTitleFont(QFont("微软雅黑",20));//设置标题字体

    chart->legend()->setAlignment(Qt::AlignLeft);//对齐
    chart->legend()->setLabelColor(QColor(255,128,255));//设置标签颜色
    chart->legend()->setVisible(true);//设置是否可视
    chart->legend()->setBorderColor(QColor(255,255,170,185));//设置边框颜色
    QFont font = chart->legend()->font();
    font.setItalic(!font.italic());
    chart->legend()->setFont(font);//设置字体为斜体
    font.setPointSizeF(12);
    chart->legend()->setFont(font);//设置字体大小
    chart->legend()->setFont(QFont("微软雅黑"));//设置字体类型

    //操作单个切片
    QPieSlice *slice1 = series->slices().at(0);
    slice1->setExploded();//切片是否与饼图分离
    slice1->setLabelVisible(true);
    slice1->setLabelColor(QColor(170,255,170));
    slice1->setColor(QColor(170,255,170));//+QString("%1").arg(slice1->value())
    slice1->setLabel(slice1->label()+"-"+"-"+QString("%1").arg(slice1->value()/totalValue*100)+"%");
    slice1->setLabelFont(QFont("微软雅黑"));
    slice1->setLabelVisible(true);

    QPieSlice *slice2 = series->slices().at(1);
    slice2->setLabelVisible(true);
    slice2->setLabelColor(QColor(170,170,255));
    slice2->setColor(QColor(170,170,255));//QString("%1").arg(slice2->value())+
    slice2->setLabel(slice2->label()+"-"+"-"+QString("%1").arg(slice2->value()/totalValue*100.0)+"%");
    slice2->setLabelArmLengthFactor(0.15);//臂长不重叠
    slice2->setLabelFont(QFont("微软雅黑"));
    slice2->setLabelVisible(true);


    QPieSlice *slice3 = series->slices().at(2);
    slice3->setLabelVisible(true);
    slice3->setLabelColor(QColor(255,170,170));

    slice3->setColor(QColor(255,170,170));//+QString("%1").arg(slice3->value())
    slice3->setLabel(slice3->label()+"-"+"-"+QString("%1").arg(slice3->value()/totalValue*100)+"%");
    slice3->setLabelArmLengthFactor(0.3);
    slice3->setLabelFont(QFont("微软雅黑"));
    slice3->setLabelVisible(true);

    QPieSlice *slice4 = series->slices().at(3);
    slice3->setLabelVisible(true);
    slice4->setLabelColor(QColor(255,170,255));
    slice4->setColor(QColor(255,170,255));//+QString("%1").arg(slice4->value())
    slice4->setLabel(slice4->label()+"-"+"-"+QString("%1").arg(slice4->value()/totalValue*100)+"%");
    slice4->setLabelArmLengthFactor(0.55);
    slice4->setLabelFont(QFont("微软雅黑"));
    slice4->setLabelVisible(true);



    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(900,400);
    chartView->show();
}
