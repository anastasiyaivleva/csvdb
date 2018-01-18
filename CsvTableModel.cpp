#include "CsvTableModel.h"
#include "CsvTableModel.h"
#include <QFile>
#include <QStringList>
#include <QString>





int CsvTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return table.count();
}

int CsvTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return horizontalHeader.count();
}

QVariant CsvTableModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.column() < 0 || index.column() >= columnCount()
        || index.row() < 0 || index.row() >= rowCount())
    {
        return QVariant();
    }

    return table.at(index.row()).at(index.column());
}

QVariant CsvTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QAbstractTableModel::headerData(section, orientation, role);
    }

    if (orientation == Qt::Vertical)
    {
        return QAbstractTableModel::headerData(section, orientation, role);
    }

    if (section < 0 || section >= columnCount())
    {
        return QAbstractTableModel::headerData(section, orientation, role);
    }

    return horizontalHeader.at(section);
}



void CsvTableModel::reset()
{
    emit beginResetModel();

    table.clear();
    horizontalHeader.clear();

    emit endResetModel();
}

bool CsvTableModel::read(const QString &file_name)
{
    emit beginResetModel();

    if (rowCount() != 0)
    {
        reset();
    }

    if (!horizontalHeader.empty())
    {
        horizontalHeader.clear();
    }

    QFile csv_file(file_name);

    if (!csv_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit endResetModel();
        return false;
    }

    bool is_full=false;

    QList<QVariant> splitted_row;

    while (!csv_file.atEnd() && !is_full)
    {
        splitted_row.append(split_line(csv_file.readLine(), &is_full));
    }

    if (!is_full)
    {
        emit endResetModel();
        return false;
    }

    horizontalHeader = splitted_row;

    while (!csv_file.atEnd())
    {
        is_full=false;

        splitted_row.clear();

        while (!csv_file.atEnd() && !is_full)
        {
            splitted_row.append(split_line(csv_file.readLine(), &is_full));
        }

        if (!is_full)
        {
            for (int i=0; i<table.count(); ++i)
            {
                table[i].clear();
            }
            table.clear();

            emit endResetModel();
            return false;
        }

        table.append(splitted_row);
    }

    emit endResetModel();
    return true;
}

QList<QVariant> CsvTableModel::split_line(const QByteArray &line, bool *is_full)
{
   /*QString row(line);

   int index = line.indexOf("\"\"");

   while(index >= 0)
   {
       line[index] = 0x01; //заглушка для двойной кавычки
       line.remove(index+1,1);

       index =
   }*/


   *is_full=true;
   QList<QVariant> result;

   QList<QByteArray> line_split = line.split(',');
   if (line_split.last().endsWith('\n'))
   {
       line_split.last().chop(1);
   }

   for (int i=0; i<line_split.count(); ++i)
       result.push_back(line_split[i]);

   return result;
}
