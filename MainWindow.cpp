#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QUrl>
#include <QString>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>
#include <QMessageBox>
#include <QJsonDocument>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QUrlQuery>
#include <QDate>
#include <QWidget>
#include <QScrollArea>
#include <QEvent>
#include <QDateTime>

#include "HourForecastWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::getCurrentData(const QString &city)
{
    for(int i=0; i<ui->tabWidget_forecast->count(); i++)
        ui->tabWidget_forecast->removeTab(i);

    QUrl url("http://api.weatherapi.com/v1/current.json");
    QUrlQuery query;
    query.addQueryItem("key", "25d23007ea95452996582509222407");
    query.addQueryItem("q", city);
    url.setQuery(query);
    QNetworkRequest req;
    req.setUrl(url);
    qDebug()<<req.url();

    currentReply = manager->get(req);
    connect(currentReply, &QNetworkReply::finished, this, [this, city](){
        currentData.clear();
        currentData.append(currentReply->readAll());
        if(currentReply->error() != QNetworkReply::NoError)
        {
            QMessageBox::warning(this,"Blad ladowania danych",QString("Request[Error] : %1").arg(currentReply->errorString()));
        }
        else{
            QJsonDocument document = QJsonDocument::fromJson(currentData);
            QJsonObject object = document.object();
            QJsonValue value = object.value("current");
            ui->label_city->setText(city);
            ui->label_lastUpdate->setText("Ostatnia aktualizacja: " + value.toObject().value("last_updated").toString());
            ui->label_temp->setText(QString::number(value.toObject().value("temp_c").toDouble())+ " stopni C");
            QString piclink = value.toObject().value("condition").toObject().value("icon").toString();
            piclink.insert(0, "http:");
            QNetworkRequest link{QUrl(piclink)};
            currentReply = manager->get(link);

            connect(currentReply, &QNetworkReply::finished, this, [this](){
                QPixmap * pix = new QPixmap;
                pix->loadFromData(currentReply->readAll());
                QPixmap temp = pix->scaled(ui->label_icon->size());
                ui->label_icon->setPixmap(temp);
            });

        }
    });
}

void MainWindow::getForecastData(const QString &city)
{
    for(int i=0; i<ui->tabWidget_forecast->count(); i++)
        ui->tabWidget_forecast->removeTab(i);

    QUrl url("http://api.weatherapi.com/v1/forecast.json");
    QUrlQuery query;
    query.addQueryItem("key", "25d23007ea95452996582509222407");
    query.addQueryItem("q", city);
    query.addQueryItem("aqi", "no");
    query.addQueryItem("alerts", "no");
    query.addQueryItem("days", QString::number(3));
    url.setQuery(query);
    QNetworkRequest req;
    req.setUrl(url);

    forecastReply = manager->get(req);

    connect(forecastReply, &QNetworkReply::finished, this, [this, city](){
        forecastData.clear();
        forecastData.append(forecastReply->readAll());
        if(forecastReply->error() != QNetworkReply::NoError)
        {
            QMessageBox::warning(this,"Blad ladowania danych",QString("Request[Error] : %1").arg(forecastReply->errorString()));
        }
        else{
            QJsonDocument document = QJsonDocument::fromJson(forecastData);
            QJsonObject object = document.object();
            QJsonValue value = object.value("forecast").toObject().value("forecastday");
            QJsonArray array = value.toArray();
            int day = 0;
            int hour = 0;
            for(const auto & v : array)
            {
                QVBoxLayout * layout = new QVBoxLayout();
                QJsonArray hourArray = v.toObject().value("hour").toArray();
                QDate date = QDate::currentDate();
                QWidget * widget = new QWidget;

                for(const auto & h : hourArray)
                {
                    HourForecastWidget * forecastWidget = new HourForecastWidget;

                    forecastWidget->setDay(day);
                    forecastWidget->setHour(hour);
                    forecastWidget->setJsonData(&forecastData);
                    forecastWidget->setCity(city);
                    QString stringTime = h.toObject().value("time").toString();
                    forecastWidget->setLocalTime(stringTime);
                    forecastWidget->setTemp(h.toObject().value("temp_c").toDouble());

                    QString piclink = h.toObject().value("condition").toObject().value("icon").toString();
                    piclink.insert(0, "https:");

                    QNetworkRequest link{QUrl(piclink)};
                    forecastReply = manager->get(link);

                    QEventLoop loop;
                    connect(forecastReply, &QNetworkReply::finished, this, [this, forecastWidget, & loop](){
                        QPixmap * pix = new QPixmap;
                        pix->loadFromData(forecastReply->readAll());
                        QPixmap temp = pix->scaled(forecastWidget->getImageLabel()->size());
                        forecastWidget->setLabelIcon(temp);
                        loop.quit();
                    });
                    loop.exec();

                    layout->addWidget(forecastWidget);
                    hour++;
                }
                widget->setLayout(layout);
                QScrollArea * scrollArea = new QScrollArea;
                scrollArea->setWidget(widget);

                ui->tabWidget_forecast->insertTab(day, scrollArea, date.addDays(day).toString());
                day++;
            }
        }
    });
}

void MainWindow::on_pushButton_button_clicked()
{
    if(ui->lineEdit_city->text().isEmpty())
    {
        QMessageBox::warning(this, "Brak danych", "Nie wprowadzono zadnego tekstu!",QMessageBox::Ok);
    }
    else{
        for(int i=0; i<ui->tabWidget_forecast->count(); i++)
            ui->tabWidget_forecast->removeTab(i);

        QString city = ui->lineEdit_city->text();
        getCurrentData(city);
        getForecastData(city);
    }
}
