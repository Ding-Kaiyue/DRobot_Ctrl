#ifndef ROTCTRL_H
#define ROTCTRL_H

#include <QWidget>

namespace Ui {
class RotCtrl;
}

class RotCtrl : public QWidget
{
    Q_OBJECT

public:
    explicit RotCtrl(QWidget *parent = nullptr);
    ~RotCtrl();

    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *ev);


private:
    Ui::RotCtrl *ui;

signals:
    void mousePress(const QPoint &pos);
    void mouseReleased(const QPoint &pos);

    void rotAroundZ(int value);
    void rotAroundX(int value);
    void rotAroundY(int value);
};

#endif // ROTCTRL_H
