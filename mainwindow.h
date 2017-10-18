#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "key_press_eater.h"

#include <QMainWindow>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QKeyEvent>
#include <QTimer>
#include <stdlib.h>

using namespace std;

#define KEYBOARD_TIMER_INTERVAL_MS      50
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

#define KEY_SEND_BUFFER_SIZE 64
#define KEY_CODES_COUNT 0xFFF


#define FRONT_DIRECTION 0b01111111
#define BACK_DIRECTION  0b11111111
#define SIGNLE_SIDE_TURN_RANGE  89
#define START_TURN_ANGLE  90

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
    void sendData(size_t bytes_count);

    void turnTurel(int offset);
    void drivePlatform(quint8 in_left_drive_power, quint8 in_right_drive_power);
    void checkDriveKeys();

public slots:
    void readPendingDatagrams();
    void onKeyboardPress(int key);
    void onKeyboardRelease(int key);
    void onKeyboardTimer();

private slots:
    void on_pushButtonInit_clicked();
    void on_pushButtonDeinit_clicked();
    void on_pushButtonDisplayText_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;
    KeyPressEater* filterObj;
    QTimer keyboard_timer;

    quint8 tower_rotation;
    quint8 left_drive_power;
    quint8 right_drive_power;

    quint8* send_buffer;

    bool* key_states;
};

#endif // MAINWINDOW_H
