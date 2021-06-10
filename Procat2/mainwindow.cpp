#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "object.h"
#include "Clothes.h"


bool active = false;
bool rt;
int last_index_case = 0, last_index = 0;
int index_0 = 0, index_1 = 0, index_2 = 0;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->lineEdit_search->setPlaceholderText("Поиск");

    ui->lineEdit_sex->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
    ui->comboBox_case_1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
    ui->comboBox_case_2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
    ui->lineEdit_size->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
    ui->checkBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);

    ui->comboBox_case_1->hide();
    ui->comboBox_case_2->hide();
    ui->lineEdit_size->hide();
    ui->lineEdit_sex->hide();
    ui->checkBox->hide();

    ui->lineEdit_name->setValidator(new QRegExpValidator(QRegExp("[А-Яа-яA-Za-z]+")));
    ui->lineEdit_code->setValidator(new QRegExpValidator(QRegExp("[0-9]+")));
    ui->lineEdit_cost->setValidator(new QRegExpValidator(QRegExp("[0-9]+")));
    ui->lineEdit_size->setValidator(new QRegExpValidator(QRegExp("[0-9]+")));
    ui->lineEdit_sex->setValidator(new QRegExpValidator(QRegExp("[0-9]+")));

    ui->radioButton_m->setChecked(true);

    connect(ui->pushButton_edit,SIGNAL(clicked()), this,SLOT(edit()));
    connect(ui->pushButton_del,SIGNAL(clicked()), this,SLOT(del()));
    connect(ui->tableWidget,SIGNAL(cellClicked()), this,SLOT(to_del()));

    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("Тип")<<tr("Код")<<tr("Название")<<tr("Цена")<<tr("Срок аренды")<<tr("Вид товара")<<tr("Размер")<<tr("Пол"));

    //ui->gridLayout_main->setColumnStretch(0, 2);
    //ui->gridLayout_main->setColumnStretch(1, 1);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::edit()
{
    QPushButton *button = (QPushButton*)sender();

    QString str1, str2;
    int number;
    bool value;

    switch(ui->comboBox_type->currentIndex()){
    case 0:{

        Clothes obj0;

        number = ui->comboBox_type->currentIndex();
        obj0.set_type(number);

        str1 = ui->lineEdit_name->text();
        obj0.set_name(str1);

        str1 = ui->lineEdit_code->text();
        number = str1.toInt();
        obj0.set_code(number);

        str1 = ui->lineEdit_cost->text();
        number = str1.toInt();
        obj0.set_cost(number);

        str1 = ui->dateEdit->text();
        str2 = ui->dateEdit_2->text();
        obj0.set_arend_time(str1, str2);

        value = ui->radioButton_f->isChecked();
        obj0.set_sex(value); // true - Ж, false - М

        number = ui->comboBox_size->currentIndex();
        obj0.set_size(number);

        number = ui->comboBox_item->currentIndex();
        obj0.set_variety(number);

        Data.push_back(&obj0);
        ui->tableWidget->setRowCount(Data.size());

        switch(Data.last()->get_type()){
        case 0:
            str1 = "Одежда";
            break;
        case 1:
            str1 = "Техника для дома";
            break;
        case 2:
            str1 = "Уход за здоровьем";
            break;
        }

        ui->tableWidget->setItem(Data.size() - 1, 0, new QTableWidgetItem(str1));

        str1 = QString::number(Data.last()->get_code());
        ui->tableWidget->setItem(Data.size() - 1, 1, new QTableWidgetItem(str1));

        str1 = Data.last()->get_name();
        ui->tableWidget->setItem(Data.size() - 1, 2, new QTableWidgetItem(str1));

        str1 = QString::number(Data.last()->get_cost());
        ui->tableWidget->setItem(Data.size() - 1, 3, new QTableWidgetItem(str1));

        str1 ="C: " + Data.last()->get_arend_time_1() + "" + "До: " + Data.last()->get_arend_time_2();
        ui->tableWidget->setItem(Data.size() - 1, 4, new QTableWidgetItem(str1));

        switch(Data.last()->get_variety()){
        case 0:
            str1 = "Нижнее белье";
            break;
        case 1:
            str1 = "Джинсы";
            break;
        case 2:
            str1 = "Головной убор";
        case 3:
            str1 = "Куртка, пальто";
        case 4:
            str1 = "Рубашка";
            break;
        }
        ui->tableWidget->setItem(Data.size() - 1, 5, new QTableWidgetItem(str1));

        switch(Data.last()->get_size()){
        case 0:
            str1 = "XS";
            break;
        case 1:
            str1 = "S";
            break;
        case 2:
            str1 = "M";
        case 3:
            str1 = "L";
        case 4:
            str1 = "XL";
            break;
        }
        ui->tableWidget->setItem(Data.size() - 1, 6, new QTableWidgetItem(str1));

        if(Data.last()->get_true()) str1 = "Ж";
        else str1 = "М";
        ui->tableWidget->setItem(Data.size() - 1, 7, new QTableWidgetItem(str1));
        break;
    }
    case 1:{
        Home_electr obj1;

        number = ui->comboBox_type->currentIndex();
        obj1.set_type(number);

        str1 = ui->lineEdit_name->text();
        obj1.set_name(str1);

        str1 = ui->lineEdit_code->text();
        number = str1.toInt();
        obj1.set_code(number);

        str1 = ui->lineEdit_cost->text();
        number = str1.toInt();
        obj1.set_cost(number);

        str1 = ui->dateEdit->text();
        str2 = ui->dateEdit_2->text();
        obj1.set_arend_time(str1, str2);

        number = ui->comboBox_case_1->currentIndex();
        obj1.set_variety(number);

        str1 = ui->lineEdit_size->text();
        number = str1.toInt();
        obj1.set_size_container(number);

        str1 = ui->lineEdit_sex->text();
        number = str1.toInt();
        obj1.set_power(number);

        Data.push_back(&obj1);
        //index_1++;
        ui->tableWidget->setRowCount(Data.size());

        number = Data.last()->get_number();

        switch(Data.last()->get_type()){
        case 0:
            str1 = "Одежда";
            break;
        case 1:
            str1 = "Техника для дома";
            break;
        case 2:
            str1 = "Уход за здоровьем";
            break;
        }
        ui->tableWidget->setItem(Data.size() - 1, 0, new QTableWidgetItem(str1));

        str1 = QString::number(Data.last()->get_code());
        ui->tableWidget->setItem(Data.size() - 1, 1, new QTableWidgetItem(str1));

        str1 = Data.last()->get_name();
        ui->tableWidget->setItem(Data.size() - 1, 2, new QTableWidgetItem(str1));

        str1 = QString::number(Data.last()->get_cost());
        ui->tableWidget->setItem(Data.size() - 1, 3, new QTableWidgetItem(str1));

        str1 ="C: " + Data.last()->get_arend_time_1() + "\n" + "До: " + Data.last()->get_arend_time_2();
        ui->tableWidget->setItem(Data.size() - 1, 4, new QTableWidgetItem(str1));

        switch(Data.last()->get_variety()){
        case 0:
            str1 = "Стиральная машина";
            break;
        case 1:
            str1 = "Утюг";
            break;
        case 2:
            str1 = "Пылесос";
            break;
        }
        ui->tableWidget->setItem(Data.size() - 1, 5, new QTableWidgetItem(str1));

        str1 = QString::number(Data.last()->get_size());
        ui->tableWidget->setItem(Data.size() - 1, 6, new QTableWidgetItem(str1));

        str1 = QString::number(Data.last()->get_number());
        ui->tableWidget->setItem(Data.size() - 1, 7, new QTableWidgetItem(str1));
        break;
        }
    case 2:{
        Health_care obj2;

        number = ui->comboBox_type->currentIndex();
        obj2.set_type(number);

        str1 = ui->lineEdit_name->text();
        obj2.set_name(str1);

        str1 = ui->lineEdit_code->text();
        number = str1.toInt();
        obj2.set_code(number);

        str1 = ui->lineEdit_cost->text();
        number = str1.toInt();
        obj2.set_cost(number);

        str1 = ui->dateEdit->text();
        str2 = ui->dateEdit_2->text();
        obj2.set_arend_time(str1, str2);

        number = ui->comboBox_case_2->currentIndex();
        obj2.set_variety(number);

        number = ui->comboBox_size->currentIndex();
        obj2.set_variety_of(number);

        if(ui->comboBox_case_2->currentIndex() == 0)
        {
            str1 = ui->lineEdit_sex->text();
            number = str1.toInt();
            obj2.set_size_container(number);
        }
        else
        {
            value = ui->checkBox->isChecked();
            obj2.set_intencity_change(value);
        }

        Data.push_back(&obj2);
        //index_2++;
        ui->tableWidget->setRowCount(Data.size());


        switch(Data.last()->get_type()){
        case 0:
            str1 = "Одежда";
            break;
        case 1:
            str1 = "Техника для дома";
            break;
        case 2:
            str1 = "Уход за здоровьем";
            break;
        }
        ui->tableWidget->setItem(Data.size() - 1, 0, new QTableWidgetItem(str1));

        str1 = QString::number(Data.last()->get_code());
        ui->tableWidget->setItem(Data.size() - 1, 1, new QTableWidgetItem(str1));

        str1 = Data.last()->get_name();
        ui->tableWidget->setItem(Data.size() - 1, 2, new QTableWidgetItem(str1));

        str1 = QString::number(Data.last()->get_cost());
        ui->tableWidget->setItem(Data.size() - 1, 3, new QTableWidgetItem(str1));

        str1 ="C: " + Data.last()->get_arend_time_1() + "\n" + "До: " + Data.last()->get_arend_time_2();
        ui->tableWidget->setItem(Data.size() - 1, 4, new QTableWidgetItem(str1));

        switch(Data.last()->get_variety()){
        case 0:
            str1 = "Ингалятор";
            break;
        case 1:
            str1 = "Зубная щётка";
            break;
        case 2:
            str1 = "Массажёр";
            break;
        }
        ui->tableWidget->setItem(Data.size() - 1, 5, new QTableWidgetItem(str1));

        switch (Data.last()->get_number()) {
        case 1:
            str1 = "Паровой";
            break;
        case 11:
            str1 = "Ультразвуковой";
            break;
        case 111:
            str1 = "Компрессионный";
            break;
        case 2:
            str1 = "Мягкая";
            break;
        case 22:
            str1 = "Средняя";
            break;
        case 222:
            str1 = "Твёрдая";
            break;
        case 3:
            str1 = "Ноги";
            break;
        case 33:
            str1 = "Тело";
            break;
        case 333:
            str1 = "Спина";
            break;
        }
        ui->tableWidget->setItem(Data.size() - 1, 6, new QTableWidgetItem(str1));

        if(Data.last()->get_variety() == 0)
            str1 = QString::number(Data.last()->get_size());
        else
        {
            if(Data.last()->get_true()) str1 = "Есть";
            else str1 = "Нет";
        }
        ui->tableWidget->setItem(Data.size() - 1, 7, new QTableWidgetItem(str1));

        break;
        }

    }
    ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::del()
{
        int row = -1;
        row = ui->tableWidget->currentRow();
        if(rt) {
            ui->tableWidget->removeRow(row);
            Data.remove(row);
        }
        rt = false;
}

void MainWindow::on_comboBox_type_currentIndexChanged(int index)
{

    //1:вид товара комбобокс, мощность (оборотов, всасыванияБ потребления) лайнэдит, максимальная загрузка(пыли, воды(мл)) кг лайн эдит(стиральная машинка),
    if(!active)
    {
        switch (index)
        {
            case 1:
            {
                ui->comboBox_case_1->show();

                ui->comboBox_item->parentWidget()->layout()->replaceWidget(ui->comboBox_item, ui->comboBox_case_1);
                ui->comboBox_item->hide();

                ui->label_sex->setText("Макс. кол. оборотов");
                ui->label_size->setText("Максимальная загрузка");

                ui->lineEdit_sex->show();
                ui->radioButton_m->parentWidget()->layout()->replaceWidget(ui->radioButton_m,ui->lineEdit_sex);

                ui->radioButton_f->hide();
                ui->radioButton_m->hide();

                ui->lineEdit_size->show();

                ui->comboBox_size->parentWidget()->layout()->replaceWidget(ui->comboBox_size,ui->lineEdit_size);
                ui->comboBox_size->hide();


                //ui->lineEdit_sex->setFixedWidth(455);

                active = true;

                ui->gridLayout_main->setColumnStretch(0, 2);
                ui->gridLayout_main->setColumnStretch(1, 1);

            break;
            }
            case 2:
            {
                //Ингалятор(Паровой, ултьтрозвуковой небулайзер, компрессорный небулайзер / объем резервуара для лекарств ), зубная щётка(жесткость / съемная насадка), массажёр(для спины, тела, шеи, ног/ регулияровка интесивности)

                ui->comboBox_case_2->show();

                ui->comboBox_item->parentWidget()->layout()->replaceWidget(ui->comboBox_item, ui->comboBox_case_2);
                ui->comboBox_item->hide();

                ui->label_size->setText("Вид ингалятора");
                ui->label_sex->setText("Объем резервуара для лекарств");

                ui->comboBox_size->setItemText(0, "Паровой");
                ui->comboBox_size->setItemText(1, "Ультразвуковой");
                ui->comboBox_size->setItemText(2, "Компрессорный");
                ui->comboBox_size->removeItem(3);
                ui->comboBox_size->removeItem(3);

                ui->lineEdit_sex->show();
                ui->radioButton_m->parentWidget()->layout()->replaceWidget(ui->radioButton_m, ui->lineEdit_sex);


                ui->radioButton_f->hide();
                ui->radioButton_m->hide();

                //ui->lineEdit_sex->setFixedWidth(392);
                active = true;

                ui->gridLayout_main->setColumnStretch(0, 2);
                ui->gridLayout_main->setColumnStretch(1, 1);
            break;
            }
        }
        last_index_case = index;
    }
    else
    {
        switch (index)
        {
        case 0:
            if(last_index_case == 1)
            {
                ui->label_sex->setText("Пол");
                ui->label_size->setText("Размер");

                ui->radioButton_f->show();
                ui->radioButton_m->show();
                ui->comboBox_item->show();
                ui->comboBox_size->show();

                ui->comboBox_size->setItemText(0, "XS");
                ui->comboBox_size->setItemText(1, "S");
                ui->comboBox_size->setItemText(2, "M");

                ui->lineEdit_sex->parentWidget()->layout()->replaceWidget(ui->lineEdit_sex, ui->radioButton_m);
                ui->comboBox_case_1->parentWidget()->layout()->replaceWidget(ui->comboBox_case_1, ui->comboBox_item);
                ui->lineEdit_size->parentWidget()->layout()->replaceWidget(ui->lineEdit_size, ui->comboBox_size);

                ui->lineEdit_sex->hide();
                ui->lineEdit_size->hide();
                ui->comboBox_case_1->hide();
            }
            else if(last_index_case == 2)
            {
                ui->label_sex->setText("Пол");
                ui->label_size->setText("Размер");

                ui->radioButton_f->show();
                ui->radioButton_m->show();
                ui->comboBox_item->show();

                ui->comboBox_size->setItemText(0, "XS");
                ui->comboBox_size->setItemText(1, "S");
                ui->comboBox_size->setItemText(2, "M");
                ui->comboBox_size->addItem("L");
                ui->comboBox_size->addItem("XL");

                ui->comboBox_case_2->parentWidget()->layout()->replaceWidget(ui->comboBox_case_2, ui->comboBox_item);

                switch (last_index) {
                case 0:
                    ui->lineEdit_sex->parentWidget()->layout()->replaceWidget(ui->lineEdit_sex, ui->radioButton_m);
                    ui->lineEdit_sex->hide();
                    break;
                case 1:
                    ui->checkBox->parentWidget()->layout()->replaceWidget(ui->checkBox, ui->radioButton_m);
                    ui->checkBox->hide();
                    break;
                case 2:
                    ui->checkBox->parentWidget()->layout()->replaceWidget(ui->checkBox, ui->radioButton_m);
                    ui->checkBox->hide();
                    break;
                }

                ui->comboBox_case_2->hide();
            }
            break;
        case 1:
            if(last_index_case == 0)
            {
                ui->comboBox_case_1->show();
                ui->lineEdit_sex->show();
                ui->lineEdit_size->show();

                ui->radioButton_m->parentWidget()->layout()->replaceWidget(ui->radioButton_m, ui->lineEdit_sex);
                ui->comboBox_item->parentWidget()->layout()->replaceWidget(ui->comboBox_item, ui->comboBox_case_1);
                ui->comboBox_size->parentWidget()->layout()->replaceWidget(ui->comboBox_size, ui->lineEdit_size);

                ui->radioButton_f->hide();
                ui->radioButton_m->hide();
                ui->comboBox_item->hide();
                ui->comboBox_size->hide();

                switch (ui->comboBox_item->currentIndex()) {
                case 0:
                    ui->label_sex->setText("Макс. кол. оборотов");
                    ui->label_size->setText("Максимальная загрузка");
                    //ui->lineEdit_sex->setFixedWidth(455);
                    break;
                case 1:
                    ui->label_sex->setText("Макс. потребление");
                    ui->label_size->setText("Макс. объём воды");
                    //ui->lineEdit_sex->setFixedWidth(488);
                    break;
                case 2:
                    ui->label_sex->setText("Мощность всасывания");
                    ui->label_size->setText("Макс. загрузка пылью");
                    //ui->lineEdit_sex->setFixedWidth(463);
                    break;
                }
            }
            else if(last_index_case == 2)
            {
                ui->comboBox_case_1->show();
                ui->lineEdit_size->show();

                ui->comboBox_case_2->parentWidget()->layout()->replaceWidget(ui->comboBox_case_2, ui->comboBox_case_1);
                ui->comboBox_size->parentWidget()->layout()->replaceWidget(ui->comboBox_size, ui->lineEdit_size);

                switch (last_index) {
                case 1:
                    ui->lineEdit_sex->show();
                    ui->checkBox->parentWidget()->layout()->replaceWidget(ui->checkBox, ui->lineEdit_sex);
                    ui->checkBox->hide();
                    break;
                case 2:
                    ui->lineEdit_sex->show();
                    ui->checkBox->parentWidget()->layout()->replaceWidget(ui->checkBox, ui->lineEdit_sex);
                    ui->comboBox_size->hide();
                    break;
                }

                ui->comboBox_case_2->hide();
                ui->comboBox_size->hide();

                switch (ui->comboBox_item->currentIndex()) {
                case 0:
                    ui->label_sex->setText("Макс. кол. оборотов");
                    ui->label_size->setText("Максимальная загрузка");
                    //ui->lineEdit_sex->setFixedWidth(455);
                    break;
                case 1:
                    ui->label_sex->setText("Макс. потребление");
                    ui->label_size->setText("Макс. объём воды");
                    //ui->lineEdit_sex->setFixedWidth(488);
                    break;
                case 2:
                    ui->label_sex->setText("Мощность всасывания");
                    ui->label_size->setText("Макс. загрузка пылью");
                    //ui->lineEdit_sex->setFixedWidth(463);
                    break;
                }
            }

            ui->gridLayout_main->setColumnStretch(0, 2);
            ui->gridLayout_main->setColumnStretch(1, 1);
            break;
        case 2:
            ui->comboBox_case_2->show();

            if(last_index_case == 0)
            {
                ui->comboBox_item->parentWidget()->layout()->replaceWidget(ui->comboBox_item, ui->comboBox_case_2);

                switch (ui->comboBox_case_2->currentIndex()) {
                case 0:
                    ui->lineEdit_sex->show();
                    ui->radioButton_m->parentWidget()->layout()->replaceWidget(ui->radioButton_m, ui->lineEdit_sex);
                    break;
                case 1:
                    ui->checkBox->show();
                    ui->radioButton_m->parentWidget()->layout()->replaceWidget(ui->radioButton_m, ui->checkBox);
                    break;
                case 2:
                    ui->checkBox->show();
                    ui->radioButton_m->parentWidget()->layout()->replaceWidget(ui->radioButton_m, ui->checkBox);
                    break;
                }

                ui->radioButton_f->hide();
                ui->radioButton_m->hide();
                ui->comboBox_item->hide();
            }
            else if(last_index_case == 1)
            {
                ui->comboBox_size->show();
                ui->comboBox_case_1->parentWidget()->layout()->replaceWidget(ui->comboBox_case_1, ui->comboBox_case_2);
                ui->lineEdit_size->parentWidget()->layout()->replaceWidget(ui->lineEdit_size, ui->comboBox_size);

                switch (ui->comboBox_case_2->currentIndex()) {
                case 1:
                    ui->checkBox->show();
                    ui->lineEdit_sex->parentWidget()->layout()->replaceWidget(ui->lineEdit_sex, ui->checkBox);
                    break;
                case 2:
                    ui->checkBox->show();
                    ui->lineEdit_sex->parentWidget()->layout()->replaceWidget(ui->lineEdit_sex, ui->checkBox);
                    break;
                }

                ui->lineEdit_size->hide();
            }

            ui->comboBox_size->removeItem(3);
            ui->comboBox_size->removeItem(3);
            ui->comboBox_case_1->hide();

            switch (ui->comboBox_case_2->currentIndex()) {
            case 0:
                ui->label_size->setText("Вид ингалятора");
                ui->label_sex->setText("Объем резервуара для лекарств");

                ui->comboBox_size->setItemText(0, "Паровой");
                ui->comboBox_size->setItemText(1, "Ультразвуковой");
                ui->comboBox_size->setItemText(2, "Компрессорный");

                //ui->lineEdit_sex->setFixedWidth(392);
                break;
            case 1:
                ui->label_size->setText("Жесткость");
                ui->label_sex->setText("Съёмность насадки");

                ui->comboBox_size->setItemText(0, "Мягкая");
                ui->comboBox_size->setItemText(1, "Средняя");
                ui->comboBox_size->setItemText(2, "Жесткая");
                break;
            case 2:
                ui->label_size->setText("Область применения на теле");
                ui->label_sex->setText("Регулировка интенсивности");

                ui->comboBox_size->setItemText(0, "Для ног");
                ui->comboBox_size->setItemText(1,"Для тела");
                ui->comboBox_size->setItemText(2,"Для спины");
                break;
            }

            ui->gridLayout_main->setColumnStretch(0, 2);
            ui->gridLayout_main->setColumnStretch(1, 1);
            break;
        }
        last_index_case = index;
    }
}


void MainWindow::on_comboBox_case_1_activated(int index)
{
    switch (index) {
    case 0:
        ui->label_sex->setText("Макс. кол. оборотов");
        ui->label_size->setText("Макс. загрузка");
        //ui->lineEdit_sex->setFixedWidth(455);
        break;
    case 1:
        ui->label_sex->setText("Макс. потребление");
        ui->label_size->setText("Макс. объём воды");
        //ui->lineEdit_sex->setFixedWidth(488);
        break;
    case 2:
        ui->label_sex->setText("Мощность всасывания");
        ui->label_size->setText("Макс. загрузка пылью");
        //ui->lineEdit_sex->setFixedWidth(463);
        break;
    }
    ui->gridLayout_main->setColumnStretch(0, 2);
    ui->gridLayout_main->setColumnStretch(1, 1);
}

void MainWindow::to_del()
{
    rt = true;
}

void MainWindow::on_comboBox_case_2_activated(int index)
{
    switch (index) {
    case 0:
        ui->label_size->setText("Вид ингалятора");
        ui->label_sex->setText("Объем резервуара для лекарств");

        ui->comboBox_size->setItemText(0, "Паровой");
        ui->comboBox_size->setItemText(1, "Ультразвуковой");
        ui->comboBox_size->setItemText(2, "Компрессорный");

        if(last_index == 1 || last_index == 2)
        {
            ui->lineEdit_sex->show();
            ui->checkBox->parentWidget()->layout()->replaceWidget(ui->checkBox, ui->lineEdit_sex);
            ui->checkBox->hide();
            //ui->lineEdit_sex->setFixedWidth(392);
        }
        break;
    case 1:
        ui->label_size->setText("Жесткость");
        ui->label_sex->setText("Съёмность насадки");

        ui->comboBox_size->setItemText(0, "Мягкая");
        ui->comboBox_size->setItemText(1, "Средняя");
        ui->comboBox_size->setItemText(2, "Жесткая");

        ui->checkBox->show();
        ui->lineEdit_sex->parentWidget()->layout()->replaceWidget(ui->lineEdit_sex, ui->checkBox);
        ui->lineEdit_sex->hide();
        break;
    case 2:
        ui->label_size->setText("Область применения на теле");
        ui->label_sex->setText("Регулировка интенсивности");

        ui->comboBox_size->setItemText(0, "Для ног");
        ui->comboBox_size->setItemText(1,"Для тела");
        ui->comboBox_size->setItemText(2,"Для спины");

        ui->checkBox->show();
        ui->lineEdit_sex->parentWidget()->layout()->replaceWidget(ui->lineEdit_sex, ui->checkBox);
        ui->lineEdit_sex->hide();

        break;
    }
    ui->gridLayout_main->setColumnStretch(0, 2);
    ui->gridLayout_main->setColumnStretch(1, 1);
    last_index = index;
}



