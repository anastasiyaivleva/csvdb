#include <QString>
#include <QtTest>
#include <QApplication>

#include "../Controller.h"
#include "../CsvTableModel.h"

class Test : public QObject
{
    Q_OBJECT

public:
    Test();

private Q_SLOTS:
    //Тесты для класса Controller
    void test_get_type();
    void test_get_type_string();


    //Тест для класса CsvTableModel
    void test_read_csv();



    //Тест полной работы системы
    void test_csv_to_db_to_csv();
};




Test::Test()
{
}

void Test::test_get_type()
{
    QVERIFY(get_type(QString("1")) == ElementType::INTEGER);
    QVERIFY(get_type(QString("1.0")) == ElementType::REAL);
    QVERIFY(get_type(QString("1,0")) == ElementType::TEXT); //потому что разделитель разрядов - точка, а не запятая
}

void Test::test_get_type_string()
{
    QVERIFY(get_type_string(ElementType::INTEGER) == QString("INTEGER"));
    QVERIFY(get_type_string(ElementType::REAL) == QString("REAL"));
    QVERIFY(get_type_string(ElementType::TEXT) == QString("TEXT"));
}

void Test::test_read_csv()
{
    bool flag;

    CsvTableModel model;

    for (int i=1; i<=3; ++i)
    {
        flag = model.read(QString("./test_csv_input/test%1.csv").arg(i));

        model.reset();

        QVERIFY2(flag, QString("Error in opening test%1.csv file").arg(i).toStdString().c_str());

    }
}




void Test::test_csv_to_db_to_csv()
{
    bool flag;

    int argc=1;
    char* argv = "Test";

    QApplication a(argc, &argv);
    QTabWidget tab;
    Controller controller(&tab);

    flag = controller.read_csv(QStringList() << QString("./test_csv_input/test1.csv")
                                             << QString("./test_csv_input/test2.csv")
                                             << QString("./test_csv_input/test3.csv"));
    QVERIFY2(flag, "Error in read csv");

    QStringList names = controller.get_all_table_names();


    flag = names.count() == 3
           && names[0] == QString("test1")
           && names[1] == QString("test2")
           && names[2] == QString("test3");

    QVERIFY2(flag, "Error in table_names after read csv");


    flag = controller.convert_to_db(names);

    QVERIFY2(flag, "Error in converting to db");

    controller.reset();

    flag = controller.read_db("test1_test2_test3.sqlite");

    QVERIFY2(flag, "Error in read db");

    names = controller.get_all_table_names();


    flag = names.count() == 3
           && names[0] == QString("test1")
           && names[1] == QString("test2")
           && names[2] == QString("test3");

    QVERIFY2(flag, "Error in table_names after read db");


    QFile("test1_test2_test3.sqlite").remove();


    flag = controller.convert_to_csv(names);

    QVERIFY2(flag, "Error in converting to csv");


    for (int i=1; i<=3; ++i)
    {
        QFile csv_true_file(QString("./test_csv_input/test%1.csv").arg(i).toStdString().c_str()); //эталонный файл

        flag = csv_true_file.open(QIODevice::ReadOnly | QIODevice::Text);

        QVERIFY2(flag, "Error in opening csv file");


        QFile csv_test_file(QString("./test%1.csv").arg(i).toStdString().c_str());

        flag = csv_test_file.open(QIODevice::ReadOnly | QIODevice::Text);

        QVERIFY2(flag, QString("Error in opening test%1.csv file").arg(i).toStdString().c_str());


        while (!csv_true_file.atEnd() && !csv_test_file.atEnd())
        {
            flag = csv_true_file.readLine() == csv_test_file.readLine();

            QVERIFY2(flag, QString("Error in comparing test%1.csv files").arg(i).toStdString().c_str());
        }

        flag = csv_true_file.atEnd() && csv_test_file.atEnd();

        csv_test_file.close();
        csv_true_file.close();

        csv_test_file.remove();


        QVERIFY2(flag, QString("Two files test%1.csv have different size").arg(i).toStdString().c_str());
    }
}


QTEST_APPLESS_MAIN(Test)

#include "test.moc"
