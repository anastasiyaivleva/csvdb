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
    if (role != Qt::DisplayRole || !index.isValid())
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

    QList<QVariant> splitted_line;

    while (!csv_file.atEnd() && !is_full)
    {
        splitted_line.append(split_line(csv_file.readLine(), &is_full));
    }

    if (!is_full)
    {
        emit endResetModel();
        return false;
    }

    horizontalHeader = splitted_line;

    while (!csv_file.atEnd())
    {
        is_full=false;

        splitted_line.clear();
        QByteArray last_cell;

        while (!csv_file.atEnd() && !is_full)
        {
            if (!splitted_line.isEmpty())
            {
                last_cell = splitted_line.last().toByteArray();
                splitted_line.removeLast();
            }
            splitted_line.append(split_line(last_cell+csv_file.readLine(), &is_full));
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

        table.append(splitted_line);
    }

    emit endResetModel();
    return true;
}

QList<QVariant> CsvTableModel::split_line(const QByteArray &line, bool *is_full)
{
    QByteArray line_copy(line);
    *is_full = true;

    int index = line_copy.indexOf("\"\"");

    //Все парные двойные кавычки заменяются на спецсимвол 0x01
    while(index >= 0)
    {
       line_copy[index] = 0x01; //заглушка для двойной кавычки
       line_copy.remove(index+1, 1);

       index = line_copy.indexOf("\"\"");
    }
    if (line_copy.endsWith('\n'))
    {
        line_copy.chop(1);
    }

    QList<QByteArray> all_parts = line_copy.split(',');
    QList<QByteArray> correct_parts;

    int i=0;

    while(i<all_parts.count())
    {
        if (all_parts[i].startsWith('\"'))
        {
            if (all_parts[i].endsWith('\"'))
            {
                all_parts[i].remove(0, 1);
                all_parts[i].chop(1);

                correct_parts.append(all_parts[i]);
                ++i;
            }
            else
            {
                int j=i+1;

                while(j<all_parts.count() && !all_parts[j].endsWith('\"'))
                {
                    all_parts[i] += ',' + all_parts[j];
                    ++j;
                }


                //Если дошли до конца списка всех подстрок, но так и не нашли ту,
                //которая заканчивается на кавычку
                if (j==all_parts.count())
                {
                    correct_parts.append(all_parts[i]);
                    *is_full = false;

                    i = j; // тогда i==j==all_parts.count() - условие выхода из внешнего while
                }
                //Если нашли строку, которая заканчивается на кавычку
                else
                {
                    all_parts[i] += ',' + all_parts[j];
                    all_parts[i].remove(0, 1);
                    all_parts[i].chop(1); //удаляется кавычка

                    correct_parts.append(all_parts[i]);

                    i=j+1;
                }
            }
        }
        else
        {
            correct_parts.append(all_parts[i]);
            ++i;
        }
    }


    //Спецсимвол 0x01 во всех подстроках заменяется на кавычку
    for (int i=0; i<correct_parts.count(); ++i)
    {
        index = correct_parts[i].indexOf(0x01);
        while (index>=0)
        {
            correct_parts[i][index] = '\"';
            index = correct_parts[i].indexOf(0x01);
        }
    }


    if (!*is_full && line.endsWith('\n'))
    {
        correct_parts.last() += '\n';
    }

    QList<QVariant> result;

    for (int i=0; i<correct_parts.count(); ++i)
    {
        result.append(correct_parts[i]);
    }

    return result;



   /* *is_full=true;
   QList<QVariant> result;

   QList<QByteArray> line_split = line.split(',');
   if (line_split.last().endsWith('\n'))
   {
       line_split.last().chop(1);
   }

   for (int i=0; i<line_split.count(); ++i)
       result.push_back(line_split[i]);

   return result;*/
}
