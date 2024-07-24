#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSettings>
#include <QComboBox>
#include <QFile>
#include <QTimer>

#define CONFIG_FILE "D:\\documents\\qt\\qt\\OTA_finally\\config.ini"
//#define CONFIG_FILE "C:\\Users\\1\\Desktop\\qt\\qt_OTA\\config.ini"
#define DEFAULT_SERIAL_SECTION_NAME "/defaultSerialConfig/"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int setComboxDefalutIndex(QComboBox *combox, const QString &str);
    int writeSerialData(char *data, unsigned int len);
    void setSerialParameters();

private slots:
    void on_browseButton_clicked();
    void on_configApply_clicked();
    void readSerialData();

    // start update firmware
    void on_startButton_clicked();

    void on_documentPath_editingFinished();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QSettings *configSerial;
    int currentIndex;
    int packetSize;
    QTimer upgradeTimer;
};
#endif // MAINWINDOW_H
