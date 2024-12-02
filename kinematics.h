#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>

class Kinematics
{
public:
    Kinematics();
    void setJointVariables(const QVector<double>& jointVars);
    void setDParameters(const QVector<double>& dParams);
    void setALengths(const QVector<double>& aLengths);
    void setAlphas(const QVector<double>& alphas);

    QVector3D calculateEndEffectorPosition() const;
    QMatrix4x4 calculateEndEffectorTransformation() const;
private:
    QVector<double> m_jointVars;  // 关节变量（角度）
    QVector<double> m_dParams;     // d 参数
    QVector<double> m_aLengths;     // a 参数
    QVector<double> m_alphas;      // alpha 参数

    QMatrix4x4 computeTransformationMatrix(int jointIndex) const;
};

#endif // KINEMATICS_H
