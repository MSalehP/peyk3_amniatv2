#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "encryption.h"
#include "utils.h"

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    performTests();
}

void MainWindow::performTests() {
    const int numMessages = 30;
    const int messageLength = 128;

    qint64 totalAesEncryptionTime = 0;
    qint64 totalRsaEncryptionTime = 0;

    for (int i = 0; i < numMessages; ++i) {
        qCritical() << "Processing message number: " << i + 1;
        std::string randomMessage = generateRandomString(messageLength);

        // Measure AES encryption time
        qint64 aesEncryptionTime = measureAESTime(randomMessage, true);
        totalAesEncryptionTime += aesEncryptionTime;

        // Measure RSA encryption time
        qint64 rsaEncryptionTime = measureRSATime(randomMessage, true);
        totalRsaEncryptionTime += rsaEncryptionTime;
    }

    // Calculate average encryption times
    qint64 averageAesEncryptionTime = totalAesEncryptionTime / numMessages;
    qint64 averageRsaEncryptionTime = totalRsaEncryptionTime / numMessages;

    // Create QBarSet for average times
    QBarSet *averageTimesSet = new QBarSet("Average Encryption Times");
    *averageTimesSet << averageAesEncryptionTime << averageRsaEncryptionTime;

    // Create labels for the categories
    QStringList categories;
    categories << "AES" << "RSA";

    // Create QBarSeries and add the QBarSet
    QBarSeries *series = new QBarSeries();
    series->append(averageTimesSet);

    // Create a chart and set up the series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Average Encryption Times (AES vs RSA)");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Configure X-axis with categories
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Configure Y-axis
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Time (ns)");
    axisY->setLabelFormat("%.0f");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Display the chart in the QChartView
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Clear any existing charts and add the new one
    QLayoutItem *item;
    while ((item = ui->chartLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    ui->chartLayout->addWidget(chartView);
}


