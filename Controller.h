#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QtSql/QSqlTableModel>
#include <QTableView>
#include <QString>
#include <QStringList>


/**
 * @brief Введён для возможности сравнения типов.
 * Благодаря этому перечислению для определения типа столбца достаточно найти максимальный тип элемента.
 */
enum ElementType
{
    INTEGER = 0,
    REAL = 1,
    TEXT = 2
};


/**
 * @brief По элементу возвращает его тип.
 * @param[in] val элемент в виде строки
 * @return Тип элемента
 */
ElementType get_type(const QString& val);


/**
 * @brief Возвращает строковое представление типа столбца для создания таблицы БД.
 * @param type тип столбца
 * @return Строковое представление типа
 */
QString get_type_string(ElementType type);


/**
 * @brief Обеспечивает загрузку данных из файлов в формате CSV и DB и выгрузку данных в них.
 */
class Controller : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор класса Controller
     * @param _tab_widget указатель на виджет вывода таблиц
     * @param parent родительский объект
     */
    explicit Controller(QTabWidget* _tab_widget, QObject *parent = nullptr);


    /**
     * @brief Деструктор класса Controller
     *
     */
    ~Controller();

    /**
     * @brief Задаёт папку для автоматического сохранения CSV и DB файлов.
     *
     * @param work_dir строковое представление папки
     */
    void set_working_directory(const QString& work_dir);


    /**
     * @brief Читает таблицы из нескольких файлов CSV.
     * В случае неудачи всё очищается и появляется сообщение о неудачном чтении файлов
     *
     * @param file_names имена файлов для чтения
     * @return Успешность выполнения функции
     */
    bool read_csv(const QStringList& file_names);


    /**
     * @brief Читает все таблицы из базы данных
     *
     * @param db_name имя файла базы данных
     * @return Успешность выполнения функции
     */
    bool read_db(const QString& db_name);


    /**
     * @brief Преобразует хранящиеся таблицы в несколько файлов CSV
     * Преобразуются только выбранные пользователем таблицы
     *
     * @param selected_names выбранные таблицы
     * @return Успешность выполнения функции
     */
    bool convert_to_csv(const QStringList& selected_names) const;


    /**
     * @brief Преобразует хранящиеся таблицы в таблицы базы данных
     * Преобразуются только выбранные пользователем таблицы
     *
     * @param selected_names выбранные таблицы
     * @return Успешность выполнения функции
     */
    bool convert_to_db(const QStringList& selected_names) const;


    /**
     * @brief Возвращает имена всех хранящихся таблиц
     *
     * @return Список имён таблиц
     */
    QStringList get_all_table_names() const;


    /**
     * @brief Очищает контроллер
     *
     */
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

    /**
     * @brief Возвращает типы столбцов выбранной модели
     *
     * @param model выбранная модель
     * @return Вектор строковых представлений типов столбцов
     */
    QVector<QString> get_column_types_of_model(const QAbstractTableModel* model) const;
};

#endif // CONTROLLER_H





















