#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QtSql/QSqlTableModel>
#include <QTableView>
#include <QString>
#include <QStringList>

enum ElementType
{
    INTEGER = 0,
    REAL = 1,
    TEXT = 2
};

ElementType get_type(QString val);
QString get_type_string(ElementType type);


class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QTabWidget* _tab_widget, QObject *parent = nullptr);
    ~Controller();

    void set_working_directory(const QString& work_dir);

    bool read_csv(const QStringList& file_names);
    bool read_db(const QString& db_name);

    bool convert_to_csv(const QStringList& selected_names) const;
    bool convert_to_db(const QStringList& selected_names) const;

    QStringList get_all_table_names() const;

    void reset();

    bool data_was_load;

signals:

public slots:

private:
    QTabWidget* tab_widget;

    QString working_directory;

    QStringList table_names;

    QList<QAbstractTableModel*> models;
    QList<QTableView*> views;

    QSqlDatabase *db;

    QVector<QString> get_column_types_of_model(const QAbstractTableModel* model) const;
};

#endif // CONTROLLER_H





















