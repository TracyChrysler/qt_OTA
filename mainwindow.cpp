#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    for(int i = 0; i<list.size(); i++)
    {
        ui->comBoxSerialPort->addItem(list.at(i).portName());
    }

    serial = new QSerialPort;
    configSerial = new QSettings(CONFIG_FILE, QSettings::IniFormat);
    QString dataBits = configSerial->value("/defaultSerialConfig/dataBits").toString();
    ui->comBoxDataBits->addItem(dataBits);
    QString parity = configSerial->value("/defaultSerialConfig/parity").toString();
    ui->comBoxParity->addItem(parity);
    QString stopBits = configSerial->value("/defaultSerialConfig/stopBits").toString();
    ui->comBoxStopBits->addItem(stopBits);
    QString flowControl = configSerial->value("/defaultSerialConfig/flowControl").toString();
    ui->comBoxFlowControl->addItem(flowControl);
    QString baudRate = configSerial->value("/defaultSerialConfig/baudRate").toString();
    ui->comBoxSpeed->addItem(baudRate);

}

MainWindow::~MainWindow()
{
    delete configSerial;
    delete serial;
    delete ui;
}

void MainWindow::on_browseButton_clicked()
{
    qDebug() << "open dialog" << endl;
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("All File(*)"));
    if(fileName.isEmpty()){
        //如果未选择文件，弹出警告框
        QMessageBox::warning(this,tr("警告"),tr("未选择文件！"));
    }
    else{
        ui->documentPath->setText(fileName);
    }
//    //读取文件内容并显示到ui->filecontent中
//    QFile file(fileName);
//    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
//        QMessageBox::critical(this,tr("错误"),tr("无法打开文件！"));
//        return;
//    }
//    QTextStream in(&file);
//    QString fileContent = in.readAll();
//    ui->fileContent->setPlainText(fileContent);

//    file.close();
}
