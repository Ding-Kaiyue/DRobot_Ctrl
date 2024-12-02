#include "rotctrl.h"
#include "ui_rotctrl.h"
#include <QMouseEvent>
#include <QPixmap>

RotCtrl::RotCtrl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RotCtrl)
{
    ui->setupUi(this);

    connect(this, &RotCtrl::mousePress, this, [=](const QPoint &pos){
        if (pos.x() < 60 && pos.x() > 0 && pos.y() < 60 && pos.y() > 0) {
            QPixmap pressedPixmap(":/img/left_down_yellow.jpg");
            ui->rot_left_down->setPixmap(pressedPixmap);
            emit rotAroundZ(2);
        } else if (pos.x() < 92 && pos.x() > 68 && pos.y() < 70 && pos.y() > 10) {
            QPixmap pressedPixmap(":/img/up_yellow.jpg");
            ui->rot_forward->setPixmap(pressedPixmap);
            emit rotAroundX(2);
        } else if (pos.x() < 160 && pos.x() > 100 && pos.y() < 60 && pos.y() > 0) {
            QPixmap pressedPixmap(":/img/right_down_yellow.jpg");
            ui->rot_right_down->setPixmap(pressedPixmap);
            emit rotAroundZ(-2);
        } else if (pos.x() < 60 && pos.x() > 0 && pos.y() < 92 && pos.y() > 68) {
            QPixmap pressedPixmap(":/img/left_yellow.jpg");
            ui->rot_left->setPixmap(pressedPixmap);
            emit rotAroundY(2);
        } else if (pos.x() < 160 && pos.x() > 100 && pos.y() < 92 && pos.y() > 68) {
            QPixmap pressedPixmap(":/img/right_yellow.jpg");
            ui->rot_right->setPixmap(pressedPixmap);
            emit rotAroundY(-2);
        } else if (pos.x() < 92 && pos.x() > 68 && pos.y() < 150 && pos.y() > 90) {
            QPixmap pressedPixmap(":/img/down_yellow.jpg");
            ui->rot_backward->setPixmap(pressedPixmap);
            emit rotAroundX(-2);
        }
    });
    connect(this, &RotCtrl::mouseReleased, this, [=](const QPoint &pos){
        if (pos.x() < 60 && pos.x() > 0 && pos.y() < 60 && pos.y() > 0) {
            QPixmap pressedPixmap(":/img/left_down_grey.jpg");
            ui->rot_left_down->setPixmap(pressedPixmap);
        } else if (pos.x() < 92 && pos.x() > 68 && pos.y() < 70 && pos.y() > 10) {
            QPixmap pressedPixmap(":/img/up_grey.jpg");
            ui->rot_forward->setPixmap(pressedPixmap);
        } else if (pos.x() < 160 && pos.x() > 100 && pos.y() < 60 && pos.y() > 0) {
            QPixmap pressedPixmap(":/img/right_down_grey.jpg");
            ui->rot_right_down->setPixmap(pressedPixmap);
        } else if (pos.x() < 60 && pos.x() > 0 && pos.y() < 92 && pos.y() > 68) {
            QPixmap pressedPixmap(":/img/left_grey.jpg");
            ui->rot_left->setPixmap(pressedPixmap);
        } else if (pos.x() < 160 && pos.x() > 100 && pos.y() < 92 && pos.y() > 68) {
            QPixmap pressedPixmap(":/img/right_grey.jpg");
            ui->rot_right->setPixmap(pressedPixmap);
        } else if (pos.x() < 92 && pos.x() > 68 && pos.y() < 150 && pos.y() > 90) {
            QPixmap pressedPixmap(":/img/down_grey.jpg");
            ui->rot_backward->setPixmap(pressedPixmap);
        }
    });
}

void RotCtrl::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) {
        emit mousePress(QPoint(ev->x(), ev->y()));
    }
}

void RotCtrl::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) {
        emit mouseReleased(QPoint(ev->x(), ev->y()));
    }
}

RotCtrl::~RotCtrl()
{
    delete ui;
}
