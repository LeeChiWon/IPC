#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Read_clicked();
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    QSharedMemory *sharedMemory;
    QString key;

    void init();
};

#endif // MAINWINDOW_H
