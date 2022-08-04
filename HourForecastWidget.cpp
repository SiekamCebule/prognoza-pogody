#include "HourForecastWidget.h"
#include "ui_HourForecastWidget.h"
#include "DetailedWeatherDialog.h"

HourForecastWidget::HourForecastWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HourForecastWidget)
{
    ui->setupUi(this);
    ui->label_temp->installEventFilter(this);

}

HourForecastWidget::~HourForecastWidget()
{
    delete ui;
}

QString HourForecastWidget::getLocalTime() const
{
    return ui->label_localtime->text();
}

void HourForecastWidget::setLocalTime(const QString &time)
{
    ui->label_localtime->setText(time);
}

double HourForecastWidget::getTemp() const
{
    return ui->label_temp->text().toDouble();
}

void HourForecastWidget::setTemp(double temp)
{
    ui->label_temp->setText(QString::number(temp));
}

void HourForecastWidget::setLabelIcon(const QPixmap &pix)
{
    ui->label_img->clear();
    ui->label_img->setPixmap(pix);
}

const QSize HourForecastWidget::getImageLabelSize() const
{
    return ui->label_img->size();
}

QLabel *HourForecastWidget::getImageLabel()
{
    return ui->label_img;
}
void HourForecastWidget::setHour(int newHour)
{
    hour = newHour;
}

bool HourForecastWidget::eventFilter(QObject *object, QEvent *event)
{
    if(object == ui->label_temp && event->type() == QEvent::MouseButtonDblClick)
    {
        DetailedWeatherDialog dialog(jsonData, ui->label_localtime->text(), city, day, hour, this);
        dialog.exec();
    }
    return false;
}

QByteArray *HourForecastWidget::getJsonData() const
{
    return jsonData;
}

void HourForecastWidget::setJsonData(QByteArray *newJsonData)
{
    jsonData = newJsonData;
}

void HourForecastWidget::setCity(const QString &newCity)
{
    city = newCity;
}

void HourForecastWidget::setDay(int newDay)
{
    day = newDay;
}
