#pragma once

#include <QDialog>
#include <QString>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupDialog(QWidget *parent = nullptr);
    ~SetupDialog();

    QString getCity() const;
    short getDays() const;


private slots:
    void on_pushButton_ok_clicked();

private:
    Ui::SetupDialog *ui;
};

