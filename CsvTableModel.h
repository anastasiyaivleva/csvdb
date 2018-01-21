#ifndef CSVTABLEMODEL_H
#define CSVTABLEMODEL_H

#include <QAbstractTableModel>

/**
 * @brief Класс модели, хранящий в себе данные из CSV-файла
 *
 */
class CsvTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:


    /**
     * @brief Конструктор
     *
     * @param parent родитель
     */
    explicit CsvTableModel(QObject *parent = nullptr) : QAbstractTableModel(parent){}


    /**
     * @brief Возвращает количество строк в таблице
     *
     * @param parent родитель
     * @return Количество строк
     */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;


    /**
     * @brief Возвращает количество столбцов таблицы
     *
     * @param parent родитель
     * @return Количество строк
     */
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;


    /**
     * @brief Возвращает данные, хранящиеся по указанному индексу
     *
     * @param index индекс
     * @param role роль
     * @return Данные, хранящиеся по индексу
     */
    virtual QVariant data(const QModelIndex &index, int role) const override;


    /**
     * @brief Возвращает имя выбранного столбца таблицы
     *
     * @param section номер столбца
     * @param orientation ориентация
     * @param role роль
     * @return Имя столбца
     */
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;


    /**
     * @brief Очищает модель
     *
     */
    void reset();


    /**
     * @brief Чтение таблицы из файла
     *
     * @param file_name имя входного файла
     * @return Успешность выполнения операции
     */
    bool read(const QString& file_name);

signals:

public slots:

private:
    QList<QList<QVariant>> table;
    QList<QVariant> horizontalHeader;


    /**
     * @brief Разделяет строку из файла на подстроки в соответствии со стандартом CSV
     * Функция static, т.к. она не привязана к конкретному объекту класса
     *
     * @param line строка файла
     * @param is_full возвращает true, если все ячейки строки прочитаны полностью и false, если данные последней ячейки неполны
     * @return Список строк - значений ячеек таблицы
     */
    static QList<QVariant> split_line(const QByteArray& line, bool* is_full);

};

#endif // CSVTABLEMODEL_H
