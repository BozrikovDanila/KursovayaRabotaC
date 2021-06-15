#ifndef MY_KR_MAINWINDOW_H
#define MY_KR_MAINWINDOW_H

#include "ui_MainWindow.h"
#include "MyList.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow() override;

private slots:
    void on_saveForm(); // как нажали кнопку сохранить в форме
    void on_saveAsNewForm(); // создали новый объект в форме
    void on_remove(); // нажали кнопку удалить в форме
    void on_selectElementInTable(); // выбрали человека из таблицы
    void on_formTypeChange(); // когда выбираем другой тип товара в форме
    void on_saveFile(); // как загрузили новый файл из менюшки
    void on_openFile(); // загрузка файла из менюшки
    void on_newFile(); // новый файл из менюшки
    void on_saveAs(); // сохранить как из менюшки
    void on_searchAbort(); // кнопка сброса поиска
    void on_search(); // применить фильтры для поиска

private:
    QLabel* statusLabel; // это для статус бара
    Ui::MainWindow *ui;

    size_t editID; // это ID формы (которая на редактирование)
    MyList* mainList;

    std::string file;

    // это для поиска
    int maxAge = 0, minAge = 0;
    double maxWeight = 0, minWeight = 0;

    void reloadTableFromDB(const MyList* db); // заполнить таблицу из db
    int insertElementToTable(const Human& man, size_t id); // вставить в таблицу
    void clearForm(); // почистить форму
    Human* createFromForm(); // создать объект из формы

    void fillFormFromDB(size_t id);
};

#endif //MY_KR_MAINWINDOW_H
