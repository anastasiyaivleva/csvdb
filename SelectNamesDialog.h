#ifndef SELECTNAMESDIALOG_H
#define SELECTNAMESDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class SelectNamesDialog;
}

/**
 * @brief Класс выбора имён таблиц для выгрузки в CSV или DB
 *
 */
class SelectNamesDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор
     *
     * @param names все имена таблиц, из которых пользователь должен выбрать нужные
     * @param parent родитель
     */
    explicit SelectNamesDialog(const QStringList& names, QWidget *parent = 0);


    /**
     * @brief Деструктор
     *
     */
    ~SelectNamesDialog();


    /**
     * @brief Вызывает диалоговое окно, ожидает выбор пользователя и возвращает выбранные им имена таблиц
     *
     * @return Выбранные имена таблиц
     */
    QStringList get_selected_names();

private:
    Ui::SelectNamesDialog *ui;
    QStandardItemModel *model;
    QStringList all_names;
};

#endif // SELECTNAMESDIALOG_H
