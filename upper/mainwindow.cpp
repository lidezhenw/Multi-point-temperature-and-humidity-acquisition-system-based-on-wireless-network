#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_spcomm = new SPComm();
    ui->textEdit->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_spcomm;
}


void MainWindow::on_pushButton_clicked()
{
    if(m_spcomm->isOpenSerial())
    {
        return;
    }

    m_spcomm->setBaudRate(9600);
    m_spcomm->setPortName("com3");
    if(m_spcomm->open())
    {
        ui->label->setText("打开串口成功");
    }
    else
    {
        ui->label->setText("请打开串口！！！");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(m_spcomm->isOpenSerial())
    {
        m_spcomm->close();
        ui->label->setText("关闭串口成功");
    }
    else
    {
        ui->label->setText("未发现已打开的串口");
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    char buf[125] = "";

    //while(1)
    {
        memset(buf, 0, sizeof(buf));
        m_spcomm->readData(buf, sizeof(buf), 0);
        ui->textEdit->setText(buf);
    }
}
