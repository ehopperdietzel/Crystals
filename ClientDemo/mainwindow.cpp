#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMovie>
#include <QDebug>
#include <QProcess>
//#include "capplication.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    resize(600,600);
    ui->setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint);
    ui->label->setMovie(new QMovie("/home/e/giphy.gif"));
    //setWindowOpacity(0.1);
    ui->label->movie()->start();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
}
