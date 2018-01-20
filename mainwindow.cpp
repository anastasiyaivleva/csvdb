#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include "SelectNamesDialog.h"

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

    working_directory = QDir::current();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::Message(const QString &message) const
{
    ui->status_bar->showMessage(message, 5000);
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
        controller->reset();
    }
    QStringList csv_file_names = QFileDialog::getOpenFileNames(this, "Файлы CSV", working_directory.canonicalPath(), "Файлы CSV (*.csv);;Все файлы (*.*)");
    if (csv_file_names.empty())
    {
        return;
    }

    if (controller->read_csv(csv_file_names))
    {
        Message("Все файлы успешно прочитаны");
    }
    else
    {
        Message("Не удалось прочитать CSV файлы");
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
        controller->reset();
    }
    QString db_file_name = QFileDialog::getOpenFileName(this, "Файлы DB", working_directory.canonicalPath(), "Файлы DB (*.sqlite);;Все файлы (*.*)");
    if (db_file_name == QString(""))
    {
        return;
    }

    if (controller->read_db(db_file_name))
    {
        Message("Файл базы данных успешно прочитан");
    }
    else
    {
        Message("Не удалось прочитать файл базы данных");
    }
}

void MainWindow::button_to_csv_clicked()
{
    if (!controller->data_was_load)
    {
        Message("Данные не были загружены");
        return;
    }

    SelectNamesDialog dialog(controller->get_all_table_names());

    if (controller->convert_to_csv(dialog.get_selected_names()))
    {
        Message("Данные успешно преобразованы в формат CSV");
    }
    else
    {
        Message("Возникла ошибка при попытке преобразования данных в формат CSV");
    }
}

void MainWindow::button_to_db_clicked()
{
    if (!controller->data_was_load)
    {
        Message("Данные для конвертации не были загружены");
        return;
    }

    SelectNamesDialog dialog(controller->get_all_table_names());

    if (controller->convert_to_db(dialog.get_selected_names()))
    {
        Message("Данные успешно преобразованы в формат DB");
    }
    else
    {
        Message("Возникла ошибка при попытке преобразования данных в формат DB");
    }
}

void MainWindow::button_settings_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(nullptr, QString("Выберите рабочую директорию"), working_directory.canonicalPath());
    if (!dir.isEmpty())
    {
        working_directory = dir;
        controller->set_working_directory(dir);
    }
}

