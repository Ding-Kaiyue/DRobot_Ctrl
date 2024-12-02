#include "jointposfed.h"
#include "ui_jointposfed.h"
#include <QByteArray>
#include <QDebug>

JointPosFed::JointPosFed(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JointPosFed)
{
    ui->setupUi(this);
    setWindowTitle("Joint Position Feedback");
//    connect(cmdSend, &CmdSend::dataReceived, this, &JointPosFed::onDataReceived);

//    jointLabels.append(ui->scrollAreaWidgetContents_J1->findChild<QLabel*>("label_J1"));
//    jointLabels.append(ui->scrollAreaWidgetContents_J2->findChild<QLabel*>("label_J2"));
//    jointLabels.append(ui->scrollAreaWidgetContents_J3->findChild<QLabel*>("label_J3"));
//    jointLabels.append(ui->scrollAreaWidgetContents_J4->findChild<QLabel*>("label_J4"));
//    jointLabels.append(ui->scrollAreaWidgetContents_J5->findChild<QLabel*>("label_J5"));
//    jointLabels.append(ui->scrollAreaWidgetContents_J6->findChild<QLabel*>("label_J6"));
}

void JointPosFed::onDataReceived(const QVector<double> &jointValues)
{
    if (jointValues.size() != 6) {
        qDebug() << "Received data does not contain 6 values";
        return;
    }

    for (int i = 0; i < 6; ++i) {
        jointLabels[i]->setText(QString::number(jointValues[i]));
    }
}
JointPosFed::~JointPosFed()
{
    delete ui;
}
