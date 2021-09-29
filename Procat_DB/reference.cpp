#include "reference.h"
#include "ui_reference.h"

reference::reference(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reference)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint| Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

reference::~reference()
{
    delete ui;
}

void reference::on_pushButton_clicked()
{
    close();
}
