#ifndef JOINTPOS_H
#define JOINTPOS_H

#include <QWidget>

namespace Ui {
class JointPos;
}

class JointPos : public QWidget
{
    Q_OBJECT

public:
    explicit JointPos(QWidget *parent = nullptr);
    ~JointPos();

    void setGVarsValue(QVector<float> GVars);
    void setJVarsValue(QVector<float> JVars);
    QVector<int> getGVarsValue() const;
    QVector<int> getJVarsValue() const;

private slots:
    void slotUpdateJVarsValue(int value);

signals:
    void callSendData(const int* data, int length);
    void msg_send_sig(const int* data);
    void sigJointValueChanged(const int index, int value);
private:
    Ui::JointPos *ui;
};

#endif // JOINTPOS_H
