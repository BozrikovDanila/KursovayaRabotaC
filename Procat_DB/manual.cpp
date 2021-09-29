#include "manual.h"
#include "ui_manual.h"

Manual::Manual(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Manual)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

Manual::~Manual()
{
    delete ui;
}

void Manual::on_pushButton_clicked()
{
    close();
}
