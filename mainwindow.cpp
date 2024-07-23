#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFile>

int MainWindow::setComboxDefalutIndex(QComboBox *combox, const QString &str)
{
    int index;
    index = combox->findText(str);
    if (index != -1) { // 确保找到了文本
        combox->setCurrentIndex(index);
        qDebug() << "already find the text:" << str << endl;
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

    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    for(int i = 0; i<list.size(); i++)
    {
        ui->comBoxSerialPort->addItem(list.at(i).portName());
    }

    //Read config files and set defaut parameters for serial
    serial = new QSerialPort;
    configSerial = new QSettings(CONFIG_FILE, QSettings::IniFormat);

    // Set data bits list
    for(int i = 0; i < 4; i++)
    {
        ui->comBoxDataBits->addItem(QString::number(5 + i));
    }
    QString dataBits = configSerial->value(DEFAULT_SERIAL_SECTION_NAME"dataBits").toString();
    setComboxDefalutIndex(ui->comBoxDataBits, dataBits);

    // Set parity list
    QString parityList[] = {"None", "Odd", "Even", "Mark", "Space"};
    for (int i = 0; i < 5; ++i) {
        ui->comBoxParity->addItem(parityList[i]);
    }
    QString parity = configSerial->value(DEFAULT_SERIAL_SECTION_NAME"parity").toString();
    setComboxDefalutIndex(ui->comBoxParity, parity);

    // Set stop bits list
    ui->comBoxStopBits->addItem("1");
    ui->comBoxStopBits->addItem("2");
    QString stopBits = configSerial->value(DEFAULT_SERIAL_SECTION_NAME"stopBits").toString();
    setComboxDefalutIndex(ui->comBoxStopBits, stopBits);

    // Set flow control list
    QString flowControlList[] = {"None", "Xon/Xoff", "Rts/Cts", "Dsr/Dtr"};
    for (int i = 0; i < 4; ++i) {
        ui->comBoxFlowControl->addItem(flowControlList[i]);
    }
    QString flowControl = configSerial->value(DEFAULT_SERIAL_SECTION_NAME"flowControl").toString();
    setComboxDefalutIndex(ui->comBoxFlowControl,flowControl );

    // Set speed list
    QString speedList[] = {"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200", "460800", "921600", "230400"};
    for (int i = 0; i < 11; ++i) {
        ui->comBoxSpeed->addItem(speedList[i]);
    }
    QString baudRate = configSerial->value(DEFAULT_SERIAL_SECTION_NAME"baudRate").toString();
    setComboxDefalutIndex(ui->comBoxSpeed, baudRate);
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
    QString strDataBit =ui->comBoxDataBits->currentText();
    QString strParit =ui->comBoxParity->currentText();
    QString strStopBit =ui->comBoxStopBits->currentText();
    QString strFlowControl =ui->comBoxFlowControl->currentText();
    QString strSpeed =ui->comBoxSpeed->currentText();
    qDebug() << "strDataBit:" << strDataBit;
    qDebug() << "strParit:" << strParit;
    qDebug() << "strStopBit:" << strStopBit;
    qDebug() << "strFlowControl:" << strFlowControl;
    qDebug() << "strSpeed:" << strSpeed;
    configSerial->setValue(DEFAULT_SERIAL_SECTION_NAME"dataBits", strDataBit);
    configSerial->setValue(DEFAULT_SERIAL_SECTION_NAME"parity", strParit);
    configSerial->setValue(DEFAULT_SERIAL_SECTION_NAME"stopBit", strStopBit);
    configSerial->setValue(DEFAULT_SERIAL_SECTION_NAME"flowControl", strFlowControl);
    configSerial->setValue(DEFAULT_SERIAL_SECTION_NAME"baudRate", strSpeed);
}

void MainWindow::on_startButton_clicked()
{
    /* Set serial parameters */
    // dataBits
    if(ui->comBoxDataBits->currentText() == "5")
    {
        serial->setDataBits(QSerialPort::Data5);
    }
    else if (ui->comBoxDataBits->currentText() == "6")
    {
        serial->setDataBits(QSerialPort::Data6);
    }
    else if (ui->comBoxDataBits->currentText() == "7")
    {
        serial->setDataBits(QSerialPort::Data7);
    }
    else if (ui->comBoxDataBits->currentText() == "8")
    {
        serial->setDataBits(QSerialPort::Data8);
    }

    // parity
    if(ui->comBoxParity->currentText() == "")
    {
        serial->(::);
    }
    else if (ui->comBoxParity->currentText() == "")
    {
        serial->(::);
    }
    else if (ui->comBoxParity->currentText() == "")
    {
        serial->(::);
    }
    else if (ui->comBoxParity->currentText() == "")
    {
        serial->(::);
    }

    // parity
    if(ui->->currentText() == "")
    {
        serial->(::);
    }
    else if (ui->->currentText() == "")
    {
        serial->(::);
    }
    else if (ui->->currentText() == "")
    {
        serial->(::);
    }
    else if (ui->->currentText() == "")
    {
        serial->(::);
    }

    // parity
    if(ui->->currentText() == "")
    {
        serial->(::);
    }
    else if (ui->->currentText() == "")
    {
        serial->(::);
    }
    else if (ui->->currentText() == "")
    {
        serial->(::);
    }
    else if (ui->->currentText() == "")
    {
        serial->(::);
    }

    // parity
    if(ui->->currentText() == "")
    {
        serial->(::);
    }
    else if (ui->->currentText() == "")
    {
        serial->(::);
    }
    else if (ui->->currentText() == "")
    {
        serial->(::);
    }
    else if (ui->->currentText() == "")
    {
        serial->(::);
    }
    /* Open firmware file */

    /* Read data from firmware */

    /* Send cmd(0x1A) to start upgrade */

    /* Read reply of starting cmd */

    /* Judge if rc euqal to zero, if not, send start cmd agin.
     * If you haven't received a zero return value in more than one minute,
     * then, terminate the upgrade process.*/

    /* Send cmd(0x1B) to transfer firmware data */

    /* Send cmd(0x1C) to finish upgrade process */
}

