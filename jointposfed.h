#ifndef JOINTPOSFED_H
#define JOINTPOSFED_H

#include <QDialog>
#include "cmdsend.h"
#include <QLabel>
#include <QVector>

namespace Ui {
class JointPosFed;
}

class JointPosFed : public QDialog
{
    Q_OBJECT

public:
    explicit JointPosFed(QWidget *parent = nullptr);
    ~JointPosFed();

private:
    Ui::JointPosFed *ui;
    CmdSend *cmdSend;
    QVector<QLabel*> jointLabels;
public slots:
    void onDataReceived(const QVector<double> &jointValues);
};

#endif // JOINTPOSFED_H
