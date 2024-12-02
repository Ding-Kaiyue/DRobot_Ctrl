#include "versioninfo.h"
#include "ui_versioninfo.h"

VersionInfo::VersionInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionInfo)
{
    ui->setupUi(this);
    setWindowTitle("About DRobot_Ctrl Version");
}

VersionInfo::~VersionInfo()
{
    delete ui;
}
