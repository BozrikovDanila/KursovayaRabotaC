#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMessageBox>
#include "MyList.h"
#include "manual.h"
#include "reference.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow() override;

private slots:
    void on_change(); // нажали кнопку изменить в форме
    void on_edit(); // создали новый объект в форме
    void on_delete(); // нажали кнопку удалить в форме
    void on_selectElementInTable(); // выбрали человека из таблицы
    void on_formTypeChange(); // когда выбираем другой тип товара в форме
    void on_saveFile(); // сохраняем файл (пункт меню)
    void on_openFile(); // загрузка файла из менюшки
    void on_newFile(); // новый файл из менюшки
    void on_saveAs(); // сохранить файл как... (пункт меню)
    void on_clearSearch(); // кнопка сброса поиска
    void on_search(); // применить признаки для поиска
    void clear_all();

    void on_category_health_currentIndexChanged(int index);
    void on_category_electronic_currentIndexChanged(int index);

    void on_manual_triggered();

    void on_actionExit_triggered();

    void on_about_triggered();

private:
    QLabel* status_label;
    QLabel* file_name_label;
    Ui::MainWindow *ui;

    size_t editID;
    MyList* mainList;
    MyList* savedList;

    std::string file;

    Manual manual;
    reference ref;


    void reloadTableFromList(const MyList* db);
    void pushElementToTable(const Product& el, size_t id);
    void clearForm();
    Product* createObject();

    void writeInFormFromDB(size_t id);

    bool maybeSave();

    void on_start();

    void set_title(std::string file);
};

#endif //MAINWINDOW_H
