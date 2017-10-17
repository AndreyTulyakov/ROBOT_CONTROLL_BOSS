#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    filterObj = new KeyPressEater(this);
    this->installEventFilter(filterObj);
    connect(filterObj, SIGNAL(signalKeyPress(int)), this, SLOT(onKeyboardPress(int)));
    initSocket();
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

void MainWindow::sendData(string data)
{
    qint64 writed_bytes = udpSocket->writeDatagram(QByteArray(data.c_str(), data.length()), QHostAddress(ui->lineRobotBodyIP->text()), ui->spinBoxRobotBodyPort->value());
    if(writed_bytes > 0) {
        qDebug() << "Writes good! " << writed_bytes;
    } else {
        qDebug() << "Writes bad!!! " << writed_bytes;
    }
}

void MainWindow::turnTurel(int offset)
{
    int current_value = ui->dialTowerRotation->value();
    ui->dialTowerRotation->setValue(current_value+offset);
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
    switch (key) {
    case Qt::Key_BracketLeft:
        turnTurel(-3);
        break;
    case Qt::Key_BracketRight:
        turnTurel(3);
        break;
    }
}

void MainWindow::on_pushButtonInit_clicked()
{
    string command = string(ROBOT_COMMAND_START_KEYWORD) + char(ROBOT_COMMAND_INIT) + string(INIT_KEYWORD);
    sendData(command);
}

void MainWindow::on_pushButtonDeinit_clicked()
{
    string command = string(ROBOT_COMMAND_START_KEYWORD) + char(ROBOT_COMMAND_DEINIT) + string(INIT_KEYWORD);
    sendData(command);
}


void MainWindow::on_pushButtonDisplayText_clicked()
{
    string command = string(ROBOT_COMMAND_START_KEYWORD) + char(ROBOT_COMMAND_DISPLAY) + char(0) + char(0) + ui->lineEditTextForDisplay->text().toStdString();
    sendData(command);
}

void MainWindow::on_dialTowerRotation_valueChanged(int value)
{
    qDebug() << value;
    string command = string(ROBOT_COMMAND_START_KEYWORD) + char(ROBOT_COMMAND_TURN_TOWER) + char(value);
    sendData(command);
}


//void MainWindow::keyPressEvent(QKeyEvent *event)
//{
//    if (event->isAutoRepeat())
//        return;

//    switch (event->key()) {
//    case Qt::Key_BracketLeft:
//        turnTurel(-3);
//        event->accept();
//        break;
//    case Qt::Key_BracketRight:
//        turnTurel(3);
//        event->accept();
//        break;
//    default:
//        QMainWindow::keyPressEvent(event);
//    }
//    qDebug() << ("You Pressed Key " + event->text()) << event->key();
//}

//void MainWindow::keyReleaseEvent(QKeyEvent *event)
//{
//    if (event->isAutoRepeat())
//        return;

//    switch (event->key()) {
//    case Qt::Key_CameraFocus:

//        break;
//    default:
//        QMainWindow::keyReleaseEvent(event);
//    }
//    qDebug() << ("You Release Key " + event->text()) << event->key();
//}
