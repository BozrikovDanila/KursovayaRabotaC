#ifndef MANUAL_H
#define MANUAL_H

#include <QDialog>

namespace Ui {
class Manual;
}

class Manual : public QDialog
{
    Q_OBJECT

public:
    explicit Manual(QWidget *parent = nullptr);
    ~Manual();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Manual *ui;
};

#endif // MANUAL_H
