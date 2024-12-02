#include "cmdsend.h"
#include "ui_cmdsend.h"
#include <QAbstractSocket>
#include <QDebug>

CmdSend::CmdSend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CmdSend),
    enable_flag(false)
{
    ui->setupUi(this);

    tcpSocket = new QTcpSocket(this);

    connect(ui->btnConnect, &QPushButton::clicked, this, [=](){
        if (ui->btnConnect->text() == "Connect") {
            ui->btnConnect->setText("Connecting...");
            tcpSocket->connectToHost(ui->IPLineEdit->text(),ui->portSpinBox->value());
            // 连接成功
            connect(tcpSocket, &QTcpSocket::connected, this, &CmdSend::connected_slot);
            // 连接失败
            connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
                            this, &CmdSend::connected_error_slot);
        }
        else if (ui->btnConnect->text() == "Disconnect"){
            ui->btnConnect->setText("Disconnecting...");
            if (tcpSocket->state() != QAbstractSocket::UnconnectedState) {
                tcpSocket->close();
            } else {
                qWarning() << "Socket is already in UnconnectedState";
            }

            ui->btnConnect->setText("Connect");
            emit disconnected_arm_successfully();   // 断开连接成功
        }
    });

    // connect(tcpSocket, &QTcpSocket::connected, this, &CmdSend::onReadyRead);    // Read server messages

    connect(ui->btnRecord, &QPushButton::clicked, [=](){
        int data_send[] = {0x00, 0x00, 0x00, 0x01};     // Record
        if (sendData(data_send, 4) == true) {
            for (int i = 0; i < 4; ++i) {
                qDebug() << data_send[i];
            }
            emit recordingSignalsClicked();
        } else {
            emit sendFailed();
        }
    });

    connect(ui->btnStopRecord, &QPushButton::clicked, [=](){
        int data_send[] = {0x00, 0x00, 0x00, 0x02};     // Stop Record
        if (sendData(data_send, 4) == true) {
            for (int i = 0; i < 4; ++i) {
                qDebug() << data_send[i];
            }
            emit stopRecordingSignalsClicked();
        } else {
            emit sendFailed();
        }
    });

    connect(ui->btnReappear, &QPushButton::clicked, [=](){
        int data_send[] = {0x00, 0x00, 0x00, 0x03};     // Replaying
        if (sendData(data_send, 4) == true) {
            for (int i = 0; i < 4; ++i) {
                qDebug() << data_send[i];
            }
            emit reproductionSignalsClicked();
        } else {
            emit sendFailed();
        }
    });

    connect(ui->btnEnable, &QPushButton::clicked, [=](){
        if (ui->btnEnable->text() == "Enable") {
            ui->btnEnable->setText("Disable");
            int data_send[] = {0x00, 0x00, 0x00, 0x04};     // Enable
            if (sendData(data_send, 4) == true) {
                emit enableSignalsClicked();
                enable_flag = true;
                for (int i = 0; i < 4; ++i) {
                    qDebug() << data_send[i];
                }
            } else {
                emit sendFailed();
            }
        } else {
            ui->btnEnable->setText("Enable");
            int data_send[] = {0x00, 0x00, 0x00, 0x05};
            if (sendData(data_send, 4) == true) {           // Disable
                emit disableSignalsClicked();
                enable_flag = false;
                for (int i = 0; i < 4; ++i) {
                    qDebug() << data_send[i];
                }
            } else {
                emit sendFailed();
            }
        }
    });

    connect(ui->btnEmergencyStop, &QPushButton::clicked, [=](){
        int data_send[] = {0x00, 0x00, 0x00, 0x06};
        if (sendData(data_send, 4) == true) {       // Emergency Stop
            emit emergencyStopSignalsClicked();
            for (int i = 0; i < 4; ++i) {
                qDebug() << data_send[i];
            }
        } else {
            emit sendFailed();
        }
    });

    connect(ui->btnHome, &QPushButton::clicked, [=](){
        int data_send[] = {0x00, 0x00, 0x00, 0x07};
        if (sendData(data_send, 4) == true) {       // initial position
            emit homeSignalsClicked();
            for (int i = 0; i < 4; ++i) {
                qDebug() << data_send[i];
            }
        } else {
            emit sendFailed();
        }
    });

    connect(ui->btn2DPlane, &QPushButton::clicked, [=](){
        emit D2PlaneSignalsClicked();
    });

    connect(ui->btn3DSpace, &QPushButton::clicked, [=](){
        emit D3SpaceSignalsClicked();
    });
}

CmdSend::~CmdSend()
{
    delete ui;
}


void CmdSend::connected_slot()
{
    ui->btnConnect->setText("Disconnect");
    emit connected_arm_successfully();      // 连接成功
}

void CmdSend::connected_error_slot()
{
    ui->btnConnect->setText("Connect");
    emit connected_arm_failed();          // 连接失败
}

bool CmdSend::sendData(const QByteArray &data)
{
    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        qint64 bytesWritten = tcpSocket->write(data);
        if (bytesWritten == -1) {
            qDebug() << "Error Waiting data to socket:" << tcpSocket->errorString();
            emit sendFailed();
            return false;
        }
        if(tcpSocket->waitForBytesWritten()) {
            emit sendSuccessfully();
            return true;
        } else {
            qDebug() << "Timed out waiting for bytes to be written: " << tcpSocket->errorString();
            emit sendFailed();
            return false;
        }
    }
    else {
        qWarning() << "Socket is not connected";
        emit socketNotOpen();
        return false;
    }
}

bool CmdSend::sendData(const int *array, int size)
{
    QByteArray byteArray;
    qint64 bytesWritten;
    for (int i = 0; i < size; i++) {
        byteArray.append(static_cast<char>(array[i] & 0xFF));        // 取低字节
        byteArray.append(static_cast<char>((array[i] >> 8) & 0xFF)); // 取中低字节
        byteArray.append(static_cast<char>((array[i] >> 16) & 0xFF)); // 取中高字节
        byteArray.append(static_cast<char>((array[i] >> 24) & 0xFF)); // 取高字节
    }

    bytesWritten = tcpSocket->write(byteArray);
    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        if (bytesWritten == -1) {
            qDebug() << "Error Waiting data to socket:" << tcpSocket->errorString();
            emit sendFailed();
            return false;
        }
        if(tcpSocket->waitForBytesWritten()) {
            emit sendSuccessfully();
            return true;
        } else {
            qDebug() << "Timed out waiting for bytes to be written: " << tcpSocket->errorString();
            emit sendFailed();
            return false;
        }
    }
    else {
        qWarning() << "Socket is not connected";
        emit socketNotOpen();
        return false;
    }
}

//void CmdSend::onReadyRead()
//{
//    QByteArray data = tcpSocket->readAll();
//    QVector<double> jointValues = parseData(data);
//    emit dataReceived(jointValues);
//}

QVector<double> CmdSend::parseData(const QByteArray &data) {
    QVector<double> jointValues;
    QDataStream stream(data);
    double value;
    for (int i = 0; i < 6; ++i) {
        stream >> value;
        jointValues.append(value);
    }
    return jointValues;
}

bool CmdSend::handlePosPlusZ(int value)
{
    int data_send[4] = {0x01, 0x01, 0x00, 0x02};
    if (value > 0) {
        data_send[2] = 0x01;     // z pos + 2
    } else {
        data_send[2] = 0x00;     // z pos - 2
    }
    if (sendData(data_send, sizeof(data_send)) == true) {
        emit sendMoveRotateMsg(data_send);
        return true;
    } else {
        return false;
    }
}

bool CmdSend::handlePosPlusX(int value)
{
    int data_send[4] = {0x02, 0x01, 0x00, 0x02};
    if (value > 0) {
        data_send[2] = 0x01;     // x pos + 2
    } else {
        data_send[2] = 0x00;     // x pos - 2
    }
    if (sendData(data_send, sizeof(data_send)) == true) {
        emit sendMoveRotateMsg(data_send);
        return true;
    } else {
        return false;
    }
}

bool CmdSend::handlePosPlusY(int value)
{
    int data_send[4] = {0x03, 0x01, 0x00, 0x02};
    if (value > 0) {
        data_send[2] = 0x01;     // y pos + 2
    } else {
        data_send[2] = 0x00;     // y pos - 2
    }
    if (sendData(data_send, sizeof(data_send)) == true) {
        emit sendMoveRotateMsg(data_send);
        return true;
    } else {
        return false;
    }
}

bool CmdSend::handleRotAroundZ(int value)
{
    int data_send[4] = {0x01, 0x02, 0x00, 0x02};
    if (value > 0) {
        data_send[2] = 0x01;     // z rot + 2
    } else {
        data_send[2] = 0x00;     // z rot - 2
    }
    if (sendData(data_send, sizeof(data_send)) == true) {
        emit sendMoveRotateMsg(data_send);
        return true;
    } else {
        return false;
    }
}

bool CmdSend::handleRotAroundX(int value)
{
    int data_send[4] = {0x02, 0x02, 0x00, 0x02};
    if (value > 0) {
        data_send[2] = 0x01;     // x rot + 2
    } else {
        data_send[2] = 0x00;     // x rot - 2
    }
    if (sendData(data_send, sizeof(data_send)) == true) {
        emit sendMoveRotateMsg(data_send);
        return true;
    } else {
        return false;
    }
}

bool CmdSend::handleRotAroundY(int value)
{
    int data_send[4] = {0x03, 0x02, 0x00, 0x02};
    if (value > 0) {
        data_send[2] = 0x01;     // y rot + 2
    } else {
        data_send[2] = 0x00;     // y rot - 2
    }
    if (sendData(data_send, sizeof(data_send)) == true) {
        emit sendMoveRotateMsg(data_send);
        return true;
    } else {
        return false;
    }
}
