#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cmdsend.h"
#include "posctrl.h"
#include "rotctrl.h"
#include "jointpos.h"
#include "armgl.h"
#include "jointposfed.h"
#include "developermode.h"
#include "versioninfo.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QMetaObject>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/img/logo1.jpg"));
    setWindowTitle(QString("DRobot Ctrl"));
    /******************************* 菜单栏 *******************************/
    connect(ui->CmdSendWidget, &CmdSend::dataReceived, this, &MainWindow::onDataReceived);
    connect(ui->actionJointPos_Feedback, &QAction::triggered, this, &MainWindow::onJointPositionFeedback);
    connect(ui->actionDeveloper_Mode, &QAction::triggered, this, &MainWindow::onDeveloperModeTriggered);
    connect(ui->actionAbout_DRobot_Ctrl_Version, &QAction::triggered, this, &MainWindow::onVersionTriggered);
    /******************************* 菜单栏 *******************************/

    /******************************* 机械臂状态显示 *******************************/
    QVBoxLayout *layout = new QVBoxLayout(ui->MsgShow);
    ui->MsgShow->setLayout(layout);

    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    layout->addWidget(label);
    /******************************* 机械臂状态显示 *******************************/

    /******************************* 虚拟机械臂状态更新 *******************************/
    void(JointPos::*joint_pos_angles)(int, int) = &JointPos::sigJointValueChanged;
    void(MainWindow::*joint_pos_update)(int, int) = &MainWindow::slotJVarsValueChange;
    connect(ui->JointPosWidget, joint_pos_angles, this, joint_pos_update);
    /******************************* 虚拟机械臂状态更新 *******************************/

    /******************************* Slider 目标状态 *******************************/
    void(JointPos::*joint_pos_sig)(const int*, int) = &JointPos::callSendData;
    bool(CmdSend::*cmd_send_slot)(const int*, int) = &CmdSend::sendData;
    connect(ui->JointPosWidget, joint_pos_sig, ui->CmdSendWidget, cmd_send_slot);
    /******************************* Slider 目标状态 *******************************/

    /******************************* Arrow 操纵末端执行器 *******************************/
    void(PosCtrl::*pos_plus_z)(int) = &PosCtrl::posPlusZ;
    void(PosCtrl::*pos_plus_x)(int) = &PosCtrl::posPlusX;
    void(PosCtrl::*pos_plus_y)(int) = &PosCtrl::posPlusY;
    void(RotCtrl::*rot_around_z)(int) = &RotCtrl::rotAroundZ;
    void(RotCtrl::*rot_around_x)(int) = &RotCtrl::rotAroundX;
    void(RotCtrl::*rot_around_y)(int) = &RotCtrl::rotAroundY;
    bool(CmdSend::*handle_pos_plus_z)(int) = &CmdSend::handlePosPlusZ;
    bool(CmdSend::*handle_pos_plus_x)(int) = &CmdSend::handlePosPlusX;
    bool(CmdSend::*handle_pos_plus_y)(int) = &CmdSend::handlePosPlusY;
    bool(CmdSend::*handle_rot_around_z)(int) = &CmdSend::handleRotAroundZ;
    bool(CmdSend::*handle_rot_around_x)(int) = &CmdSend::handleRotAroundX;
    bool(CmdSend::*handle_rot_around_y)(int) = &CmdSend::handleRotAroundY;
    connect(ui->PosCtrlWidget, pos_plus_x, ui->CmdSendWidget, handle_pos_plus_x);
    connect(ui->PosCtrlWidget, pos_plus_y, ui->CmdSendWidget, handle_pos_plus_y);
    connect(ui->PosCtrlWidget, pos_plus_z, ui->CmdSendWidget, handle_pos_plus_z);
    connect(ui->RotCtrlWidget, rot_around_x, ui->CmdSendWidget, handle_rot_around_x);
    connect(ui->RotCtrlWidget, rot_around_y, ui->CmdSendWidget, handle_rot_around_y);
    connect(ui->RotCtrlWidget, rot_around_z, ui->CmdSendWidget, handle_rot_around_z);
    /******************************* Arrow 操纵末端执行器 *******************************/

    /******************************* Frame 显示信息 *******************************/
    connect(ui->CmdSendWidget, &CmdSend::sendFailed, this, [=](){
        QString text = "Send Failed!";
        outputMsg(&text);
    });
    connect(ui->CmdSendWidget, &CmdSend::sendSuccessfully, this, [=](){
        void(JointPos::*msg_send_sig)(const int*) = &JointPos::msg_send_sig;
        void(MainWindow::*print_msg_send_sig)(const int*) = &MainWindow::sendJointMsg;
        connect(ui->JointPosWidget, msg_send_sig, this, print_msg_send_sig);
        void(CmdSend::*msg_send_move_rotate)(const int*) = &CmdSend::sendMoveRotateMsg;
        void(MainWindow::*print_msg_pos_movez_sig)(const int*) = &MainWindow::sendMoveRotateMsg;
        connect(ui->CmdSendWidget, msg_send_move_rotate, this, print_msg_pos_movez_sig);
        connect(ui->CmdSendWidget, &CmdSend::homeSignalsClicked, this, &MainWindow::initialPostureUpdate);
        connect(ui->CmdSendWidget, &CmdSend::recordingSignalsClicked, this, [=](){
            QString text = "Recording...";
            outputMsg(&text);
        });
        connect(ui->CmdSendWidget, &CmdSend::stopRecordingSignalsClicked, this, [=](){
            QString text = "Stop Recording!";
            outputMsg(&text);
        });
        connect(ui->CmdSendWidget, &CmdSend::reproductionSignalsClicked, this, [=](){
            QString text = "Reappearing...";
            outputMsg(&text);
        });
        connect(ui->CmdSendWidget, &CmdSend::homeSignalsClicked, this, [=](){
            QString text = "Initial Pose.";
            outputMsg(&text);
        });
        connect(ui->CmdSendWidget, &CmdSend::enableSignalsClicked, this, [=](){
            QString text = "Enable the arm!";
            outputMsg(&text);
        });
        connect(ui->CmdSendWidget, &CmdSend::disableSignalsClicked, this, [=](){
            QString text = "Disable the arm!";
            outputMsg(&text);
        });
//        connect(ui->CmdSendWidget, &CmdSend::armDisabled, this, [=](){
//            QString text = "Arm is disabled!";
//            outputMsg(&text);
//        });
        connect(ui->CmdSendWidget, &CmdSend::emergencyStopSignalsClicked, this, [=](){
            QString text = "Emergency Stop!";
            outputMsg(&text);
        });

    });
    connect(ui->CmdSendWidget, &CmdSend::socketNotOpen, this, [=](){
        QString text = "Socket Not Connected!";
        outputMsg(&text);
    });
    /******************************* Frame 显示信息 *******************************/

    /******************************* Socket 连接状态 *******************************/
    connect(ui->CmdSendWidget, &CmdSend::connected_arm_successfully, this, [=](){
        QString text = "Socket Connected!";
        outputMsg(&text);
    });
    connect(ui->CmdSendWidget, &CmdSend::connected_arm_failed, this, [=](){
        QString text = "Socket Connected Failed!";
        outputMsg(&text);
    });
    connect(ui->CmdSendWidget, &CmdSend::disconnected_arm_successfully, this, [=](){
        QString text = "Socket Disconnected!";
        outputMsg((&text));
    });
    /******************************* Socket 连接状态 *******************************/
}

void MainWindow::slotJVarsValueChange(int index, int value)
{
    ui->RobotShow->mRobotConfig_real.JVars[index] = value;
    ui->RobotShow->updateGL();
}

void MainWindow::initialPostureUpdate()
{
    for (int i = 0; i < 6; i++) {
        ui->RobotShow->mRobotConfig_real.JVars[i] = 0;
    }
    QVector<float> JVarsInitial = {0, 0, 0, 0, 0, 0, 0};
    QVector<float> GVarsInitial = {0, 100, 20};
    ui->JointPosWidget->setGVarsValue(GVarsInitial);
    ui->JointPosWidget->setJVarsValue(JVarsInitial);
    ui->RobotShow->updateGL();
}
void MainWindow::outputMsg(QString *text)
{
    label->setText(*text);
}

void MainWindow::sendJointMsg(const int *data)
{
    QString text = "<span style='color: blue;'>Send Message:</span><br>";
    for (int i = 0; i < 6; i = i + 3) {
        text += QString("J%1: %2 J%3: %4 J%5: %6<br>").arg(i+1).arg(data[i]).arg(i+2).arg(data[i+1]).arg(i+3).arg(data[i+2]);
    }
    text += QString("Gripper Pos: %1<br>").arg(data[6]);
    text += QString("Gripper Vel: %1<br>").arg(data[7]);
    text += QString("Gripper Force: %1<br>").arg(data[8]);

    outputMsg(&text);
}

void MainWindow::sendMoveRotateMsg(const int *data)
{
    QString text = "<span style='color: blue;'>Send Command:</span><br>";
    if (data[1] == 0x01) {
        text += QString("The robot arm moves %1 ").arg(data[3]);
        text += QString("cm<br>");
        if (data[2] == 0x01) {
            text += QString("in the positive direction ");
        } else if (data[2] == 0x00) {
            text += QString("in the negative direction ");
        }

        if (data[0] == 0x01) {
            text += QString("along the z-axis.");
        } else if (data[0] == 0x02) {
            text += QString("along the x-axis.");
        } else if (data[0] == 0x03) {
            text += QString("along the y-axis.");
        }
    } else if(data[1] == 0x02) {
        text += QString("The robot arm rotates %1 ").arg(data[3]);
        text += QString("degrees<br>");
        if (data[2] == 0x01) {
            text += QString("counterclockwise<br>");
        } else if (data[2] == 0x00) {
            text += QString("clockwize<br>");
        }

        if (data[0] == 0x01) {
            text += QString("around the z-axis.");
        } else if (data[0] == 0x02) {
            text += QString("around the x-axis.");
        } else if (data[0] == 0x03) {
            text += QString("around the y-axis.");
        }
    }
    outputMsg(&text);
}

void MainWindow::onDataReceived(const QVector<double> &data)
{
    receivedData = data;
    if (jointPosFedDialog && jointPosFedDialog->isVisible()) {
        jointPosFedDialog->onDataReceived(receivedData);
    }
}
void MainWindow::onJointPositionFeedback()
{
    jointPosFedDialog = new JointPosFed(this);
    jointPosFedDialog->setAttribute(Qt::WA_DeleteOnClose);
    jointPosFedDialog->show();

    if(!receivedData.isEmpty()) {
        jointPosFedDialog->onDataReceived(receivedData);
    }
}

void MainWindow::onGripperFeedback()
{
//    dialog->setAttribute(Qt::WA_DeleteOnClose);
//    dialog->show();
}

void MainWindow::onDeveloperModeTriggered()
{
    DeveloperMode *dialog = new DeveloperMode(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void MainWindow::onVersionTriggered()
{
    VersionInfo *dialog = new VersionInfo(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QBrush brush(QColor(102, 157, 52));
    // 画圆角矩形的背景颜色
    brush.setStyle(Qt::Dense7Pattern);
    painter.setBrush(brush); // 矩形填充颜色
    QPen transparentPen(Qt::transparent);
    painter.setPen(transparentPen);
    painter.drawRoundedRect(QRect(580, 240, 200, 141), 10, 10);
    painter.drawRoundedRect(QRect(107, 421, 455, 164), 10, 10);
    painter.drawRoundedRect(QRect(23, 341, 539, 70), 10, 10);
    // 画虚线边框
    QPen pen(QColor(18, 95, 179), 2); // 设置边框颜色和宽度
    pen.setStyle(Qt::DashLine); // 设置为虚线样式

    // 设置虚线间隔
    const QVector<qreal> dashPattern = { 3, 4 }; // 四个单位长的线段，三个单位长的空隙
    pen.setDashPattern(dashPattern);

    painter.setPen(pen);
    painter.drawRoundedRect(QRect(580, 240, 200, 141), 10, 10);
    painter.drawRoundedRect(QRect(107, 421, 455, 164), 10, 10);
    painter.drawRoundedRect(QRect(23, 341, 539, 70), 10, 10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

