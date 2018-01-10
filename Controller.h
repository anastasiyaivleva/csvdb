#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QtSql/QSqlTableModel>
#include <QTableView>
#include <QtSql/QSqlDatabase>
#include <QString>
#include <QStringList>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QTabWidget* _tab_widget, QObject *parent = nullptr);
    bool read_csv(const QStringList& file_names);
    bool read_db(const QString& db_name);

    bool convert_to_csv(const QStringList& selected_names) const;
    bool convert_to_db(const QStringList& selected_names) const;

signals:

public slots:

private:
    QList<QSqlTableModel*> models;
    QList<QTableView*> views;
    QTabWidget* tab_widget;
    QSqlDatabase* db;
    bool data_was_load;

    QStringList parse_csv_string(const QString& row);
};

#endif // CONTROLLER_H
