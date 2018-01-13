#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->button_read_csv, &QPushButton::clicked, this, &MainWindow::button_read_csv_clicked);

    connect(ui->button_read_db, &QPushButton::clicked, this, &MainWindow::button_read_db_clicked);

    connect(ui->button_to_csv, &QPushButton::clicked, this, &MainWindow::button_to_csv_clicked);

    connect(ui->button_to_db, &QPushButton::clicked, this, &MainWindow::button_to_db_clicked);

    connect(ui->button_settings, &QPushButton::clicked, this, &MainWindow::button_settings_clicked);

    controller = new Controller(ui->tab_widget, this); //удалится при очистке главного окна
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::Error(const QString &message) const
{
    ui->status_bar->showMessage(message);
}


void MainWindow::button_read_csv_clicked()
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
        controller->clear();
    }
    QStringList csv_file_names = QFileDialog::getOpenFileNames(this, "Файлы CSV", "", "Файлы CSV (*.csv)");
    if (csv_file_names.empty())
    {
        return;
    }

    if (!controller->read_csv(csv_file_names))
    {
        Error("Не удалось прочитать CSV файлы");
    }
}

void MainWindow::button_read_db_clicked()
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
        controller->clear();
    }
    QString db_file_name = QFileDialog::getOpenFileName(this, "Файлы DB", "", "Файлы DB (*.sqlite)");
    if (db_file_name == QString(""))
    {
        return;
    }

    if (!controller->read_db(db_file_name))
    {
        Error("Не удалось прочитать файл базы данных");
    }
}

void MainWindow::button_to_csv_clicked()
{
    //Добавить выбор имён файлов

    if (!controller->data_was_load)
    {
        Error("Данные для конвертации не были загружены");
        return;
    }

    if (!controller->convert_to_csv(controller->get_all_table_names()))
    {
        Error("Возникла ошибка при попытке преобразования данных в CSV");
    }
}

void MainWindow::button_to_db_clicked()
{
    //Добавить выбор имён файлов

    if (!controller->data_was_load)
    {
        Error("Данные для конвертации не были загружены");
        return;
    }

    if (!controller->convert_to_db(controller->get_all_table_names()))
    {
        Error("Возникла ошибка при попытке преобразования данных в DB");
    }
}

void MainWindow::button_settings_clicked()
{
    //Сделать
}
