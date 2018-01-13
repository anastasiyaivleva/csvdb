//#ifndef CSVTABLEMODEL_H
//#define CSVTABLEMODEL_H

//#include <QAbstractTableModel>

//enum ElementType
//{
//    INTEGER = 0,
//    REAL = 1,
//    TEXT = 2
//};

//ElementType get_type(QString val);
//QString getTypeString(ElementType type);



//class CsvTableModel : public QAbstractTableModel
//{
//    Q_OBJECT
//public:
//    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
//    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
//    virtual QVariant data(const QModelIndex &index, int role) const override;
//    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

//    void reset();

//    explicit CsvTableModel(QObject *parent = nullptr) : QAbstractTableModel(parent){}
//    bool read(const QString& file_name);
//    const QVector<ElementType>& get_column_types() const;
//signals:

//public slots:

//private:
//    QList<QStringList> table;
//    QStringList column_names;
//    QVector<ElementType> column_types;

//    void splitRow(const QByteArray &row, QStringList& result, bool &isFullRow);
//};

//#endif // CSVTABLEMODEL_H
