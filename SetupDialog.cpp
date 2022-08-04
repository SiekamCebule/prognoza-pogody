#include "SetupDialog.h"
#include "ui_SetupDialog.h"

SetupDialog::SetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupDialog)
{
    ui->setupUi(this);
}

SetupDialog::~SetupDialog()
{
    delete ui;
}

QString SetupDialog::getCity() const
{
    return ui->lineEdit_city->text();
}

short SetupDialog::getDays() const
{
    return ui->spinBox_days->value();
}

void SetupDialog::on_pushButton_ok_clicked()
{
    accept();
}

