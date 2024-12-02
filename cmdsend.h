#ifndef CMDSEND_H
#define CMDSEND_H

#include <QWidget>
#include <QTcpSocket>


namespace Ui {
class CmdSend;
}

class CmdSend : public QWidget
{
    Q_OBJECT

public:
    explicit CmdSend(QWidget *parent = nullptr);
    ~CmdSend();

public slots:
    // TCP客户端发送
    bool sendData(const int *array, int size);
    bool sendData(const QByteArray &data);
    // TCP客户端读取
    // void onReadyRead();
    // 位姿状态转换
    bool handlePosPlusZ(int value);
    bool handlePosPlusX(int value);
    bool handlePosPlusY(int value);
    bool handleRotAroundZ(int value);
    bool handleRotAroundX(int value);
    bool handleRotAroundY(int value);

private:
    Ui::CmdSend *ui;
    QTcpSocket *tcpSocket;
    bool enable_flag = false;       // 默认失能状态

    QVector<double> parseData(const QByteArray &data);
private slots:
    void connected_slot();
    void connected_error_slot();

signals:
    void connected_arm_successfully();
    void connected_arm_failed();
    void disconnected_arm_successfully();

    void recordingSignalsClicked();
    void stopRecordingSignalsClicked();
    void reproductionSignalsClicked();

    void sendFailed();
    void sendSuccessfully();
    void socketNotOpen();
    void dataReceived(const QVector<double> &jointValues);

    void sendMoveRotateMsg(const int *data);

    void homeSignalsClicked();
    void enableSignalsClicked();
    void disableSignalsClicked();
    void emergencyStopSignalsClicked();

    void D2PlaneSignalsClicked();
    void D3SpaceSignalsClicked();
};

#endif // CMDSEND_H
