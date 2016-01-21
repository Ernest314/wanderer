#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString status_str = "Status: ";

    // Set up camera
    int camera_num = QCameraInfo::availableCameras().count();
    status_str += QString::number(camera_num) + " camera(s)\n";

    QCamera* camera = new QCamera();
    camera->setViewfinder(ui->viewfinder);
    camera->start();

    ui->label_status->setText(status_str);
}

MainWindow::~MainWindow()
{
    delete ui;
}
