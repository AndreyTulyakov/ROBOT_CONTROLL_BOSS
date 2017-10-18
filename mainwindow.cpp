#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    send_buffer = new quint8[KEY_SEND_BUFFER_SIZE];

    key_states = new bool[KEY_CODES_COUNT];
    memset(key_states, false, KEY_CODES_COUNT);

    connect(&keyboard_timer, SIGNAL(timeout()), this, SLOT(onKeyboardTimer()));
    keyboard_timer.start(KEYBOARD_TIMER_INTERVAL_MS);

    filterObj = new KeyPressEater(this);
    this->installEventFilter(filterObj);
    connect(filterObj, SIGNAL(signalKeyPress(int)), this, SLOT(onKeyboardPress(int)));
    connect(filterObj, SIGNAL(signalKeyRelease(int)), this, SLOT(onKeyboardRelease(int)));
    initSocket();

    tower_rotation = START_TURN_ANGLE;
    turnTurel(0);
    left_drive_power = 0;
    right_drive_power = 0;

}

MainWindow::~MainWindow()
{
    delete filterObj;
    delete ui;
}

void MainWindow::initSocket()
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::Any, ui->spinBoxRobotBodyPort->value());
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

void MainWindow::processTheDatagram(QNetworkDatagram datagram)
{
    QString data = QString(datagram.data());
    qDebug() << "Data:" << data;
}

void MainWindow::sendData(size_t bytes_count)
{
    QString hexstr;
    for(size_t i=0; i < bytes_count; i++) {
        hexstr = hexstr + " " + QString::number(send_buffer[i], 16).toUpper();
    }
    qDebug() << "send data: [" << hexstr.trimmed() << "]";

    qint64 writed_bytes = udpSocket->writeDatagram(QByteArray((const char *)send_buffer, bytes_count), QHostAddress(ui->lineRobotBodyIP->text()), ui->spinBoxRobotBodyPort->value());
    if(writed_bytes <= 0) {
        qDebug() << "Writes bad!!! " << writed_bytes;
    }
}

void MainWindow::turnTurel(int offset)
{
    int minimal_angle = START_TURN_ANGLE-SIGNLE_SIDE_TURN_RANGE;
    int maximal_angle = START_TURN_ANGLE+SIGNLE_SIDE_TURN_RANGE;

    int rotation = (int)tower_rotation - offset;
    if(rotation > maximal_angle)
        rotation = maximal_angle;
    if(rotation < minimal_angle)
        rotation = minimal_angle;

    if( tower_rotation != (quint8)rotation) {
        tower_rotation = (quint8)rotation;
        ui->lcdNumberRotation->display(tower_rotation);

        send_buffer[0] = ROBOT_COMMAND_START_KEYWORD[0];
        send_buffer[1] = ROBOT_COMMAND_START_KEYWORD[1];
        send_buffer[2] = ROBOT_COMMAND_TURN_TOWER;
        send_buffer[3] = tower_rotation;
        sendData(4);
    }
}

void MainWindow::drivePlatform(quint8 in_left_drive_power, quint8 in_right_drive_power)
{
    if(left_drive_power != in_left_drive_power || right_drive_power != in_right_drive_power)
    {
        left_drive_power  = in_left_drive_power;
        right_drive_power = in_right_drive_power;
        ui->lcdNumberLeftMotorPower->display(left_drive_power);
        ui->lcdNumberRightMotorPower->display(right_drive_power);

        send_buffer[0] = ROBOT_COMMAND_START_KEYWORD[0];
        send_buffer[1] = ROBOT_COMMAND_START_KEYWORD[1];
        send_buffer[2] = ROBOT_COMMAND_DRIVE_CONTROLL;
        send_buffer[3] = left_drive_power;
        send_buffer[4] = right_drive_power;
        sendData(5);
    }
}

void MainWindow::checkDriveKeys()
{
    quint8 MOTORS_POWER = 0xFF;

    if(key_states[Qt::Key_W] && !key_states[Qt::Key_A] && !key_states[Qt::Key_S] && !key_states[Qt::Key_D])
    {
        drivePlatform(MOTORS_POWER & FRONT_DIRECTION,
                      MOTORS_POWER & FRONT_DIRECTION);
        return;
    }

    if(!key_states[Qt::Key_W] && !key_states[Qt::Key_A] && key_states[Qt::Key_S] && !key_states[Qt::Key_D])
    {
        drivePlatform(MOTORS_POWER & BACK_DIRECTION,
                      MOTORS_POWER & BACK_DIRECTION);
        return;
    }

    if(!key_states[Qt::Key_W] && key_states[Qt::Key_A] && !key_states[Qt::Key_S] && !key_states[Qt::Key_D])
    {
        drivePlatform(MOTORS_POWER & BACK_DIRECTION,
                      MOTORS_POWER & FRONT_DIRECTION);
        return;
    }

    if(!key_states[Qt::Key_W] && !key_states[Qt::Key_A] && !key_states[Qt::Key_S] && key_states[Qt::Key_D])
    {
        drivePlatform(MOTORS_POWER & FRONT_DIRECTION,
                      MOTORS_POWER & BACK_DIRECTION);
        return;
    }

    drivePlatform(0, 0);
}

void MainWindow::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        processTheDatagram(datagram);
    }
}

void MainWindow::onKeyboardPress(int key)
{
    if(key < KEY_CODES_COUNT) {
        key_states[key] = true;
    }
}

void MainWindow::onKeyboardRelease(int key)
{
    if(key < KEY_CODES_COUNT) {
        key_states[key] = false;
    }
}

void MainWindow::onKeyboardTimer()
{
    if(key_states[Qt::Key_BracketLeft]) {
        turnTurel(-2);
    }

    if(key_states[Qt::Key_BracketRight]) {
        turnTurel(2);
    }

    checkDriveKeys();
}

void MainWindow::on_pushButtonInit_clicked()
{
    send_buffer[0] = ROBOT_COMMAND_START_KEYWORD[0];
    send_buffer[1] = ROBOT_COMMAND_START_KEYWORD[1];
    send_buffer[2] = ROBOT_COMMAND_INIT;
    send_buffer[3] = INIT_KEYWORD[0];
    send_buffer[4] = INIT_KEYWORD[1];
    send_buffer[5] = INIT_KEYWORD[2];
    sendData(6);
}

void MainWindow::on_pushButtonDeinit_clicked()
{
    send_buffer[0] = ROBOT_COMMAND_START_KEYWORD[0];
    send_buffer[1] = ROBOT_COMMAND_START_KEYWORD[1];
    send_buffer[2] = ROBOT_COMMAND_DEINIT;
    send_buffer[3] = INIT_KEYWORD[0];
    send_buffer[4] = INIT_KEYWORD[1];
    send_buffer[5] = INIT_KEYWORD[2];
    sendData(6);
}

void MainWindow::on_pushButtonDisplayText_clicked()
{
    QString text = ui->lineEditTextForDisplay->text();
    send_buffer[0] = ROBOT_COMMAND_START_KEYWORD[0];
    send_buffer[1] = ROBOT_COMMAND_START_KEYWORD[1];
    send_buffer[2] = ROBOT_COMMAND_DISPLAY;
    send_buffer[3] = 0; // X position
    send_buffer[4] = 0; // Y position
    size_t counter = 5;
    for(int i = 0; i < text.length() && i < KEY_SEND_BUFFER_SIZE-5; i++)
    {
        counter++;
        send_buffer[5+i] = QChar(text[i]).toLatin1();
    }
    sendData(counter);
}
