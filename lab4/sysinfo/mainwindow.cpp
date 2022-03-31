#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QTime>
#include <QTimer>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>

void Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    FILE* fp;
    char text[100], tmp[1024];
    ui->setupUi(this);
    QStandardItemModel* model = new QStandardItemModel();
    model->setColumnCount(6);
    model->setHeaderData(0,Qt::Horizontal, "PID");
    model->setHeaderData(1,Qt::Horizontal, "应用");
    model->setHeaderData(2,Qt::Horizontal, "状态");
    model->setHeaderData(3,Qt::Horizontal, "PPID");
    model->setHeaderData(4,Qt::Horizontal, "优先级");
    model->setHeaderData(5,Qt::Horizontal, "VSize");

    char pinfo[6][100];
    DIR* dp;
    struct dirent* entry;
    struct stat statbuf;
    dp = opendir("/proc");
    chdir("/proc");
    int count=0;
    while (1)
    {
        entry = readdir(dp);
        if (entry == NULL) break;
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode))
        {
            if (strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) continue;
            if (entry->d_name[0] >= '0' && entry->d_name[0] <= '9')
            {
                chdir(entry->d_name);
                fp = fopen("stat", "r");
                if (fp == NULL) break;
                fscanf(fp, "%s (%s %s %s", pinfo[0], pinfo[1], pinfo[2], pinfo[3]);
                for (int i = 0; i < 100; i++)
                {
                    if (pinfo[1][i] == ')') pinfo[1][i] = 0;
                }
                for (int i = 0; i < 14; i++) fscanf(fp, "%s", pinfo[4]);
                for (int i = 0; i < 5; i++) fscanf(fp, "%s", pinfo[5]);
                model->setItem(count, 0, new QStandardItem(QString::fromStdString(pinfo[0])));
                model->setItem(count, 1, new QStandardItem(QString::fromStdString(pinfo[1])));
                model->setItem(count, 2, new QStandardItem(QString::fromStdString(pinfo[2])));
                model->setItem(count, 3, new QStandardItem(QString::fromStdString(pinfo[3])));
                model->setItem(count, 4, new QStandardItem(QString::fromStdString(pinfo[4])));
                model->setItem(count, 5, new QStandardItem(QString::fromStdString(pinfo[5])));
                count++;
                chdir("..");
                fclose(fp);
            }
        }
    }
    ui->tableView->setModel(model);



    //获取主机名（功能1）
    memset(text, 0, sizeof(text));
    fp = fopen("/proc/sys/kernel/hostname", "r");
    fscanf(fp, "%[^\n]", text);
    ui->label->setText("主机名："+QString::fromStdString(text));
    fclose(fp);

    //获取系统版本号（功能4）
    fp = fopen("/proc/sys/kernel/ostype", "r");
    fscanf(fp, "%s", text);
    ui->label_4->setText("系统类型："+QString::fromStdString(text));
    fclose(fp);
    fp = fopen("/proc/sys/kernel/osrelease", "r");
    fscanf(fp, "%s", text);
    ui->label_5->setText("内核版本："+QString::fromStdString(text));
    fclose(fp);

    //获取CPU信息（功能5）
    fp = fopen("/proc/cpuinfo","r");
    for (int i = 0; i < 5; i++) fgets(tmp, 100, fp);
    for (int i = 13; i < 100; i++) text[i - 13] = tmp[i];
    ui->label_6->setText("CPU信息："+QString::fromStdString(text));

    ui->label_2->setText("当前运行时间：");
    ui->label_3->setText("总运行时间：");
    ui->label_7->setText("当前时间：");
    ui->label_8->setText("可用内存/总内存：");
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(1000);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerUpdate(void)
{
    FILE* fp;
    char text[100], tmp[1024];
    // 获取当前系统时间
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    ui->label_7->setText("当前时间："+str);

    //获取启动时间和运行时间（功能2、3）
    memset(text, 0, sizeof(text));
    fp = fopen("/proc/uptime", "r");
    fscanf(fp, "%s", text);
    strcat(text," s");
    ui->label_2->setText("当前运行时间："+QString::fromStdString(text));
    fscanf(fp, "%s", text);
    strcat(text," s");
    ui->label_3->setText("总运行时间："+QString::fromStdString(text));
    fclose(fp);

    //显示系统剩余内存
    int MemTotal, MemFree, MemUsed;
    fp = fopen("/proc/meminfo", "r");
    fscanf(fp, "%s %d %s", tmp, &MemTotal,tmp);
    fscanf(fp, "%s %d", tmp, &MemFree);
    MemUsed = MemTotal - MemFree;
    ui->label_8->setText("已用内存/总内存："+QString::number(MemUsed)+" KB /"+QString::number(MemTotal)+" KB");
}

void MainWindow::on_pushButton_clicked()
{
    system("shutdown -h now");
}


void MainWindow::on_pushButton_2_clicked()
{
    system("firefox");
}

