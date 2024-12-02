#include "kinematics.h"
#include <QDebug>

Kinematics::Kinematics()
{
    m_jointVars.resize(6);  // 初始化关节角度
    m_dParams.resize(6);     // 初始化 d 参数
    m_aLengths.resize(6);    // 初始化 a 参数
    m_alphas.resize(6);       // 初始化 alpha 参数
}

void Kinematics::setJointVariables(const QVector<double>& jointVars) {
    m_jointVars = jointVars;
}

void Kinematics::setDParameters(const QVector<double>& dParams) {
    m_dParams = dParams;
}

void Kinematics::setALengths(const QVector<double>& aLengths) {
    m_aLengths = aLengths;
}

void Kinematics::setAlphas(const QVector<double>& alphas) {
    m_alphas = alphas;
}

QMatrix4x4 Kinematics::computeTransformationMatrix(int jointIndex) const {
    double theta = m_jointVars[jointIndex];
    double d = m_dParams[jointIndex];
    double a = m_aLengths[jointIndex];
    double alpha = m_alphas[jointIndex];

    QMatrix4x4 transformationMatrix;
    transformationMatrix.setToIdentity();

    transformationMatrix(0, 0) = qCos(qDegreesToRadians(theta));
    transformationMatrix(0, 1) = -qSin(qDegreesToRadians(theta)) * qCos(qDegreesToRadians(alpha));
    transformationMatrix(0, 2) = qSin(qDegreesToRadians(theta)) * qSin(qDegreesToRadians(alpha));
    transformationMatrix(0, 3) = a * qCos(qDegreesToRadians(theta));

    transformationMatrix(1, 0) = qSin(qDegreesToRadians(theta));
    transformationMatrix(1, 1) = qCos(qDegreesToRadians(theta)) * qCos(qDegreesToRadians(alpha));
    transformationMatrix(1, 2) = -qCos(qDegreesToRadians(theta)) * qSin(qDegreesToRadians(alpha));
    transformationMatrix(1, 3) = a * qSin(qDegreesToRadians(theta));

    transformationMatrix(2, 1) = qSin(qDegreesToRadians(alpha));
    transformationMatrix(2, 2) = qCos(qDegreesToRadians(alpha));
    transformationMatrix(2, 3) = d;

    transformationMatrix(3, 3) = 1.0;

    return transformationMatrix;
}

QMatrix4x4 Kinematics::calculateEndEffectorTransformation() const {
    QMatrix4x4 T;
    T.setToIdentity();  // 将矩阵设置为单位矩阵

    for (int i = 0; i < m_jointVars.size(); ++i) {
        T *= computeTransformationMatrix(i);
        qDebug() << i << ":" << T;
    }

    return T;
}

QVector3D Kinematics::calculateEndEffectorPosition() const {
    QMatrix4x4 T = calculateEndEffectorTransformation();
    return QVector3D(T(0, 3), T(1, 3), T(2, 3));
}

