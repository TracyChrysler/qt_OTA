#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTimer>

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
    , currentIndex(0)
    , packetSize(256)
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
    if(ui->comBoxParity->currentText() == "None")
    {
        serial->setParity(QSerialPort::NoParity);
    }
    else if (ui->comBoxParity->currentText() == "Odd")
    {
        serial->setParity(QSerialPort::OddParity);
    }
    else if (ui->comBoxParity->currentText() == "Even")
    {
        serial->setParity(QSerialPort::EvenParity);
    }
    else if (ui->comBoxParity->currentText() == "Mark")
    {
        serial->setParity(QSerialPort::MarkParity);
    }
    else if (ui->comBoxParity->currentText() == "Space")
    {
        serial->setParity(QSerialPort::SpaceParity);
    }

    // stopBits
    if(ui->comBoxStopBits->currentText() == "1")
    {
        serial->setStopBits(QSerialPort::OneStop);
    }
    else if (ui->comBoxStopBits->currentText() == "2")
    {
        serial->setStopBits(QSerialPort::TwoStop);
    }

    // flowControl
    if(ui->comBoxFlowControl->currentText() == "None")
    {
        serial->setFlowControl(QSerialPort::NoFlowControl);
    }
    else if (ui->comBoxFlowControl->currentText() == "Xon/Xoff")
    {
        serial->setFlowControl(QSerialPort::SoftwareControl);
    }
    else if (ui->comBoxFlowControl->currentText() == "Rts/Sts")
    {
        serial->setFlowControl(QSerialPort::HardwareControl);
    }
    else if (ui->comBoxFlowControl->currentText() == "Dsr/Dtr")
    {
        serial->setFlowControl(QSerialPort::HardwareControl);
    }

    // baud
    if(ui->comBoxSpeed->currentText() == "1200")
    {
        serial->setBaudRate(QSerialPort::Baud1200);
    }
    else if (ui->comBoxSpeed->currentText() == "2400")
    {
        serial->setBaudRate(QSerialPort::Baud2400);
    }
    else if (ui->comBoxSpeed->currentText() == "4800")
    {
        serial->setBaudRate(QSerialPort::Baud4800);
    }
    else if (ui->comBoxSpeed->currentText() == "9600")
    {
        serial->setBaudRate(QSerialPort::Baud9600);
    }
    else if (ui->comBoxSpeed->currentText() == "19200")
    {
        serial->setBaudRate(QSerialPort::Baud19200);
    }
    else if (ui->comBoxSpeed->currentText() == "38400")
    {
        serial->setBaudRate(QSerialPort::Baud38400);
    }
    else if (ui->comBoxSpeed->currentText() == "57600")
    {
        serial->setBaudRate(QSerialPort::Baud57600);
    }
    else if (ui->comBoxSpeed->currentText() == "115200")
    {
        serial->setBaudRate(QSerialPort::Baud115200);
    }
    else if (ui->comBoxSpeed->currentText() == "460800")
    {
        serial->setBaudRate(460800);
    }
    else if (ui->comBoxSpeed->currentText() == "921600")
    {
        serial->setBaudRate(921600);
    }
    else if (ui->comBoxSpeed->currentText() == "230400")
    {
        serial->setBaudRate(230400);
    }

    qDebug() << "dataBits:" << serial->dataBits();
    qDebug() << "parity:" << serial->parity();
    qDebug() << "stopBits:" << serial->stopBits();
    qDebug() << "flowControl:" << serial->flowControl();
    qDebug() << "baudRate:" << serial->baudRate() << endl;

    /* Open firmware file */
    QString filePath = ui->documentPath->text();
    if(filePath.isEmpty()){
        QMessageBox::warning(this,tr("警告"),tr("未选择文件！"));
        return;
    }

    QFile firmwareFile;
    firmwareFile.setFileName(filePath);
    if(!firmwareFile.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("警告"),tr("无法打开固件文件！"));
        return;
    }

    /* Read data from firmware */
    QByteArray firmwareData = firmwareFile.readAll();
    firmwareFile.close();
    if(firmwareData.isEmpty()){
        QMessageBox::warning(this,tr("警告"),tr("固件文件为空或者读取失败"));
    }
    qDebug() << "Firmare file loaded. size:" << firmwareData.size() << endl;

    /* Send cmd(0x1A) to start upgrade */
    QByteArray cmdStart;
    cmdStart.append(0xAA);  // 起始符
    cmdStart.append(0x55);  // 起始符
    cmdStart.append(0x15);  // 地址码
    cmdStart.append(0x1A);  // 命令字：开始升级
    cmdStart.append(0x08);  // 数据长度

    // 添加软件版本号和升级包大小
    uint32_t version = 1;  // 示例版本号
    uint32_t size = firmwareData.size();
    cmdStart.append(reinterpret_cast<const char*>(&version), sizeof(version));
    cmdStart.append(reinterpret_cast<const char*>(&size), sizeof(size));

    serial->write(cmdStart);
    if (serial->waitForBytesWritten(1000)) {
        qDebug() << "Start upgrade command sent.";
    } else {
        QMessageBox::critical(this, tr("错误"), tr("发送开始升级命令失败！"));
        return;
    }

    /* Read reply of starting cmd */
    if (serial->waitForReadyRead(1000)) {
        QByteArray response = serial->readAll();
        if (!response.isEmpty() && static_cast<uint8_t>(response[0]) == 0x00) {
            packetSize = response[1] << 8 | response[2];  // 获取协商的分段大小
            qDebug() << "Start command acknowledged. Packet size:" << packetSize;
            currentIndex = 0;
            QTimer upgradeTimer;
            upgradeTimer.start(100);  // 每100ms发送一块数据
        } else {
            qDebug() << "Start command not acknowledged. Retrying...";
            //startUpgrade();
            if (currentIndex < firmwareData.size()) {
                QByteArray dataBlock = firmwareData.mid(currentIndex, packetSize);
                QByteArray cmdData;
                cmdData.append(0xAA);  // 起始符
                cmdData.append(0x55);  // 起始符
                cmdData.append(0x15);  // 地址码
                cmdData.append(0x1B);  // 命令字：发送升级数据
                cmdData.append(dataBlock.size() + 2);  // 数据长度
                uint16_t packetId = currentIndex / packetSize + 1;
                cmdData.append(reinterpret_cast<const char*>(&packetId), sizeof(packetId));
                cmdData.append(dataBlock);

                serial->write(cmdData);
                if (serial->waitForBytesWritten(1000)) {
                    qDebug() << "Data block sent. Packet ID:" << packetId;
                } else {
                    QMessageBox::critical(this, tr("错误"), tr("发送数据块失败！"));
                    upgradeTimer.stop();
                    return;
                }

                // 等待回应
                if (serial->waitForReadyRead(1000)) {
                    QByteArray response = serial->readAll();
                    if (!response.isEmpty() && static_cast<uint16_t>(response[0]) == packetId) {
                        qDebug() << "Data block acknowledged. Continue transferring.";
                        currentIndex += packetSize;
                    } else {
                        qDebug() << "Data block not acknowledged. Retrying...";
                        return;
                    }
                } else {
                    QMessageBox::critical(this, tr("错误"), tr("设备未响应数据块传输！"));
                    upgradeTimer.stop();
                    return;
                }
            } else {
                upgradeTimer.stop();
                //finishUpgrade();
                QByteArray cmdFinish;
                cmdFinish.append(0xAA);  // 起始符
                cmdFinish.append(0x55);  // 起始符
                cmdFinish.append(0x15);  // 地址码
                cmdFinish.append(0x1C);  // 命令字：升级结束
                cmdFinish.append(0x04);  // 数据长度

                // 添加校验和
                uint32_t checksum = 0;  // 计算固件数据的校验和
                for (const auto &byte : firmwareData) {
                    checksum += static_cast<uint8_t>(byte);
                }
                cmdFinish.append(reinterpret_cast<const char*>(&checksum), sizeof(checksum));

                serial->write(cmdFinish);
                if (serial->waitForBytesWritten(1000)) {
                    qDebug() << "Finish upgrade command sent.";
                } else {
                    QMessageBox::critical(this, tr("错误"), tr("发送升级结束命令失败！"));
                    return;
                }

                // 等待回应
                if (serial->waitForReadyRead(1000)) {
                    QByteArray response = serial->readAll();
                    if (!response.isEmpty() && static_cast<uint8_t>(response[0]) == 0x00) {
                        qDebug() << "Firmware upgrade completed successfully.";
                        QMessageBox::information(this, tr("成功"), tr("固件升级完成！"));
                    } else {
                        QMessageBox::critical(this, tr("错误"), tr("设备未确认完成命令！"));
                    }
                } else {
                    QMessageBox::critical(this, tr("错误"), tr("设备未响应完成命令！"));
                }
            }
        }
    } else {
        QMessageBox::critical(this, tr("错误"), tr("设备未响应开始命令！"));
    }

    /* Judge if rc euqal to zero, if not, send start cmd agin.
     * If you haven't received a zero return value in more than one minute,
     * then, terminate the upgrade process.*/

    /* Send cmd(0x1B) to transfer firmware data */


    /* Send cmd(0x1C) to finish upgrade process */
}

