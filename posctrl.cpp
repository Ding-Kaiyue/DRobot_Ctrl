#include "posctrl.h"
#include "ui_posctrl.h"
#include <QMouseEvent>
#include <QDebug>
#include <QPixmap>
//#include "cmdsend.h"

//CmdSend cmd_send_pos_ctrl;

PosCtrl::PosCtrl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PosCtrl)
{
    ui->setupUi(this);

    connect(this, &PosCtrl::mousePress, this, [=](const QPoint &pos){
        if (pos.x() < 24 && pos.x() > 0 && pos.y() < 60 && pos.y() > 0) {
            QPixmap pressedPixmap(":/img/up_yellow.jpg");
            ui->pos_up->setPixmap(pressedPixmap);
            emit posPlusZ(2);
        } else if (pos.x() < 92 && pos.x() > 68 && pos.y() < 70 && pos.y() > 10) {
            QPixmap pressedPixmap(":/img/up_yellow.jpg");
            ui->pos_forward->setPixmap(pressedPixmap);
            emit posPlusX(2);
        } else if (pos.x() < 160 && pos.x() > 136 && pos.y() < 60 && pos.y() > 0) {
            QPixmap pressedPixmap(":/img/down_yellow.jpg");
            ui->pos_down->setPixmap(pressedPixmap);
            emit posPlusZ(-2);
        } else if (pos.x() < 60 && pos.x() > 0 && pos.y() < 92 && pos.y() > 68) {
            QPixmap pressedPixmap(":/img/left_yellow.jpg");
            ui->pos_left->setPixmap(pressedPixmap);
            emit posPlusY(-2);
        } else if (pos.x() < 160 && pos.x() > 100 && pos.y() < 92 && pos.y() > 68) {
            QPixmap pressedPixmap(":/img/right_yellow.jpg");
            ui->pos_right->setPixmap(pressedPixmap);
            emit posPlusY(2);
        } else if (pos.x() < 92 && pos.x() > 68 && pos.y() < 150 && pos.y() > 90) {
            QPixmap pressedPixmap(":/img/down_yellow.jpg");
            ui->pos_backward->setPixmap(pressedPixmap);
            emit posPlusX(-2);
        }
    });
    connect(this, &PosCtrl::mouseReleased, this, [=](const QPoint &pos){
        if (pos.x() < 24 && pos.x() > 0 && pos.y() < 60 && pos.y() > 0) {
            QPixmap pressedPixmap(":/img/up_grey.jpg");
            ui->pos_up->setPixmap(pressedPixmap);
        } else if (pos.x() < 92 && pos.x() > 68 && pos.y() < 70 && pos.y() > 10) {
            QPixmap pressedPixmap(":/img/up_grey.jpg");
            ui->pos_forward->setPixmap(pressedPixmap);
        } else if (pos.x() < 160 && pos.x() > 136 && pos.y() < 60 && pos.y() > 0) {
            QPixmap pressedPixmap(":/img/down_grey.jpg");
            ui->pos_down->setPixmap(pressedPixmap);
        } else if (pos.x() < 60 && pos.x() > 0 && pos.y() < 92 && pos.y() > 68) {
            QPixmap pressedPixmap(":/img/left_grey.jpg");
            ui->pos_left->setPixmap(pressedPixmap);
        } else if (pos.x() < 160 && pos.x() > 100 && pos.y() < 92 && pos.y() > 68) {
            QPixmap pressedPixmap(":/img/right_grey.jpg");
            ui->pos_right->setPixmap(pressedPixmap);
        } else if (pos.x() < 92 && pos.x() > 68 && pos.y() < 150 && pos.y() > 90) {
            QPixmap pressedPixmap(":/img/down_grey.jpg");
            ui->pos_backward->setPixmap(pressedPixmap);
        }
    });
}

void PosCtrl::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) {
        emit mousePress(QPoint(ev->x(), ev->y()));
    }
}

void PosCtrl::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) {
        emit mouseReleased(QPoint(ev->x(), ev->y()));
    }
}


PosCtrl::~PosCtrl()
{
    delete ui;
}
