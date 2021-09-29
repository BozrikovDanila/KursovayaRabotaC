#include <QFileDialog>
#include "MainWindow.h"

MainWindow::MainWindow() : QMainWindow(nullptr), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    editID = 0;

    ui->table->setColumnWidth(0, 40);
    ui->table->setColumnWidth(1, 140);
    ui->table->setColumnWidth(2, 60);
    ui->table->setColumnWidth(3, 80);
    ui->table->setColumnWidth(4, 80);
    ui->table->setColumnWidth(5, 150);
    ui->table->setColumnWidth(6, 150);

    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QTableWidget::connect(ui->table, SIGNAL(itemSelectionChanged()), this, SLOT(on_selectElementInTable()));

    QPushButton::connect(ui->edit, SIGNAL(clicked(bool)), this, SLOT(on_edit()));
    QPushButton::connect(ui->change, SIGNAL(clicked(bool)), this, SLOT(on_change()));
    QPushButton::connect(ui->del, SIGNAL(clicked(bool)), this, SLOT(on_delete()));
    QPushButton::connect(ui->clear_s, SIGNAL(clicked(bool)), this, SLOT(on_clearSearch()));
    QPushButton::connect(ui->search, SIGNAL(clicked(bool)), this, SLOT(on_search()));
    QPushButton::connect(ui->del_all, SIGNAL(clicked(bool)), this, SLOT(clear_all()));

    QAction::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(on_newFile()));
    QAction::connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(on_openFile()));
    QAction::connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(on_saveFile()));
    QAction::connect(ui->actionSaveAs, SIGNAL(triggered(bool)), this, SLOT(on_saveAs()));

    QComboBox::connect(ui->type, SIGNAL(currentIndexChanged(int)), this, SLOT(on_formTypeChange()));

    status_label = new QLabel("[ INFO ] Приложение запущено. Добро пожаловать!");
    ui->statusbar->addWidget(status_label);
    mainList = nullptr;
    clearForm();   
    on_start();
    resize(1200, 500);
    setWindowTitle("Информационно-поисковая система: Прокат вещей     File: Untitled.DB");
}

MainWindow::~MainWindow() {
    delete ui;
    delete mainList;
}

void MainWindow::on_change() {
    if (!mainList->exists(editID)) {
        status_label->setText(QString("[ FAILED ] Не найдена запись с ID=" + QString::number(editID)));
        return;
    }

    auto p = createObject();
    if (p != nullptr) {
        mainList->edit(editID, p);
        delete p;
        reloadTableFromList(mainList);
        status_label->setText(QString("[ OK ] Запись изменена"));
    }
}

void MainWindow::on_edit() {
    auto p = createObject();
    if (p != nullptr) {
        editID = mainList->add(p); // добавим в базу данных
        delete p; // удалим объект, так как он в куче
        reloadTableFromList(mainList); // обновим таблицу
        status_label->setText(QString("[ OK ] Сохранено успешно (новый ID=" + QString::number(editID) + ")"));
    }
}

void MainWindow::on_delete() {
    if (editID != MyList::INC_ID) {
        if (mainList->remove(editID)) {
            status_label->setText(QString("[ OK ] Удален объект с ID=" + QString::number(editID)));
            reloadTableFromList(mainList);
            clearForm();
        } else {
            status_label->setText(QString("[FAILED] Возникла ошибка при удалении объекта"));
        }
    } else {
        status_label->setText(QString("[FAILED] Объект не найден"));
    }
}

void MainWindow::on_selectElementInTable() {
    auto w = ui->table->item(ui->table->currentRow(), 0);
    if (w != nullptr) {
        size_t id = w->text().toInt();
        ui->table->selectRow(ui->table->currentRow());
        writeInFormFromDB(id);
    }
}

void MainWindow::on_formTypeChange() {
    auto index = ui->type->currentIndex();

    ui->category_clothes->setVisible(index == 1);
    ui->label_item_type_clothes->setVisible(index == 1);
    ui->size->setVisible(index == 1);
    ui->label_size->setVisible(index == 1);
    ui->sex->setVisible(index == 1);
    ui->label_sex->setVisible(index == 1);

    ui->category_electronic->setVisible(index == 2);
    ui->label_item_type_electronic->setVisible(index == 2);
    ui->power->setVisible(index == 2);
    ui->label_power->setVisible(index == 2);
    ui->label_power_2->setVisible(index == 2);

    ui->category_health->setVisible(index == 3);
    ui->label_item_type_health->setVisible(index == 3);
    ui->variety_health->setVisible(index == 3);
    ui->label_variety->setVisible(index == 3);
}


void MainWindow::on_clearSearch() {
    reloadTableFromList(mainList);

    ui->name_s->setText("");
    ui->checkBox_clothes->setChecked(true);
    ui->checkBox_product->setChecked(true);
    ui->checkBox_electronic->setChecked(true);
    ui->checkBox_health->setChecked(true);
    ui->code_s->clear();
    ui->cost_s->clear();
    ui->cost_s_2->clear();
}

void MainWindow::on_search() { // поиск   
    MyList temp;

    bool typeP = !(ui->checkBox_product->isChecked());
    bool typeC = !(ui->checkBox_clothes->isChecked());
    bool typeE = !(ui->checkBox_electronic->isChecked());
    bool typeH = !(ui->checkBox_health->isChecked());

    if(typeC && typeE && typeP && typeH){
        status_label->setText(QString("[ OK ] Найдено 0 записей. Видимо, Вы не выбрали тип"));
        reloadTableFromList(&temp);
        return;
    }

    if(ui->cost_s->value() > ui->cost_s_2->value()){
        status_label->setText(QString("[ FAILED ] Поле цена введено некорректно!"));
        return;
    }

    bool nameIsNotEmpty = !(ui->name_s->text().toStdString().empty());

    for(auto i = mainList->get(); i != nullptr; i = i->next()){
        auto el = i->get();
        size_t id = i->id;

        bool con;
        switch(el->get_type()){
        case NOTYPE:
            con = typeP;
            break;
        case CLOTHES:
            con = typeC;
            break;
        case ELECTRONIC:
            con = typeE;
            break;
        case HEALTH:
            con = typeH;
            break;
        }
        if(con) continue;

        if(ui->code_s->value() != 0){
            if(ui->code_s->value() != el->get_code()) continue;
        }

        if(ui->cost_s->value() != 0 && ui->cost_s_2->value() != 0){
            if(ui->cost_s->value() > el->get_cost()) continue;
            if(ui->cost_s_2->value() < el->get_cost()) continue;
        }

        if(nameIsNotEmpty){
            if (el->get_name().find(ui->name_s->text().toStdString()) == std::string::npos) continue;
        }

        temp.add(id, el);
    }

    status_label->setText(QString("[ OK ] Найдено " + QString::number(temp.size()) + " записей"));
    reloadTableFromList(&temp);
}

void MainWindow::clear_all()
{
    if(mainList->get() != nullptr){
        QMessageBox::StandardButton ret;
        ret = QMessageBox::question(this, tr("Вопрос"), tr("Вы точно хотите удалить все записи?"), QMessageBox::Ok | QMessageBox::Cancel);
        if(ret == QMessageBox::Ok){
            mainList->clear();
            clearForm();
            reloadTableFromList(mainList);
            status_label->setText("[ OK ] Вы отчистили базу данных");
        }
    }
    else{
        status_label->setText("[ INFO ] В базе данных нет записей для удаления");
    }
}

void MainWindow::reloadTableFromList(const MyList *db) {
    ui->table->clearContents();
    ui->table->setRowCount(0);

    if(db->get() != nullptr){
        for(auto i = db->get(); i != nullptr; i = i->next())
            pushElementToTable(*i->get(), i->id);
    }
}

void MainWindow::pushElementToTable(const Product &el, size_t id) { // добавление в таблицу
    int row = ui->table->rowCount();
    ui->table->insertRow(row);

    ui->table->setItem(row, 0, (new QTableWidgetItem(QString::number(id))));

    switch (el.get_type()) {
    case NOTYPE:
        ui->table->setItem(row, 1, (new QTableWidgetItem("Товар")));
        break;
    case CLOTHES:
        ui->table->setItem(row, 1, (new QTableWidgetItem("Одежда")));
        break;
    case ELECTRONIC:
        ui->table->setItem(row, 1, (new QTableWidgetItem("Электроника")));
        break;
    case HEALTH:
        ui->table->setItem(row, 1, (new QTableWidgetItem("Уход за здоровьем")));
        break;
    }

    ui->table->setItem(row, 2, (new QTableWidgetItem(QString::number(el.get_code()))));
    ui->table->setItem(row, 3, (new QTableWidgetItem(el.get_name().c_str())));
    ui->table->setItem(row, 4, (new QTableWidgetItem(QString::number(el.get_cost()))));
    ui->table->setItem(row, 5, (new QTableWidgetItem(el.get_term_from().c_str())));
    ui->table->setItem(row, 6, (new QTableWidgetItem(el.get_term_to().c_str())));

    for (int i = 0; i < 6; i++) {
        QTableWidgetItem *item = ui->table->item(row, i);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    }
}

void MainWindow::clearForm() {
    editID = MyList::INC_ID;

    ui->ID->setText("ID " + QString::number(editID));
    ui->type->setCurrentIndex(0);

    ui->name->clear();
    ui->code->clear();
    ui->cost->clear();
    ui->dateFrom->setDate(ui->dateFrom->minimumDate());
    ui->dateTo->setDate(ui->dateTo->minimumDate());
    ui->category_clothes->setCurrentIndex(0);
    ui->category_electronic->setCurrentIndex(0);
    ui->category_health->setCurrentIndex(0);
    ui->variety_health->setCurrentIndex(0);
    ui->size->setCurrentIndex(0);
    ui->sex->setCurrentIndex(0);
    ui->power->clear();

    on_formTypeChange();
}

Product* MainWindow::createObject() {
    if(ui->code->text().toStdString().empty()){
        status_label->setText("[ FAILED ] Заполните поле номера заказа!");
        return nullptr;
    }

    QStringList list1 = ui->dateFrom->text().split(".");
    QStringList list2 = ui->dateTo->text().split(".");

    if(list1[2].toInt() > list2[2].toInt()){
        status_label->setText("[ FAILED ] Видимо, дата некорректна!");
        return nullptr;
    }
    else if(list1[2].toInt() == list2[2].toInt() && list1[1].toInt() > list2[1].toInt()){
        status_label->setText("[ FAILED ] Видимо, дата некорректна!");
        return nullptr;
    }
    else if(list1[2].toInt() == list2[2].toInt() && list1[1].toInt() == list2[1].toInt() && list1[0].toInt() >= list2[0].toInt()){
        status_label->setText("[ FAILED ] Видимо, дата некорректна!");
        return nullptr;
    }

    int code = ui->code->text().toInt();
    std::string name = ui->name->text().toStdString();
    double cost = ui->cost->text().toDouble();
    std::string dateFrom = ui->dateFrom->text().toStdString();
    std::string dateTo = ui->dateTo->text().toStdString();

    auto category_clothes = (CategoriesClothes)ui->category_clothes->currentIndex();
    auto size = (Size)ui->size->currentIndex();
    auto sex = (Sex)ui->sex->currentIndex();

    auto category_electronic = (CategoriesElectronic)ui->category_electronic->currentIndex();

    auto category_health = (CategoriesHealth)ui->category_health->currentIndex();

    Variety variety;

    if(ui->category_health->currentIndex() == 0){
        switch(ui->variety_health->currentIndex()){
        case 0:{
            variety = STEAM;
            break;
            }
        case 1:{
            variety = ULTRASONIC;
            break;
            }
        case 2:{
            variety = COMPRESSION;
            break;
            }
        }
    }
    else if(ui->category_health->currentIndex() == 1){
        switch(ui->variety_health->currentIndex()){
        case 0:{
            variety = SOFT;
            break;
            }
        case 1:{
            variety = MEDIUM;
            break;
            }
        case 2:{
            variety = HARD;
            break;
            }
        }
    }
    else if(ui->category_health->currentIndex() == 2){
        switch(ui->variety_health->currentIndex()){
        case 0:{
            variety = LEGS;
            break;
            }
        case 1:{
            variety = NECK;
            break;
            }
        case 2:{
            variety = BACK;
            break;
            }
        }
    }

    switch(ui->type->currentIndex()){
    case NOTYPE:
        return new Product(name, code, cost, dateFrom, dateTo);
    case CLOTHES:
        return new ProductClothes(name, code, cost, dateFrom, dateTo, category_clothes, sex, size);
    case ELECTRONIC:
        return new ProductElectronic(name, code, cost, dateFrom, dateTo, category_electronic, ui->power->text().toInt());
    case HEALTH:
        return new ProductHealth(name, code, cost, dateFrom, dateTo, category_health, variety);
    default:
        return nullptr;
    }

}

void MainWindow::writeInFormFromDB(size_t id) {
    clearForm();

    editID = id;
    auto element = mainList->get(id);
    if (element == nullptr) return;
    auto product = element->get();


    ui->ID->setText("ID " + QString::number(editID));
    ui->name->setText(product->get_name().c_str());
    ui->code->setValue(product->get_code());
    ui->cost->setValue(product->get_cost());
    QDate date= date.fromString(product->get_term_from().c_str(), "dd.MM.yyyy");
    ui->dateFrom->setDate(date);
    date= date.fromString(product->get_term_to().c_str(), "dd.MM.yyyy");
    ui->dateTo->setDate(date);
    auto type = product->get_type();
    ui->type->setCurrentIndex(type);

    on_formTypeChange();

    switch (type) {
        case NOTYPE:
            break;
        case CLOTHES:
            {
                auto* clothes = dynamic_cast<const ProductClothes*>(product);
                if (clothes != nullptr) {
                    ui->category_clothes->setCurrentIndex((int)clothes->get_category());
                    ui->size->setCurrentIndex((int)clothes->get_size());
                    ui->sex->setCurrentIndex((int)clothes->get_sex());
                } else {
                    status_label->setText("[ DOUBT ] Возникла ошибка, ProductClothes объект некорректен");
                }
            }
            break;
        case ELECTRONIC:
            {
                auto* electronic = dynamic_cast<const ProductElectronic*>(product);
                if (electronic != nullptr) {
                    ui->category_electronic->setCurrentIndex((int)electronic->get_category());
                    ui->power->setValue(electronic->get_power());
                } else {
                    status_label->setText("[ DOUBT ] Возникла ошибка, ProductElectronic объект некорректен");
                }
            }
            break;
        case HEALTH:
            {
                auto* health = dynamic_cast<const ProductHealth*>(product);
                if (health != nullptr) {
                    ui->category_health->setCurrentIndex((int)health->get_category());
                    switch((int)health->get_variety()){
                    case 3:
                        ui->variety_health->setCurrentIndex(0);
                        break;
                    case 4:
                        ui->variety_health->setCurrentIndex(1);
                        break;
                    case 5:
                        ui->variety_health->setCurrentIndex(2);
                        break;
                    case 6:
                        ui->variety_health->setCurrentIndex(0);
                        break;
                    case 7:
                        ui->variety_health->setCurrentIndex(1);
                        break;
                    case 8:
                        ui->variety_health->setCurrentIndex(2);
                        break;
                    default:
                        ui->variety_health->setCurrentIndex((int)health->get_variety());
                    }

                } else {
                    status_label->setText("[ DOUBT ] Возникла ошибка, ProductHealth объект некорректен");
                }
            }
            break;
        default:
            status_label->setText("[ DOUBT ] Объект имеет неизвестный тип");
            clearForm();
            break;
    }
}


void MainWindow::on_category_health_currentIndexChanged(int index)
{
    switch (index) {
    case 0:{
        ui->variety_health->setItemText(0, "Паровой");
        ui->variety_health->setItemText(1, "Ультразвуковой");
        ui->variety_health->setItemText(2, "Компрессионный");
        break;
        }
    case 1:{
        ui->variety_health->setItemText(0, "Мягкая");
        ui->variety_health->setItemText(1, "Средняя");
        ui->variety_health->setItemText(2, "Твёрдая");
        break;
        }
    case 2:{
        ui->variety_health->setItemText(0, "Ноги");
        ui->variety_health->setItemText(1, "Шея");
        ui->variety_health->setItemText(2, "Спина");
        break;
        }
    }
}

void MainWindow::on_category_electronic_currentIndexChanged(int index)
{
    switch (index) {
    case 0:{
        ui->label_power_2->setText("об/мин");
        break;
        }
    case 1:{
        ui->label_power_2->setText("Вт");
        break;
        }
    case 2:{
        ui->label_power_2->setText("Вт");
        break;
        }
    }
}

/****************Работа с файлами***********/

void MainWindow::on_saveFile() {
    if(QString(file.c_str()).isEmpty()){
        on_saveAs();
        return;
    }
    if (mainList != nullptr) {
        if (mainList->reserve(file)) {
            status_label->setText("[ OK ] Данные сохранены в \"" + QString(file.c_str()) + "\"");
            savedList = mainList;
            set_title(file);
        } else {
            status_label->setText("[ FAILED ] Возникла проблема при сохранении данных");
        }
    }
}

void MainWindow::on_openFile() {
    if(maybeSave()){
        QString f = QFileDialog::getOpenFileName(this, tr("Открыть..."), QString(), tr("DB-file (*.DB);;All Files (*)"));
        if (f.isEmpty()) return;
        if (!mainList) mainList = new MyList();
        if (mainList->load(f.toStdString())) {
            clearForm();
            on_clearSearch();
            status_label->setText(QString("[ OK ] Данные загружены из \"" + f + "\""));
            file = f.toStdString();
            savedList = mainList;
            set_title(file);
        } else {
            status_label->setText(QString("[ FAILED ] Ошибка загрузки данных из \"" + f + "\""));
        }
    }
}

void MainWindow::on_newFile() {
        if(maybeSave()){
            if (!mainList) mainList = new MyList();
            file = "";
            mainList->clear();
            clearForm();
            on_clearSearch();
            savedList = mainList;
            set_title(file);
        }
}

void MainWindow::on_saveAs() {
    QString f = QFileDialog::getSaveFileName(this, tr("Сохранить как..."), QString(), tr("DB-file (*.DB);;All Files (*)"));
    file = f.toStdString();
    if (!mainList) return;
    if (mainList->reserve(f.toStdString())) {
        status_label->setText(QString("[ OK ] Данные сохранены в \"" + f + "\""));
        savedList = mainList;
        set_title(file);
    } else {
        status_label->setText(QString("[ FAILED ] Возникла ошибка при сохранении данных в \"" + f + "\""));
    }
}

bool MainWindow::maybeSave()
{
    if(mainList->get() == nullptr) return true;
    if(!mainList) return true;
    if(mainList == savedList) return true;

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Application"), tr("Ваша база даных была изменена.\n" "Вы хотите сохранить изменения?"),
    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
    {
        if(!QString(file.c_str()).isEmpty()){
            on_saveFile();
            return true;
        }

        on_saveAs();
        return true;
    }
    else if (ret == QMessageBox::Cancel) return false;
    return true;
}

void MainWindow::on_start()
{
    if (!mainList) mainList = new MyList();
    mainList->clear();
    clearForm();
    on_clearSearch();
}

void MainWindow::set_title(std::string file)
{
    if(QString(file.c_str()).isEmpty()) {
        setWindowTitle("Информационно-поисковая система: Прокат вещей     File: \"Untitled.DB\"" );
    }
    else {
        QStringList name = QString(file.c_str()).split('/');
        setWindowTitle("Информационно-поисковая система: Прокат вещей     File name: \"" + name.last() + "\"");
    }
}

void MainWindow::on_manual_triggered()
{
    manual.show();
}

void MainWindow::on_actionExit_triggered()
{
    if(maybeSave()){
        mainList->clear();
        clearForm();
        reloadTableFromList(mainList);
        close();
    }
}


void MainWindow::on_about_triggered()
{
    ref.show();
}
