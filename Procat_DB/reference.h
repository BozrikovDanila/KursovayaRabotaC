#ifndef REFERENCE_H
#define REFERENCE_H

#include <QDialog>

namespace Ui {
class reference;
}

class reference : public QDialog
{
    Q_OBJECT

public:
    explicit reference(QWidget *parent = nullptr);
    ~reference();

private:
    Ui::reference *ui;

private slots:
    void on_pushButton_clicked();
};

#endif // REFERENCE_H
