#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Controller.h"
#include <QDir>

namespace Ui {
class MainWindow;
}

/**
 * @brief Главное окно приложения
 *
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * @brief Конструктор
     *
     * @param parent родитель
     */
    explicit MainWindow(QWidget *parent = 0);


    /**
     * @brief Деструктор класса MainWindow
     *
     */
    ~MainWindow();


private slots:
    /**
     * @brief Функция обработки нажатия на кнопку "Загрузить CSV"
     *
     */
    void button_read_csv_clicked();


    /**
     * @brief Функция обработки нажатия на кнопку "Загрузить DB"
     *
     */
    void button_read_db_clicked();


    /**
     * @brief Функция обработки нажатия на кнопку "Импортировать в CSV"
     *
     */
    void button_to_csv_clicked();


    /**
     * @brief Функция обработки нажатия на кнопку "Импортировать в DB"
     *
     */
    void button_to_db_clicked();


    /**
     * @brief Функция обработки нажатия на кнопку "Выбрать рабочую директорию"
     *
     */
    void button_settings_clicked();

private:
    Ui::MainWindow *ui;

    Controller* controller;
    QDir working_directory;

    /**
     * @brief Функция вывода сообщения в status_bar
     *
     * @param message сообщение
     */
    void Message(const QString& message) const;
};

#endif // MAINWINDOW_H
