#include "DetailedWeatherDialog.h"
#include "ui_DetailedWeatherDialog.h"
#include <QJsonValue>
#include <QJsonArray>

DetailedWeatherDialog::DetailedWeatherDialog(QByteArray * jsonData, const QString & date, const QString & city, int day, int hour, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetailedWeatherDialog),
    date(date),
    city(city),
    jsonData(jsonData),
    day(day),
    hour(hour)
{
    ui->setupUi(this);
    setupInfo();
}
DetailedWeatherDialog::~DetailedWeatherDialog()
{
    delete ui;
}

void DetailedWeatherDialog::setJson(QByteArray *newJson)
{
    jsonData = newJson;
}

void DetailedWeatherDialog::setupInfo()
{
    QJsonDocument document = QJsonDocument::fromJson(*jsonData);
    QJsonObject object = document.object();
    QJsonValue value = object.value("forecast").toObject().value("forecastday");
    QJsonArray array = value.toArray();
    int m_day = 0, m_hour = 0;
    for(const auto & v : array)
    {
        QJsonArray hourArray = v.toObject().value("hour").toArray();
        for(const auto & h : hourArray)
        {
            if(m_day == day & m_hour == hour)
            {
                ui->label_title->setText(QString("Szczegolowa pogoda - ").toUtf8() + city);
                ui->label_hour->setText(h.toObject().value("time").toString());
                ui->label_temp->setText(QString::number(h.toObject().value("temp_c").toDouble()));
                ui->label_feelTemp->setText(QString::number(h.toObject().value("feelslike_c").toDouble()));
                ui->label_windSpeed->setText(QString::number(h.toObject().value("wind_kph").toDouble()) + "km/h");
                ui->label_humidity->setText(QString::number(h.toObject().value("humidity").toDouble()) + "%");
                ui->label_rainChance->setText(QString::number(h.toObject().value("chance_of_rain").toDouble()) + "%");
                ui->label_snowChance->setText(QString::number(h.toObject().value("chance_of_snow").toDouble()) + "%");
            }
            m_hour++;
        }
        m_day++;
    }
}
