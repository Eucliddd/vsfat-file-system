#include "potential_figure.h"
#include "ui_potential_figure.h"
#include <QStandardItem>
#include "all.h"
extern FILE_SYSTEM System;
Potential_figure::Potential_figure(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Potential_figure)
{
    ui->setupUi(this);
    showTable();
}

Potential_figure::~Potential_figure()
{
    delete ui;
}

void Potential_figure::showTable()
{
    /* 创建表格视图 */
        QTableView *tableView = new QTableView;

        /* 设置表格视图大小 */
        tableView->resize(640, 340);

        /* 创建数据模型 */
        QStandardItemModel* model = new QStandardItemModel();

        /* 设置表格标题行(输入数据为QStringList类型) */
       // model->setHorizontalHeaderLabels({"", "User Name", "City", "Classify", "Score", "Sign"});

        /* 自适应所有列，让它布满空间 */
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        /* 加载共8行数据，并每行有16列数据 */
        for (int i = 0; i < 8; i++)
            for(int j = 0; j< 16 ;j++)
            model->setItem(i, j, new QStandardItem(QString("")));

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

        //引导区红色
        model->setData(model->index(0, 0), QBrush(QColor(255, 0, 0)), Qt::BackgroundColorRole);
        //文件区橙色
        int use[8][16]={0};
        use[0][0]=1;//表示已被占用
        int n = 0;
        int i;
        for(i = 0;i < 8;i++)
        {
            if(use[i][i] == 1)
                continue;
            for(int j = 0;j < i;j++)
            {
                if(use[j][i] == 1)
                    continue;
                if(n == Fonum)
                    break;
                model->setData(model->index(j, i), QBrush(QColor(36, 169, 225)), Qt::BackgroundColorRole);
                use[j][i] = 1;
                n++;
            }
            if(n == Fonum)
                break;
            for(int k = i;k >= 0;k--)
            {
                if(use[i][k] == 1)
                    continue;
                if(n == Fonum)
                    break;
                model->setData(model->index(i, k), QBrush(QColor(36, 169, 225)), Qt::BackgroundColorRole);
                use[i][k] = 1;
                n++;
            }
        }
        if(i == 8)
        {
            for(i = 8;i < 16;i++)
            {
                if(use[i][i] == 1)
                    continue;
                for(int j = 8;j < i;j++)
                {
                    if(use[j][i] == 1)
                        continue;
                    if(n == Fonum)
                        break;
                    model->setData(model->index(j, i), QBrush(QColor(36, 169, 225)), Qt::BackgroundColorRole);
                    use[j][i] = 1;
                    n++;
                }
                if(n == Fonum)
                    break;
                for(int k = i;k >= 8;k--)
                {
                    if(use[i][k] == 1)
                        continue;
                    if(n == Fonum)
                        break;
                    model->setData(model->index(i, k), QBrush(QColor(36, 169, 225)), Qt::BackgroundColorRole);
                    use[i][k] = 1;
                    n++;
                }
            }
        }
        //权限区蓝色
        n = 0;
        for(i = 0;i < 8;i++)
        {
            if(use[i][i] == 1)
                continue;
            for(int j = 0;j < i;j++)
            {
                if(use[j][i] == 1)
                    continue;
                if(n == Fpnum)
                    break;
                model->setData(model->index(j, i), QBrush(QColor(137, 190, 178)), Qt::BackgroundColorRole);
                use[j][i] = 1;
                n++;
            }
            if(n == Fpnum)
                break;
            for(int k = i;k >= 0;k--)
            {
                if(use[i][k] == 1)
                    continue;
                if(n == Fpnum)
                    break;
                model->setData(model->index(i, k), QBrush(QColor(137, 190, 178)), Qt::BackgroundColorRole);
                use[i][k] = 1;
                n++;
            }
        }
        if(i == 8)
        {
            for(i = 8;i < 16;i++)
            {
                if(use[i][i] == 1)
                    continue;
                for(int j = 8;j < i;j++)
                {
                    if(use[j][i] == 1)
                        continue;
                    if(n == Fpnum)
                        break;
                    model->setData(model->index(j, i), QBrush(QColor(137, 190, 178)), Qt::BackgroundColorRole);
                    use[j][i] = 1;
                    n++;
                }
                if(n == Fpnum)
                    break;
                for(int k = i;k >= 8;k--)
                {
                    if(use[i][k] == 1)
                        continue;
                    if(n == Fpnum)
                        break;
                    model->setData(model->index(i, k), QBrush(QColor(137, 190, 178)), Qt::BackgroundColorRole);
                    use[i][k] = 1;
                    n++;
                }
            }
        }
        //空闲区白色//灰色
        for(i = 0;i < 8;i++)
            for(int j = 0;j < 16;j++)
                if(use[i][j]==0)
                {
                   // model->setData(model->index(i, j), QBrush(QColor(220, 220, 220)), Qt::BackgroundColorRole);
                    use[i][j] = 1;
                }
        /* 设置表格视图数据 */
        tableView->setModel(model);

        /* 显示 */
        tableView->show();

}
