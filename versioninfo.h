#ifndef VERSIONINFO_H
#define VERSIONINFO_H

#include <QDialog>

namespace Ui {
class VersionInfo;
}

class VersionInfo : public QDialog
{
    Q_OBJECT

public:
    explicit VersionInfo(QWidget *parent = nullptr);
    ~VersionInfo();

private:
    Ui::VersionInfo *ui;
};

#endif // VERSIONINFO_H
