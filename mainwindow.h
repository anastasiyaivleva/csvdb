#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Controller.h"

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
    void on_button_read_csv_clicked();

    void on_button_read_db_clicked();

    void on_button_to_csv_clicked();

    void on_button_to_db_clicked();

    void on_button_settings_clicked();

private:
    Ui::MainWindow *ui;
    Controller* controller;

    void Error(const QString& message) const;
};

#endif // MAINWINDOW_H
