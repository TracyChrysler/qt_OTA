#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFile>

// Define parity structure
struct parityOption{
    QString text;
    QSerialPort:: Parity value;
};
// Define flowControl structure
struct flowControlOption{
    QString text;
    QSerialPort:: FlowControl value;
};
// Define baudRate structure
struct buadRateOption{
    QString text;
    int value;
};

int MainWindow::setComboxDefalutIndex(QComboBox *combox, const QString &str)
{
    int index;
    index = combox->findText(str);
    if (index != -1) { // 确保找到了文本
        combox->setCurrentIndex(index);
        qDebug() << "already find text:" << str << endl;
    } else {
        // 文本未找到的处理逻辑
        qDebug() << "Given text:" << str << "not found in the ComboBox.";
        return -1;
    }
    return 0;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Select serial list and show
    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    for(int i = 0; i<list.size(); i++)
    {
        ui->comBoxSerialPort->addItem(list.at(i).portName());
    }

    // Read config files and set defaut parameters for serial
    serial = new QSerialPort;
    configSerial = new QSettings(CONFIG_FILE, QSettings::IniFormat);

    // Set data bits list
    for(int i = 0; i < 4; i++)
    {
        int dataBits = 5 + i;
        ui->comBoxDataBits->addItem(QString::number(dataBits),dataBits);
        qDebug() << "Item" << i << "text:" << ui->comBoxDataBits->itemText(i) << "data:" << ui->comBoxDataBits->itemData(i).toInt();
    }
    QString dataBits = configSerial->value("/defaultSerialConfig/dataBits").toString();
    setComboxDefalutIndex(ui->comBoxDataBits, dataBits);

    // Set parity list
    parityOption parityOption[] = {
        {"None",QSerialPort::NoParity},
        {"Odd",QSerialPort::OddParity},
        {"Even",QSerialPort::EvenParity},
        {"Mark",QSerialPort::MarkParity},
        {"Space",QSerialPort::SpaceParity}
    };
    for (const struct parityOption &parity : parityOption) {
        ui->comBoxParity->addItem(parity.text,parity.value);
    }
    for (int i = 0; i < ui->comBoxParity->count(); ++i) {
        qDebug() << "Item" << i << "text:" << ui->comBoxParity->itemText(i) << "data:" << ui->comBoxParity->itemData(i).toString();
    }
    QString parity = configSerial->value("/defaultSerialConfig/parity").toString();
    setComboxDefalutIndex(ui->comBoxParity, parity);

    // Set stop bits list
    for(int i = 0; i < 2; i++)
    {
        int stopBits = 1 + i;
        ui->comBoxStopBits->addItem(QString::number(stopBits),stopBits);
        qDebug() << "Item" << i << "text:" << ui->comBoxStopBits->itemText(i) << "data:" << ui->comBoxStopBits->itemData(i).toInt();
    }
    QString stopBits = configSerial->value("/defaultSerialConfig/stopBits").toString();
    setComboxDefalutIndex(ui->comBoxStopBits, stopBits);

    // Set flow control list
    flowControlOption flowControlOption[] = {
        {"None",QSerialPort::NoFlowControl},
        {"Xon/Xoff",QSerialPort::SoftwareControl},
        {"Rts/Cts",QSerialPort::HardwareControl},
        {"Dsr/Dtr",QSerialPort::HardwareControl},
    };
    for (const struct flowControlOption &flowControl : flowControlOption) {
        ui->comBoxFlowControl->addItem(flowControl.text,flowControl.value);
    }
    for (int i = 0; i < ui->comBoxFlowControl->count(); ++i) {
        qDebug() << "Item" << i << "text:" << ui->comBoxFlowControl->itemText(i) << "data:" << ui->comBoxFlowControl->itemData(i).toString();
    }
    QString flowControl = configSerial->value("/defaultSerialConfig/flowControl").toString();
    setComboxDefalutIndex(ui->comBoxFlowControl,flowControl );

    // Set speed list
    buadRateOption buadRateOption[] = {
        {"1200",1200},
        {"2400",2400},
        {"4800",4800},
        {"9600",9600},
        {"19200",19200},
        {"38400",38400},
        {"57600",57600},
        {"115200",115200},
        {"460800",460800},
        {"921600",921600},
        {"230400",230400}
    };
    for(const struct buadRateOption &buadRate : buadRateOption){
        ui->comBoxSpeed->addItem(buadRate.text,buadRate.value);
    }
    for (int i = 0; i < ui->comBoxSpeed->count(); ++i) {
        qDebug() << "Item" << i << "text:" << ui->comBoxSpeed->itemText(i) << "data:" << ui->comBoxSpeed->itemData(i).toInt();
    }
    QString baudRate = configSerial->value("/defaultSerialConfig/baudRate").toString();
    setComboxDefalutIndex(ui->comBoxSpeed, baudRate);

    // Try opening the serial port
    //if (!serial->open(QIODevice::ReadWrite)) {
    //    QMessageBox::critical(this, tr("错误"), tr("无法打开串口！"));
    //    return;
    //}
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

void MainWindow::on_documentPath_editingFinished()
{
    QString filePath = ui->documentPath->text();
    if (filePath.isEmpty() || !QFile::exists(filePath)) {
        QMessageBox::warning(this, tr("警告"), tr("文件路径无效或文件不存在！"));
        return;
    }

    // 文件路径有效，可以在这里进一步处理文件
    qDebug() << "File path entered: " << filePath;

}


void MainWindow::on_configApply_clicked()
{
    // Set serialPort
    serial->setPort(QSerialPortInfo(ui->comBoxSerialPort->currentText()));

    // Set serialDataBits
    int dataBits = ui->comBoxDataBits->currentData().toInt();
    qDebug() << "dataBits is:" << dataBits;

    if (serial->setDataBits(static_cast<QSerialPort::DataBits>(dataBits))) {
        qDebug() << "dataBits has been set :" << dataBits;
    } else {
        QMessageBox::critical(this, tr("error"), tr("Cann't setting dataBits!"));
    }

    // Set serialParity
    QSerialPort::Parity parity = static_cast<QSerialPort::Parity>(ui->comBoxParity->currentData().toInt());
    qDebug() << "parity is:" << parity;

    if (serial->setParity(parity)) {
        qDebug() << "parity has been set:" << parity;
    } else {
        QMessageBox::critical(this, tr("error"), tr("Cannot set parity!"));
    }

    // Set serialStopBits
    int stopBits = ui->comBoxStopBits->currentData().toInt();
    qDebug() << "stopBits is:" << stopBits;

    if (serial->setStopBits(static_cast<QSerialPort::StopBits>(stopBits))) {
        qDebug() << "stopBits has been set :" << stopBits;
    } else {
        QMessageBox::critical(this, tr("error"), tr("Cann't setting stopBits!"));
    }

    // Set serialFlowControl
    QSerialPort::FlowControl flowControl = static_cast<QSerialPort::FlowControl>(ui->comBoxFlowControl->currentData().toInt());
    qDebug() << "parity is:" << parity;

    if (serial->setFlowControl(flowControl)) {
        qDebug() << "flowControl has been set:" << flowControl;
    } else {
        QMessageBox::critical(this, tr("error"), tr("Cannot set flowcontrol!"));
    }
    // Set serialBaudRate
    int baudRate = ui->comBoxSpeed->currentData().toInt();
    qDebug() << "baudRate is:" << baudRate;

    if (serial->setBaudRate(baudRate)) {
        qDebug() << "baudRate has been set:" << baudRate;
    } else {
        QMessageBox::critical(this, tr("error"), tr("Cannot set baudRate!"));
    }
}

