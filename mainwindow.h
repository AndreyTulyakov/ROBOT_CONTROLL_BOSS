#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "key_press_eater.h"

#include <QMainWindow>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QKeyEvent>
#include <stdlib.h>

using namespace std;

#define CONNECTION_PORT 7755
#define ROBOT_COMMAND_START_KEYWORD     "!$"
#define INIT_KEYWORD "ZBW"

// Robot commands list
#define ROBOT_COMMAND_INIT              0xA0
#define ROBOT_COMMAND_DEINIT            0xF0
#define ROBOT_COMMAND_TURN_TOWER        0xA1
#define ROBOT_COMMAND_DRIVE_CONTROLL    0xA2
#define ROBOT_COMMAND_GET_INFO          0xA3
#define ROBOT_COMMAND_DISPLAY           0xA4


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initSocket();
    void processTheDatagram(QNetworkDatagram datagram);
    void sendData(string data);

    void turnTurel(int offset);

public slots:
    void readPendingDatagrams();
    void onKeyboardPress(int key);

private slots:
    void on_pushButtonInit_clicked();

    void on_pushButtonDeinit_clicked();

    void on_pushButtonDisplayText_clicked();

    void on_dialTowerRotation_valueChanged(int value);

//    void keyPressEvent(QKeyEvent *ev);
//    void keyReleaseEvent(QKeyEvent *ev);

private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;
    KeyPressEater* filterObj;
};

#endif // MAINWINDOW_H
