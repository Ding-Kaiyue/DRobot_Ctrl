#include "jointpos.h"
#include "ui_jointpos.h"

JointPos::JointPos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JointPos)
{
    ui->setupUi(this);

    ui->ValueQ1->setText("0");
    ui->ValueQ2->setText("0");
    ui->ValueQ3->setText("0");
    ui->ValueQ4->setText("0");
    ui->ValueQ5->setText("0");
    ui->ValueQ6->setText("0");
    ui->angleValue->setText("0");
    ui->speedValue->setText("100");
    ui->forceValue->setText("20");

    QList<QSlider *> SliderList = this->findChildren<QSlider *>();
    for (int i = 0; i < SliderList.size(); i++) {
        QSlider *slider = SliderList.at(i);
        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateJVarsValue(int)), Qt::UniqueConnection);
    }

    connect(ui->btnOK, &QPushButton::clicked, [=](){
        const QVector<int> JVars = JointPos::getJVarsValue();
        const QVector<int> GVars = JointPos::getGVarsValue();

        int data_send[] = {1145, JVars[1], JVars[2], JVars[3], JVars[4], JVars[5], JVars[6], GVars[0], GVars[1], GVars[2]};
        int data_send_copy[9];
        memcpy(data_send_copy, data_send + 1,sizeof(int) * 9);
        emit callSendData(data_send, 10);
        emit msg_send_sig(data_send_copy);
    });
}

void JointPos::slotUpdateJVarsValue(int value)
{
    QSlider *slider = (QSlider *)sender();
    QString objectName = slider->objectName();
    QString index = objectName.at(objectName.size() - 1);
    ui->ValueQ1->setText(QString::number(ui->SliderQ1->value()));
    ui->ValueQ2->setText(QString::number(ui->SliderQ2->value()));
    ui->ValueQ3->setText(QString::number(ui->SliderQ3->value()));
    ui->ValueQ4->setText(QString::number(ui->SliderQ4->value()));
    ui->ValueQ5->setText(QString::number(ui->SliderQ5->value()));
    ui->ValueQ6->setText(QString::number(ui->SliderQ6->value()));
    ui->angleValue->setText(QString::number(ui->sliderAngle->value()));
    ui->speedValue->setText(QString::number(ui->sliderSpeed->value()));
    ui->forceValue->setText(QString::number(ui->sliderForce->value()));
    emit sigJointValueChanged(index.toInt(), value);
}

void JointPos::setGVarsValue(QVector<float> GVars)
{
    ui->sliderAngle->setValue(GVars[0]);
    ui->sliderSpeed->setValue(GVars[1]);
    ui->sliderForce->setValue(GVars[2]);
}

QVector<int> JointPos::getJVarsValue() const
{
    QVector<int> JVars;
    JVars.append(0);
    JVars.append(ui->SliderQ1->value());
    JVars.append(ui->SliderQ2->value());
    JVars.append(ui->SliderQ3->value());
    JVars.append(ui->SliderQ4->value());
    JVars.append(ui->SliderQ5->value());
    JVars.append(ui->SliderQ6->value());
    return JVars;
}

QVector<int> JointPos::getGVarsValue() const
{
    QVector<int> GVars;
    GVars.append(ui->sliderAngle->value());
    GVars.append(ui->sliderSpeed->value());
    GVars.append(ui->sliderForce->value());
    return GVars;
}

void JointPos::setJVarsValue(QVector<float> JVars)
{
    // 0号表示底座
    ui->SliderQ1->setValue(JVars[1]);
    ui->SliderQ2->setValue(JVars[2]);
    ui->SliderQ3->setValue(JVars[3]);
    ui->SliderQ4->setValue(JVars[4]);
    ui->SliderQ5->setValue(JVars[5]);
    ui->SliderQ6->setValue(JVars[6]);
}


JointPos::~JointPos()
{
    delete ui;
}
