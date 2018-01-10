#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->button_read_csv, &QPushButton::clicked, this, &MainWindow::on_button_read_csv_clicked);

    connect(ui->button_read_db, &QPushButton::clicked, this, &MainWindow::on_button_read_db_clicked);

    connect(ui->button_to_csv, &QPushButton::clicked, this, &MainWindow::on_button_to_csv_clicked);

    connect(ui->button_to_db, &QPushButton::clicked, this, &MainWindow::on_button_to_db_clicked);

    connect(ui->button_settings, &QPushButton::clicked, this, &MainWindow::on_button_settings_clicked);

    controller = new Controller(ui->table, this); //удалится при очистке главного окна
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::Error(const QString &message) const
{
    ui->status_bar->showMessage(message);
}


void MainWindow::on_button_read_csv_clicked()
{
    if (controller->data_was_load)
    {
        int answer = QMessageBox::question(this,
                                           "Очистка данных",
                                           "Вы действительно хотите очистить данные?",
                                           QMessageBox::Yes|QMessageBox::No);
        if (answer == QMessageBox::No)
        {
            return;
        }
    }
    QStringList file_names = QFileDialog::getOpenFileNames(this, "Файлы CSV", "", "Файлы CSV (*.csv)");
    if (file_names.empty())
    {
        return;
    }

    if (!controller->read_csv(file_names))
    {
        Error("Не удалось прочитать CSV файлы");
    }
}

void MainWindow::on_button_read_db_clicked()
{

}

void MainWindow::on_button_to_csv_clicked()
{

}

void MainWindow::on_button_to_db_clicked()
{

}

void MainWindow::on_button_settings_clicked()
{

}

