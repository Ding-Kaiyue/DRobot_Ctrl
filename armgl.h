#ifndef ARMGL_H
#define ARMGL_H

#include "rrglwidget.h"
#include <QMatrix4x4>

struct ArmStl {
    STLFileLoader *base_link_virtual;
    STLFileLoader *link01_virtual;
    STLFileLoader *link02_virtual;
    STLFileLoader *link03_virtual;
    STLFileLoader *link04_virtual;
    STLFileLoader *link05_virtual;
    STLFileLoader *link06_virtual;

    STLFileLoader *base_link_real;
    STLFileLoader *link01_real;
    STLFileLoader *link02_real;
    STLFileLoader *link03_real;
    STLFileLoader *link04_real;
    STLFileLoader *link05_real;
    STLFileLoader *link06_real;
};

struct RotateAngle {
    int xAngle = 0;
    int yAngle = 0;
    int zAngle = 0;
};

class ArmGL : public RRGLWidget
{
    Q_OBJECT
public:
    ArmGL(QWidget *parent = nullptr);
    void drawGL() override;
protected:
    void paintGL() override;
private:
    ArmStl model;
};

#endif // ARMGL_H
