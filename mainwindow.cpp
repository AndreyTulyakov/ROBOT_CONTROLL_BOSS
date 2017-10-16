#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initSocket();
}

MainWindow::~MainWindow()
{
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

void MainWindow::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        processTheDatagram(datagram);
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
