#include "SelectNamesDialog.h"
#include "ui_SelectNamesDialog.h"
#include <QStandardItemModel>
#include <QMessageBox>

SelectNamesDialog::SelectNamesDialog(const QStringList& names, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectNamesDialog),
    all_names(names)
{
    ui->setupUi(this);

    model = new QStandardItemModel(names.count(), 1, this); //удалится при удалении диалогового окна
    for (int i=0; i<names.count(); ++i)
    {
        QStandardItem *item = new QStandardItem(names[i]);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);
        model->setItem(i, 0, item);
    }
    ui->list_view_names->setModel(model);
}

SelectNamesDialog::~SelectNamesDialog()
{
    delete ui;
}

QStringList SelectNamesDialog::get_selected_names()
{
    QStringList selected_names;
    if (exec() == QMessageBox::Cancel)
    {
        return selected_names;
    }

    for (int i=0; i<model->rowCount(); ++i)
    {
        QModelIndex index = model->index(i, 0);
        if (index.data(Qt::CheckStateRole) == Qt::Checked)
        {
            selected_names.append(index.data().toString());
        }
    }
    return selected_names;
}
