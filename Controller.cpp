#include "Controller.h"
#include "CsvTableModel.h"
#include <QFile.h>
#include <QSqlDatabase>

Controller::Controller(QTabWidget *_tab_widget, QObject *parent) : QObject(parent), tab_widget(_tab_widget)
{
    data_was_load = false;
}

QStringList Controller::parse_csv_string(const QString &row)
{
    return row.split(",");
}

bool Controller::read_csv(const QStringList &file_names)
{
    return true;
    /*for (int i=0; i<file_names.size(); ++i)
    {
        QFile file(file_names[i]);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return false;

        QSqlTableModel* model = new CsvTableModel(this);

        if (!file.atEnd())
        {
            columnNames = splitRow(file.readLine(), sep);
            columnCount = columnNames.count();
            columnTypes.fill(ElementType::INT, columnCount);
        }
    }*/
}

bool Controller::read_db(const QString &db_name)
{
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db->setDatabaseName(db_name);

    if (!db->open())
    {
        data_was_load = false;
        return false;
    }

    table_names = db->tables();

    for (int i=0; i<table_names.size(); ++i)
    {
        //Заполнение модели

        QSqlTableModel* model = new QSqlTableModel(nullptr, *db);
        model->setTable(table_names[i]);

        if (!model->select())
        {
            data_was_load = false;
            return false;
        }

        models.append(model);

        //Создание вьюхи

        QTableView* view = new QTableView();
        view->setModel(model);
        view->setAlternatingRowColors(true);
        tab_widget->addTab(view, table_names[i]);
    }
    data_was_load = true;
    return true;
}

bool Controller::convert_to_csv(const QStringList &selected_names) const
{
    return true;
}

bool Controller::convert_to_db(const QStringList &selected_names) const
{
    return true;
}

QStringList Controller::get_all_table_names() const
{
    return table_names;
}

void Controller::clear()
{
    for (int i=0; i<models.size(); ++i)
    {
        delete models[i];
    }
    models.clear();

    for (int i=0; i<views.size(); ++i)
    {
        delete views[i];
    }
    views.clear();

    int tab_count = tab_widget->count();
    for (int i=0; i<tab_count; ++i)
    {
        tab_widget->removeTab(0);
    }
    tab_widget->clear();

    db->close();
    delete db;

    table_names.clear();

    data_was_load = false;
}






























