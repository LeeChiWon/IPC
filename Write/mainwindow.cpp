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

void MainWindow::on_pushButton_Clear_clicked()
{
    if (sharedMemory->isAttached())
    {
        if (!sharedMemory->detach())
        {
            ui->label->setText(sharedMemory->errorString());
            return;
        }
    }

    sharedMemory->lock();
    char *to = (char*)sharedMemory->data();
    memset(to, '\0', sharedMemory->size());
    sharedMemory->unlock();
}

void MainWindow::on_pushButton_Write_clicked()
{
    if (sharedMemory->isAttached())
    {
        if (!sharedMemory->detach())
        {
            ui->label->setText(sharedMemory->errorString());
            return;
        }
    }

    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    QImage image;
    QString fileName;

    switch (ui->comboBox->currentIndex())
    {
    case 1:
        fileName = QFileDialog::getOpenFileName(this, QString(), QString(),
                                                tr("Images (*.png *.bmp *.jpg)"));
        if (!image.load(fileName)) {
            ui->label->setText("Select Image.");
            return;
        }
        ui->label->setPixmap(QPixmap::fromImage(image));
        out << image;
        break;
    default:
        out << ui->textEdit->toPlainText();
        break;
    }

    int size = buffer.size();
    if (!sharedMemory->create(size)) {
        ui->label->setText(sharedMemory->errorString());
        return;
    }
    sharedMemory->lock();
    char *to = (char*)sharedMemory->data();
    const char *from = buffer.data().data();
    memcpy(to, from, qMin(sharedMemory->size(), size));
    sharedMemory->unlock();
    buffer.close();
}

void MainWindow::init()
{
    key="SharedMemory_Test";
    sharedMemory=new QSharedMemory(key);

    ui->comboBox->addItems(QStringList()<<"Text"<<"Image");
    ui->comboBox->setCurrentIndex(0);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->label->setVisible(false);
        ui->textEdit->setVisible(true);
        break;
    default:
        ui->label->setVisible(true);
        ui->textEdit->setVisible(false);
        break;
    }
    ui->label->clear();
    ui->textEdit->clear();
}
