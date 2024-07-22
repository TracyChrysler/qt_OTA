#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSettings>
#include <QComboBox>

#define CONFIG_FILE "D:\\documents\\qt\\qt\\OTA_finally\\config.ini"
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

private slots:
    void on_browseButton_clicked();
    void on_configApply_clicked();

    // start update firmware
    void on_startButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QSettings *configSerial;
};
#endif // MAINWINDOW_H
