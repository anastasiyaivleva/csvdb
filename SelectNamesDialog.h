#ifndef SELECTNAMESDIALOG_H
#define SELECTNAMESDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class SelectNamesDialog;
}

class SelectNamesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectNamesDialog(const QStringList& names, QWidget *parent = 0);
    ~SelectNamesDialog();

    QStringList get_selected_names();

private:
    Ui::SelectNamesDialog *ui;
    QStandardItemModel *model;
    QStringList all_names;
};

#endif // SELECTNAMESDIALOG_H
