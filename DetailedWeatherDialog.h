#pragma once

#include <QDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

namespace Ui {
class DetailedWeatherDialog;
}

class DetailedWeatherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DetailedWeatherDialog(QByteArray * jsonData, const QString & date, const QString & city, int day, int hour, QWidget *parent = nullptr);
    ~DetailedWeatherDialog();

    void setJson(QByteArray *newJson);

private:
    Ui::DetailedWeatherDialog *ui;

    QString date, city;
    QByteArray * jsonData;
    void setupInfo();
    int day, hour;
};

