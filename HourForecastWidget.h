#pragma once

#include <QWidget>
#include <QString>
#include <QPixmap>
#include <QSize>
#include <QLabel>
#include <QMouseEvent>
#include <QJsonObject>
#include <QEvent>

namespace Ui {
class HourForecastWidget;
}

class HourForecastWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HourForecastWidget(QWidget *parent = nullptr);
    ~HourForecastWidget();

    QString getLocalTime() const;
    void setLocalTime(const QString & time);
    double getTemp() const;
    void setTemp(double temp);
    void setLabelIcon(const QPixmap & pix);
    const QSize getImageLabelSize() const;
    QLabel * getImageLabel();

    void setJson(QJsonDocument *newJson);
    void setDay(int newDay);
    void setHour(int newHour);

    void setCity(const QString &newCity);

    QByteArray *getJsonData() const;
    void setJsonData(QByteArray *newJsonData);

protected:
    bool eventFilter(QObject * object, QEvent * event);

private:
    Ui::HourForecastWidget *ui;
    QByteArray * jsonData;
    int day, hour;
    QString city;
};

