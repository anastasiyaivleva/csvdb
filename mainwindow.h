#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Controller.h"
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void button_read_csv_clicked();

    void button_read_db_clicked();

    void button_to_csv_clicked();

    void button_to_db_clicked();

    void button_settings_clicked();

private:
    Ui::MainWindow *ui;

    Controller* controller;
    QDir working_directory;

    void Message(const QString& message) const;
};

#endif // MAINWINDOW_H
