#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    multiTab = new MultiRowTabWidget(3, this);

    setCentralWidget(multiTab);

    for (uint r = 0; r < 3; ++r) {
        for (uint c = 0; c < 4; ++c) {
            QLabel *label = new QLabel(QString("Content for Row %1, Tab %2").arg(r).arg(c));
            label->setAlignment(Qt::AlignCenter);
            multiTab->addTab(label, QString("R%1-Tab%2").arg(r).arg(c), r);
        }
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}
