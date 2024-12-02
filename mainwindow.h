#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QApplication>
#include <QLabel>
#include "cmdsend.h"
#include "jointpos.h"
#include "jointposfed.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QLabel *label = new QLabel("Welcome to use DRobot! ");

    void paintEvent(QPaintEvent *);
private:
    Ui::MainWindow *ui;
    JointPos *jointPos;
    CmdSend *cmdSend;
    JointPosFed *jointPosFedDialog;
    QVector<double> receivedData;
private slots:
    void outputMsg(QString *text);
    void sendJointMsg(const int *data);
    void sendMoveRotateMsg(const int *data);
    void slotJVarsValueChange(int index, int value);
    void initialPostureUpdate();

    void onDeveloperModeTriggered();
    void onJointPositionFeedback();
    void onDataReceived(const QVector<double> &data);
    void onGripperFeedback();
    void onVersionTriggered();
};
#endif // MAINWINDOW_H
