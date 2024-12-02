#ifndef DEVELOPERMODE_H
#define DEVELOPERMODE_H

#include <QDialog>

namespace Ui {
class DeveloperMode;
}

class DeveloperMode : public QDialog
{
    Q_OBJECT

public:
    explicit DeveloperMode(QWidget *parent = nullptr);
    ~DeveloperMode();

private:
    Ui::DeveloperMode *ui;
};

#endif // DEVELOPERMODE_H
