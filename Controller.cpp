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



Controller::Controller(QTabWidget *_tab_widget, QObject *parent) : QObject(parent), tab_widget(_tab_widget)
{
    data_was_load = false;
}

QVector<ElementType> Controller::get_column_types_of_model(const QAbstractTableModel *model)
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

    return types;
}

bool Controller::read_csv(const QStringList &file_names)
{
    table_names.clear();

    for (int i=0; i<file_names.count(); ++i)
    {
        table_names.push_back(QFileInfo(file_names[i]).completeBaseName());

        CsvTableModel* model = new CsvTableModel(this);

        if (!model->read(file_names[i]))
        {
            clear();

            return false;
        }

        models.append(model);
        column_types.push_back(get_column_types_of_model(model));

        //Создание вьюхи

        QTableView* view = new QTableView();
        view->setModel(model);
        view->setAlternatingRowColors(true);
        view->setWordWrap(true);
        view->resizeRowsToContents();

        tab_widget->addTab(view, table_names[i]);
    }

    data_was_load = true;
    return true;
}

bool Controller::read_db(const QString &db_name)
{
    QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));
    db.setDatabaseName(db_name);

    if (!db.open())
    {
        data_was_load = false;
        return false;
    }

    table_names = db.tables();

    for (int i=0; i<table_names.count(); ++i)
    {
        //Заполнение модели

        QSqlTableModel* model = new QSqlTableModel(nullptr, db);
        model->setTable(table_names[i]);

        if (!model->select())
        {
            data_was_load = false;
            return false;
        }

        models.append(model);
        column_types.push_back(get_column_types_of_model(model));

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
    for (int model_num=0; model_num<models.count(); ++model_num)
    {
        if (selected_names.indexOf(table_names[model_num]) >= 0)
        {
            QFile file("./"+table_names[model_num]+".csv");

            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return false;

            QTextStream fout(&file);

            int j;

            for (j=0; j<models[model_num]->columnCount()-1; ++j)
            {
                QString cell = models[model_num]->headerData(j, Qt::Orientation::Horizontal).toString();

                if (cell.indexOf(',') >= 0)
                {
                    cell = "\""+cell+"\"";
                }

                fout << cell << ",";
            }

            QString cell = models[model_num]->headerData(j, Qt::Orientation::Horizontal).toString();

            if (cell.indexOf(',') >= 0)
            {
                cell = "\""+cell+"\"";
            }

            fout << cell << "\n";

            for (int i=0; i<models[model_num]->rowCount(); ++i)
            {
                for (j=0; j<models[model_num]->columnCount()-1; ++j)
                {
                    QString cell = models[model_num]->index(i, j).data().toString();

                    if (cell.indexOf(',') >= 0)
                    {
                        cell = "\""+cell+"\"";
                    }

                    fout << cell << ",";
                }

                QString cell = models[model_num]->index(i, j).data().toString();

                if (cell.indexOf(',') >= 0)
                {
                    cell = "\""+cell+"\"";
                }

                fout << cell << "\n";
            }
            file.close();
        }
    }
    return true;
}

bool Controller::convert_to_db(const QStringList &selected_names) const
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    QString db_name="./"; //Добавить нормальный путь
    for (int i=0; i<selected_names.count()-1; ++i)
    {
        db_name+=table_names[i]+"_";
    }
    db_name+=table_names[table_names.count()-1]+".sqlite";

    QFile(db_name).remove();

    db.setDatabaseName(db_name);

    if (!db.open())
    {
        return false;
    }


    for (int model_num = 0; model_num < models.count(); ++model_num)
    {
        if (table_names[model_num] != QString("sqlite_sequence")
            && selected_names.indexOf(table_names[model_num]) >= 0)
        {
            QSqlQuery query;

            QString query_string = QString("create table %1(").arg(table_names[model_num]);
            for (int column = 0; column < models[model_num]->columnCount(); ++column)
                query_string += QString("\"%1\" %2, ")
                        .arg(models[model_num]->headerData(column, Qt::Horizontal, Qt::DisplayRole).toString())
                        .arg(get_type_string(column_types[model_num][column]));

            query_string.remove(query_string.length() - 2, 2);
            query_string += ")";
            if (!query.exec(query_string))
            {
                qDebug("Failed to create table.");
                qDebug(qPrintable(query.lastError().text()));
                return false;
            }

            for (int i = 0; i < models[model_num]->rowCount(); ++i)
            {
                query_string = QString("insert into %1 values(").arg(table_names[model_num]);
                for (int j=0; j<models[model_num]->columnCount(); ++j)
                {
                    if (column_types[model_num][j] == ElementType::TEXT)
                    {
                        query_string+=QString("'%1'").arg(models[model_num]->data(models[model_num]->index(i,j), Qt::DisplayRole).toString());
                    }
                    else
                    {
                        query_string+=models[model_num]->data(models[model_num]->index(i,j), Qt::DisplayRole).toString();
                    }
                    query_string+=", ";
                }
                query_string.remove(query_string.length() - 2, 2);
                query_string += ")";

                if (!query.exec(query_string))
                {
                    qDebug("Failed to append table.");
                    qDebug(qPrintable(query.lastError().text()));
                    return false;
                }
            }
        }
    }

    db.close();
    return true;
}

QStringList Controller::get_all_table_names() const
{
    return table_names;
}

void Controller::clear()
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

    for (int i=0; i<column_types.count(); ++i)
    {
        column_types[i].clear();
    }
    column_types.clear();


    data_was_load = false;
}






























