#include "armgl.h"
#include "kinematics.h"
#include <cmath>
#include <QDebug>

Kinematics robotKinematics;

ArmGL::ArmGL(QWidget *parent) : RRGLWidget(parent)
{
    model.base_link_virtual = new STLFileLoader(":/stl/z1_Link00.STL", 1000);
    model.link01_virtual = new STLFileLoader(":/stl/z1_Link01.STL", 1000);
    model.link02_virtual = new STLFileLoader(":/stl/z1_Link02.STL", 1000);
    model.link03_virtual = new STLFileLoader(":/stl/z1_Link03.STL", 1000);
    model.link04_virtual = new STLFileLoader(":/stl/z1_Link04.STL", 1000);
    model.link05_virtual = new STLFileLoader(":/stl/z1_Link05.STL", 1000);
    model.link06_virtual = new STLFileLoader(":/stl/z1_Link06.STL", 1000);

    model.base_link_real = new STLFileLoader(":/stl/z1_Link00.STL", 1000);
    model.link01_real = new STLFileLoader(":/stl/z1_Link01.STL", 1000);
    model.link02_real = new STLFileLoader(":/stl/z1_Link02.STL", 1000);
    model.link03_real = new STLFileLoader(":/stl/z1_Link03.STL", 1000);
    model.link04_real = new STLFileLoader(":/stl/z1_Link04.STL", 1000);
    model.link05_real = new STLFileLoader(":/stl/z1_Link05.STL", 1000);
    model.link06_real = new STLFileLoader(":/stl/z1_Link06.STL", 1000);

    mRobotConfig_virtual.d = {0, 60, 56, 0, 56, 0, 0};
    mRobotConfig_virtual.JVars = {0, 0, 0, 0, 0, 0, 0};
    mRobotConfig_virtual.a = {0, 0, 0, 0, 0, 0, 0};
    mRobotConfig_virtual.alpha = {0, 0, 0, 0, 0, 0, 0};
    mRobotConfig_real.d = {0, 60, 56, 0, 56, 0, 0};
    mRobotConfig_real.JVars = {0, 0, 0, 0, 0, 0, 0};
    mRobotConfig_real.a = {0, 0, 0, 0, 0, 0, 0};
    mRobotConfig_real.alpha = {0, 0, 0, 0, 0, 0, 0};
    mGlobalConfig = {true, true, false, false, false, false, false, false, false, false};

    QVector<double> jointVars = {0, 0, 0, 0, 0, 0}; // 初始角度
    robotKinematics.setJointVariables(jointVars);

    // 设置 d 参数 TODO: dParams[0] 参数加的值是机械臂第三连杆弯曲部分的高度
    QVector<double> dParams = {0.1035, 0, 0, 0.072, 0, 30};     // 0, 0, 0, d4, 0, d6
    robotKinematics.setDParameters(dParams);

    // 设置 a 参数（这里假设均为0）
    QVector<double> aLengths = {0, -0.35, -0.2253, 0, 0.0982, 0};     // 0, a2, 0, 0, 0, 0
    robotKinematics.setALengths(aLengths);

    // 设置 alpha 参数（这里假设均为0）
    QVector<double> alphas = {90, 0, 90, -90, 90, 0};   // pi/2, 0, pi/2, -pi/2, pi/2, 0
    robotKinematics.setAlphas(alphas);

    QVector3D endEffectorPosition = robotKinematics.calculateEndEffectorPosition();
    qDebug() << "末端执行器位置:" << endEffectorPosition;
}

void ArmGL::drawGL()
{
    glPushMatrix();
    glScalef(1.8f, 1.8f, 1.8f); // 进行1.8倍缩放
    glTranslatef(0, 0, -200);
    glRotatef(45, 0.0, 0.0, 1.0);
    if (mGlobalConfig.isDrawGrid) {
      drawGrid();
    }
    if (mGlobalConfig.isDrawWorldCoord) {
      drawCoordinates();
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // link00
    setupColor(33, 90, 89, 175);
    model.base_link_virtual->draw();
    // link01
    if (mGlobalConfig.isDrawJoint1Coord) {
      drawSTLCoordinates(255, 0, 0, 175);
    }
    setupColor(237, 114, 63, 175);
    glTranslatef(0.0, 0.0, mRobotConfig_virtual.d[1]);      // z轴方向平移
    glRotatef(mRobotConfig_virtual.JVars[1], 0.0, 0.0, 1.0);    // 绕z轴旋转
    glTranslatef(mRobotConfig_virtual.a[1], 0.0, 0.0);      // x轴方向平移
    glRotatef(mRobotConfig_virtual.alpha[1], 1.0, 0.0, 0.0);    // 绕x轴方向旋转
    model.link01_virtual->draw();
    // 调整坐标系
    glRotatef(0, 1.0, 0.0, 0.0);

    // link02
    if (mGlobalConfig.isDrawJoint2Coord) {
      drawSTLCoordinates(255, 0, 0, 175);
    }
    setupColor(237, 160, 31, 175);
    glTranslatef(0, 0, mRobotConfig_virtual.d[2]);           // z轴方向平移
    glRotatef(mRobotConfig_virtual.JVars[2], 0.0, 1.0, 0.0);        //绕y轴旋转
    glTranslatef(mRobotConfig_virtual.a[2], 0.0, 0.0);       // x轴方向平移
    glRotatef(mRobotConfig_virtual.alpha[2], 1.0, 0.0, 0.0);        //绕x轴旋转
    model.link02_virtual->draw();

    // 调整坐标系
    glTranslatef(-350.0, 0.0, 0.0);

    // link03
    if (mGlobalConfig.isDrawJoint3Coord) {
      drawSTLCoordinates(255, 0, 0, 175);
    }
    setupColor(111, 155, 198, 175);
    glTranslatef(0, 0, mRobotConfig_virtual.d[3]);           // z轴方向平移
    glRotatef(mRobotConfig_virtual.JVars[3], 0.0, 1.0, 0.0);        //绕y轴旋转
    glTranslatef(mRobotConfig_virtual.a[3], 0.0, 0.0);       // x轴方向平移
    glRotatef(mRobotConfig_virtual.alpha[3], 1.0, 0.0, 0.0);        //绕x轴旋转
    model.link03_virtual->draw();

    // 调整坐标系
    glTranslatef(220.0, 0.0, 0.0);

    // link04
    if (mGlobalConfig.isDrawJoint4Coord) {
      drawSTLCoordinates(255, 0, 0, 175);
    }
    setupColor(255, 183, 202, 175);
    glTranslatef(0, 0, mRobotConfig_virtual.d[4]);           // z轴方向平移
    glRotatef(mRobotConfig_virtual.JVars[4], 0.0, 1.0, 0.0);        //绕y轴旋转
    glTranslatef(mRobotConfig_virtual.a[4], 0.0, 0.0);       // x轴方向平移
    glRotatef(mRobotConfig_virtual.alpha[4], 1.0, 0.0, 0.0);        //绕x轴旋转
    model.link04_virtual->draw();

    // 调整坐标系
    glTranslatef(68.0, 0.0, 0.0);

    // link05
    if (mGlobalConfig.isDrawJoint5Coord) {
      drawSTLCoordinates(255, 0, 0, 175);
    }
    setupColor(130, 134, 208, 175);
    glTranslatef(0, 0, mRobotConfig_virtual.d[5]);           // z轴方向平移
    glRotatef(mRobotConfig_virtual.JVars[5], 0.0, 0.0, 1.0);        //绕z轴旋转
    glTranslatef(mRobotConfig_virtual.a[5], 0.0, 0.0);       // x轴方向平移
    glRotatef(mRobotConfig_virtual.alpha[5], 1.0, 0.0, 0.0);        //绕x轴旋转
    model.link05_virtual->draw();

    // 调整坐标系
    glTranslatef(49.0, 0.0, 0.0);

    // link06
    if (mGlobalConfig.isDrawJoint6Coord) {
      drawSTLCoordinates(255, 0, 0, 175);
    }
    setupColor(141, 183, 153, 175);
    glTranslatef(0, 0, mRobotConfig_virtual.d[6]);           // z轴方向平移
    glRotatef(mRobotConfig_virtual.JVars[6], 1.0, 0.0, 0.0);        //绕x轴旋转
    glTranslatef(mRobotConfig_virtual.a[6], 0.0, 0.0);       // x轴方向平移
    glRotatef(mRobotConfig_virtual.alpha[6], 1.0, 0.0, 0.0);        //绕x轴旋转
    model.link06_virtual->draw();

    glTranslatef(13, 0, -172);

    // link00
    setupColor(33, 90, 89, 255);
    model.base_link_real->draw();
    // link01
    if (mGlobalConfig.isDrawJoint1Coord) {
      drawSTLCoordinates(255, 0, 0, 255);
    }
    setupColor(237, 114, 63, 255);
    glTranslatef(0.0, 0.0, mRobotConfig_real.d[1]);      // z轴方向平移
    glRotatef(mRobotConfig_real.JVars[1], 0.0, 0.0, 1.0);    // 绕z轴旋转
    glTranslatef(mRobotConfig_real.a[1], 0.0, 0.0);      // x轴方向平移
    glRotatef(mRobotConfig_real.alpha[1], 1.0, 0.0, 0.0);    // 绕x轴方向旋转
    model.link01_real->draw();
    // 调整坐标系
    glRotatef(0, 1.0, 0.0, 0.0);

    // link02
    if (mGlobalConfig.isDrawJoint2Coord) {
      drawSTLCoordinates(255, 0, 0, 255);
    }
    setupColor(237, 160, 31, 255);
    glTranslatef(0, 0, mRobotConfig_real.d[2]);           // z轴方向平移
    glRotatef(mRobotConfig_real.JVars[2], 0.0, 1.0, 0.0);        //绕y轴旋转
    glTranslatef(mRobotConfig_real.a[2], 0.0, 0.0);       // x轴方向平移
    glRotatef(mRobotConfig_real.alpha[2], 1.0, 0.0, 0.0);        //绕x轴旋转
    model.link02_real->draw();

    // 调整坐标系
    glTranslatef(-350.0, 0.0, 0.0);

    // link03
    if (mGlobalConfig.isDrawJoint3Coord) {
      drawSTLCoordinates(255, 0, 0, 255);
    }
    setupColor(111, 155, 198, 255);
    glTranslatef(0, 0, mRobotConfig_real.d[3]);           // z轴方向平移
    glRotatef(mRobotConfig_real.JVars[3], 0.0, 1.0, 0.0);        //绕y轴旋转
    glTranslatef(mRobotConfig_real.a[3], 0.0, 0.0);       // x轴方向平移
    glRotatef(mRobotConfig_real.alpha[3], 1.0, 0.0, 0.0);        //绕x轴旋转
    model.link03_real->draw();

    // 调整坐标系
    glTranslatef(220.0, 0.0, 0.0);

    // link04
    if (mGlobalConfig.isDrawJoint4Coord) {
      drawSTLCoordinates(255, 0, 0, 255);
    }
    setupColor(255, 183, 202, 255);
    glTranslatef(0, 0, mRobotConfig_real.d[4]);           // z轴方向平移
    glRotatef(mRobotConfig_real.JVars[4], 0.0, 1.0, 0.0);        //绕y轴旋转
    glTranslatef(mRobotConfig_real.a[4], 0.0, 0.0);       // x轴方向平移
    glRotatef(mRobotConfig_real.alpha[4], 1.0, 0.0, 0.0);        //绕x轴旋转
    model.link04_real->draw();

    // 调整坐标系
    glTranslatef(68.0, 0.0, 0.0);

    // link05
    if (mGlobalConfig.isDrawJoint5Coord) {
      drawSTLCoordinates(255, 0, 0, 255);
    }
    setupColor(130, 134, 208, 255);
    glTranslatef(0, 0, mRobotConfig_real.d[5]);           // z轴方向平移
    glRotatef(mRobotConfig_real.JVars[5], 0.0, 0.0, 1.0);        //绕z轴旋转
    glTranslatef(mRobotConfig_real.a[5], 0.0, 0.0);       // x轴方向平移
    glRotatef(mRobotConfig_real.alpha[5], 1.0, 0.0, 0.0);        //绕x轴旋转
    model.link05_real->draw();

    // 调整坐标系
    glTranslatef(49.0, 0.0, 0.0);

    // link06
    if (mGlobalConfig.isDrawJoint6Coord) {
      drawSTLCoordinates(255, 0, 0, 255);
    }
    setupColor(141, 183, 153, 255);
    glTranslatef(0, 0, mRobotConfig_real.d[6]);           // z轴方向平移
    glRotatef(mRobotConfig_real.JVars[6], 1.0, 0.0, 0.0);        //绕x轴旋转
    glTranslatef(mRobotConfig_real.a[6], 0.0, 0.0);       // x轴方向平移
    glRotatef(mRobotConfig_real.alpha[6], 1.0, 0.0, 0.0);        //绕x轴旋转
    model.link06_real->draw();

    glPopMatrix();
    glDisable(GL_BLEND);
}

void ArmGL::paintGL()
{
    /* 用来绘制OpenGL的窗口了，只要有更新发生，这个函数就会被调用 */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* 清除屏幕和深度缓存 */
    glPushMatrix();
    glTranslated(0, 0, z_zoom);
    glTranslated(xTran, yTran, 0);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0); /* 绕x轴旋转 */
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0); /* 绕y轴旋转 */
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0); /* 绕z轴旋转 */
    glRotated(+90.0, 1.0, 0.0, 0.0);
    drawGL();
    glPopMatrix();
}
