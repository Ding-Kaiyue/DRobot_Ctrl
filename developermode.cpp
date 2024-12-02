#include "developermode.h"
#include "ui_developermode.h"

DeveloperMode::DeveloperMode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeveloperMode)
{
    ui->setupUi(this);
    setWindowTitle("DeveloperMode");
}

DeveloperMode::~DeveloperMode()
{
    delete ui;
}
