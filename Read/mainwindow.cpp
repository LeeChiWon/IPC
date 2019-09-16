#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    if(sharedMemory!=NULL)
    {
        if(sharedMemory->isAttached())
        {
            sharedMemory->detach();
        }
        delete sharedMemory;
    }
    delete ui;
}

void MainWindow::init()
{
    key="SharedMemory_Test";
    sharedMemory=new QSharedMemory(key);

    ui->comboBox->addItems(QStringList()<<"Text"<<"Image");
    ui->comboBox->setCurrentIndex(0);
}

void MainWindow::on_pushButton_Read_clicked()
{
    if (!sharedMemory->attach())
    {
        ui->label->setText(sharedMemory->errorString());
        return;
    }

    QBuffer buffer;
    QDataStream in(&buffer);
    QImage image;
    QString text;
    sharedMemory->lock();
    buffer.setData((char*)sharedMemory->constData(), sharedMemory->size());
    buffer.open(QBuffer::ReadOnly);

    switch (ui->comboBox->currentIndex()) {
    case 1:
        in >> image;
        ui->label->setPixmap(QPixmap::fromImage(image));
        break;
    default:
        in >> text;
        ui->label->setText(text);
        break;
    }
    sharedMemory->unlock();
    sharedMemory->detach();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->label->clear();
}
