#pragma once

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QVector>
#include <QString>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager * manager;
    QNetworkReply * currentReply;
    QNetworkReply * forecastReply;
    QByteArray currentData;
    QByteArray forecastData;
    void getCurrentData(const QString & city);
    void getForecastData(const QString & city);

private slots:
    void on_pushButton_button_clicked();
};
