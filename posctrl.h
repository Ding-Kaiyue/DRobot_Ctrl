#ifndef POSCTRL_H
#define POSCTRL_H

#include <QWidget>
#include <QProcess>

namespace Ui {
class PosCtrl;
}

class PosCtrl : public QWidget
{
    Q_OBJECT

public:
    explicit PosCtrl(QWidget *parent = nullptr);
    ~PosCtrl();

    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *ev);

private:
    Ui::PosCtrl *ui;

signals:
    void mousePress(const QPoint &pos);
    void mouseReleased(const QPoint &pos);

    void send_successfully();
    void send_failed();

    void posPlusZ(int step_size);
    void posPlusX(int step_size);
    void posPlusY(int step_size);
};

#endif // POSCTRL_H
