#ifndef CSVTABLEMODEL_H
#define CSVTABLEMODEL_H

#include <QAbstractTableModel>

class CsvTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    //virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    void reset();

    bool read(const QString& file_name);

    explicit CsvTableModel(QObject *parent = nullptr) : QAbstractTableModel(parent){}

signals:

public slots:

private:
    QList<QList<QVariant>> table;
    QList<QVariant> horizontalHeader;

    static QList<QVariant> split_line(const QByteArray& line, bool* is_full);

};

#endif // CSVTABLEMODEL_H
