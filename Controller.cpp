#include "Controller.h"
#include "CsvTableModel.h"
#include <QFile.h>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextStream>
#include <QFileInfo>
#include <QHeaderView>
#include <QDir>

#include <QDebug>

ElementType get_type(QString val)
{
    bool ok;

    val.toInt(&ok, 10);
    if (ok)
    {
        return ElementType::INTEGER;
    }

    val.toDouble(&ok);
    if (ok)
    {
        return ElementType::REAL;
    }

    return ElementType::TEXT;
}


QString get_type_string(ElementType type)
{
    switch (type)
    {
    case ElementType::INTEGER:
        return QString("INTEGER");
    case ElementType::REAL:
        return QString("REAL");
    case ElementType::TEXT:
        return QString("TEXT");
    }
    return QString();
}



Controller::Controller(QTabWidget *_tab_widget, QObject *parent) :
    QObject(parent),
    tab_widget(_tab_widget)
{
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    working_directory = QDir::current().canonicalPath();
    data_was_load = false;
}

Controller::~Controller()
{
    delete db;
}

void Controller::set_working_directory(const QString &work_dir)
{
    working_directory = work_dir;
}

QVector<QString> Controller::get_column_types_of_model(const QAbstractTableModel *model) const
{
    QVector<ElementType> types;
    types.fill(ElementType::INTEGER, model->columnCount());

    for (int i=0; i<model->rowCount(); ++i)
    {
        for (int j=0; j<model->columnCount(); ++j)
        {
            ElementType type = get_type(model->index(i, j).data().toString());
            if (type > types[j])
            {
                types[j] = type;
            }
        }
    }

    QVector<QString> result;

    for (int j=0; j<model->columnCount(); ++j)
    {
        result.push_back(get_type_string(types[j]));
    }

    return result;
}

bool Controller::read_csv(const QStringList &file_names)
{
    for (int i=0; i<file_names.count(); ++i)
    {
        table_names.push_back(QFileInfo(file_names[i]).completeBaseName());

        CsvTableModel* model = new CsvTableModel(this);

        if (!model->read(file_names[i]))
        {
            reset();

            return false;
        }

        models.append(model);

        //Создание вьюхи

        QTableView* view = new QTableView();
        view->setModel(model);
        view->setAlternatingRowColors(true);
        view->resizeRowsToContents();

        tab_widget->addTab(view, table_names[i]);
    }

    data_was_load = true;
    return true;
}

bool Controller::read_db(const QString &db_name)
{
    db->setDatabaseName(db_name);

    if (!db->open())
    {
        data_was_load = false;
        return false;
    }

    table_names = db->tables();

    if (table_names.count() == 0)
    {
        db->close();
        data_was_load = false;
        return false;
    }

    for (int i=0; i<table_names.count(); ++i)
    {
        //Заполнение модели

        QSqlTableModel* model = new QSqlTableModel(nullptr, *db);
        model->setTable(table_names[i]);

        if (!model->select())
        {
            db->close();
            data_was_load = false;
            return false;
        }

        models.append(model);

        //Создание вьюхи

        QTableView* view = new QTableView();
        view->setModel(model);
        view->setAlternatingRowColors(true);
        view->resizeRowsToContents();

        tab_widget->addTab(view, table_names[i]);
    }

    db->close();
    data_was_load = true;
    return true;
}

bool Controller::convert_to_csv(const QStringList &selected_names) const
{
    for (int model_num=0; model_num<models.count(); ++model_num)
    {
        if (selected_names.indexOf(table_names[model_num]) >= 0)
        {
            QFile file(working_directory + "/" + table_names[model_num] + ".csv");

            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return false;

            QTextStream fout(&file);

            int i, j, index;

            for (j=0; j<models[model_num]->columnCount()-1; ++j)
            {
                QString cell = models[model_num]->headerData(j, Qt::Orientation::Horizontal).toString();

                //все кавычки ячейки заменяются на двойные
                index = cell.indexOf('\"');

                while (index >= 0)
                {
                    cell.insert(index, '\"');
                    index = cell.indexOf('\"', index + 2);
                }

                if (cell.indexOf(',') >= 0 || cell.indexOf('\n') >= 0)
                {
                    cell = "\"" + cell + "\"";
                }

                fout << cell << ",";
            }

            QString cell = models[model_num]->headerData(j, Qt::Orientation::Horizontal).toString();

            //все кавычки ячейки заменяются на двойные
            index = cell.indexOf('\"');

            while (index >= 0)
            {
                cell.insert(index, '\"');
                index = cell.indexOf('\"', index + 2);
            }

            if (cell.indexOf(',') >= 0 || cell.indexOf('\n') >= 0)
            {
                cell = "\"" + cell + "\"";
            }

            fout << cell << "\n";

            //Формирование заголовка закончено

            for (i=0; i<models[model_num]->rowCount()-1; ++i)
            {
                for (j=0; j<models[model_num]->columnCount()-1; ++j)
                {
                    QString cell = models[model_num]->index(i, j).data().toString();

                    //все кавычки ячейки заменяются на двойные
                    index = cell.indexOf('\"');

                    while (index >= 0)
                    {
                        cell.insert(index, '\"');
                        index = cell.indexOf('\"', index + 2);
                    }

                    if (cell.indexOf(',') >= 0 || cell.indexOf('\n') >= 0)
                    {
                        cell = "\"" + cell + "\"";
                    }

                    fout << cell << ",";
                }

                QString cell = models[model_num]->index(i, j).data().toString();

                //все кавычки ячейки заменяются на двойные
                index = cell.indexOf('\"');

                while (index >= 0)
                {
                    cell.insert(index, '\"');
                    index = cell.indexOf('\"', index + 2);
                }

                if (cell.indexOf(',') >= 0 || cell.indexOf('\n') >= 0)
                {
                    cell = "\"" + cell + "\"";
                }

                fout << cell << "\n";
            }


            //дальше i==models[model_num]->rowCount()-1

            for (j=0; j<models[model_num]->columnCount()-1; ++j)
            {
                QString cell = models[model_num]->index(i, j).data().toString();

                //все кавычки ячейки заменяются на двойные
                index = cell.indexOf('\"');

                while (index >= 0)
                {
                    cell.insert(index, '\"');
                    index = cell.indexOf('\"', index + 2);
                }

                if (cell.indexOf(',') >= 0 || cell.indexOf('\n') >= 0)
                {
                    cell = "\"" + cell + "\"";
                }

                fout << cell << ",";
            }

            cell = models[model_num]->index(i, j).data().toString();

            //все кавычки ячейки заменяются на двойные
            index = cell.indexOf('\"');

            while (index >= 0)
            {
                cell.insert(index, '\"');
                index = cell.indexOf('\"', index + 2);
            }

            if (cell.indexOf(',') >= 0 || cell.indexOf('\n') >= 0)
            {
                cell = "\"" + cell + "\"";
            }

            file.close();
        }
    }

    return true;
}

bool Controller::convert_to_db(const QStringList &selected_names) const
{
    QString db_name = working_directory + "/";
    for (int i=0; i<selected_names.count()-1; ++i)
    {
        db_name += table_names[i] + "_";
    }
    db_name += table_names[table_names.count()-1] + ".sqlite";

    QFile(db_name).remove();

    db->setDatabaseName(db_name);

    if (!db->open())
    {
        return false;
    }

    QSqlQuery query;

    for (int model_num = 0; model_num < models.count(); ++model_num)
    {
        if (/*table_names[model_num] != QString("sqlite_sequence")
            && */selected_names.indexOf(table_names[model_num]) >= 0)
        {
            QVector<QString> types = get_column_types_of_model(models[model_num]);

            QString query_string = QString("create table %1(").arg(table_names[model_num]);

            for (int column = 0; column < models[model_num]->columnCount(); ++column)
                query_string += QString("\"%1\" %2, ")
                        .arg(models[model_num]->headerData(column, Qt::Horizontal, Qt::DisplayRole).toString())
                        .arg(types[column]);

            query_string.remove(query_string.length() - 2, 2);
            query_string += ")";
            if (!query.exec(query_string))
            {
                qDebug("Failed to create table.");///
                qDebug(qPrintable(query.lastError().text()));///
                return false;
            }

            //Уже создана база данных

            for (int i = 0; i < models[model_num]->rowCount(); ++i)
            {
                query_string = QString("insert into %1 values(").arg(table_names[model_num]);
                for (int j=0; j<models[model_num]->columnCount(); ++j)
                {
                    if (types[j] == QString("TEXT"))
                    {
                        query_string += QString("'%1'").arg(models[model_num]->data(models[model_num]->index(i, j), Qt::DisplayRole).toString());
                    }
                    else
                    {
                        query_string += models[model_num]->data(models[model_num]->index(i, j), Qt::DisplayRole).toString();
                    }
                    query_string += ", ";
                }
                query_string.remove(query_string.length() - 2, 2);
                query_string += ")";

                if (!query.exec(query_string))
                {
                    qDebug("Failed to append table.");////////
                    qDebug(qPrintable(query.lastError().text()));/////////
                    return false;
                }
            }
        }
    }

    db->close();
    return true;
}

QStringList Controller::get_all_table_names() const
{
    return table_names;
}

void Controller::reset()
{
    for (int i=0; i<models.count(); ++i)
    {
        delete models[i];
    }
    models.clear();

    for (int i=0; i<views.count(); ++i)
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

    table_names.clear();

    /*for (int i=0; i<column_types.count(); ++i)
    {
        column_types[i].clear();
    }
    column_types.clear();*/


    data_was_load = false;
}
