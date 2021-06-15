#include <QFileDialog>
#include "MainWindow.h"

MainWindow::MainWindow() : QMainWindow(nullptr), ui(new Ui::MainWindow) {
    editID = 0;
    ui->setupUi(this);

    ui->table->setColumnWidth(0, 40);
    ui->table->setColumnWidth(1, 140);
    ui->table->setColumnWidth(2, 60);
    ui->table->setColumnWidth(3, 80);
    ui->table->setColumnWidth(4, 80);
    ui->table->setColumnWidth(5, 100);

    QTableWidget::connect(ui->table, SIGNAL(itemSelectionChanged()), this, SLOT(on_selectElementInTable()));

    QPushButton::connect(ui->saveAsButton     , SIGNAL(clicked(bool)), this, SLOT(on_saveAsNewForm()));
    QPushButton::connect(ui->saveButton       , SIGNAL(clicked(bool)), this, SLOT(on_saveForm()));
    QPushButton::connect(ui->removeButton     , SIGNAL(clicked(bool)), this, SLOT(on_remove()));
    QPushButton::connect(ui->searchResetButton, SIGNAL(clicked(bool)), this, SLOT(on_searchAbort()));
    QPushButton::connect(ui->searchButton     , SIGNAL(clicked(bool)), this, SLOT(on_search()));

    QAction::connect(ui->actionNew   , SIGNAL(triggered(bool)), this, SLOT(on_newFile()));
    QAction::connect(ui->actionOpen  , SIGNAL(triggered(bool)), this, SLOT(on_openFile()));
    QAction::connect(ui->actionSave  , SIGNAL(triggered(bool)), this, SLOT(on_saveFile()));
    QAction::connect(ui->actionSaveAs, SIGNAL(triggered(bool)), this, SLOT(on_saveAs()));

    QMainWindow::connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(close()));

    QComboBox::connect(ui->type, SIGNAL(currentIndexChanged(int)), this, SLOT(on_formTypeChange()));
    maxAge = 0;
    minAge = 0;
    maxWeight = 0;
    minWeight = 0;
    statusLabel = new QLabel("[ INFO ] Приложение запущено");
    ui->statusbar->addWidget(statusLabel);
    mainList = nullptr;
    clearForm();
    on_newFile();
    resize(800, 400);
    setWindowTitle("Информационно-поисковая система: фитнес клуб");
}

MainWindow::~MainWindow() {
    delete ui; // удалим UI, за ним удалятся все виджеты
    delete mainList; // контроллер тоже подлежит удалению
}

void MainWindow::on_saveForm() {
    // посмотрим, если такой человек в базе
    if (!mainList->contains(editID)) {
        statusLabel->setText(QString("[FAILED] Не найдена запись с ID=" + QString::number(editID)));
        return;
    }
    // создаем объект из формы
    auto p = createFromForm();
    if (p != nullptr) {
        mainList->edit(editID, p); // редактируем
        delete p; // удаляем объект из кучи
        reloadTableFromDB(mainList); // обновляем таблицу
        statusLabel->setText(QString("[  OK  ] Сохранено успешно"));
    }
}

void MainWindow::on_saveAsNewForm() {
    // создадим человека из формы
    auto p = createFromForm();
    if (p != nullptr) {
        editID = mainList->add(p); // добавим в базу данных
        delete p; // удалим объект, так как он в куче
        reloadTableFromDB(mainList); // обновим таблицу
        statusLabel->setText(QString("[  OK  ] Сохранено успешно (новый ID=" + QString::number(editID) + ")"));
    }
}

void MainWindow::on_remove() {
    if (editID != MyList::INVALID_ID) { // если редактируем кого-то
        if (mainList->remove(editID)) { // пытаемся удалить
            statusLabel->setText(QString("[  OK  ] Удален объект с ID=" + QString::number(editID)));
            ui->table->removeRow(ui->table->currentRow()); // если редактируем, значит что-то все равно выбрано
            ui->table->clearSelection(); // удалим выделение, так как оно любит само появлятся
            reloadTableFromDB(mainList);
            clearForm(); // за одно почистим форму
        } else { // не получилось, не фортануло
            statusLabel->setText(QString("[FAILED] Возникла ошибка при удалении объекта"));
        }
    } else { // тут уже юзер виноват, нехер жать что попало
        statusLabel->setText(QString("[FAILED] Объект не найден"));
    }
}

void MainWindow::on_selectElementInTable() {
    auto w = ui->table->item(ui->table->currentRow(), 0); // получаем первую ячейку (где лежит ID) из выделенной строки
    if (w != nullptr) { // таблица имеет право вернуть nullptr
        size_t id = w->text().toInt();
        ui->table->selectRow(ui->table->currentRow()); // выделим всю строку
        fillFormFromDB(id); // заполним форму
    }
}

void MainWindow::on_formTypeChange() {
    // покажем поля по индексу
    auto i = ui->type->currentIndex();
    ui->teacherIDLabel     ->setVisible(i == 1);
    ui->teacherID          ->setVisible(i == 1);
    ui->teacherPhoneLabel  ->setVisible(i == 1);
    ui->teacherPhone       ->setVisible(i == 1);
    ui->salaryLabel        ->setVisible(i == 1 || i == 2);
    ui->salary             ->setVisible(i == 1 || i == 2);
    ui->managerEduLabel    ->setVisible(i == 2);
    ui->managerEdu         ->setVisible(i == 2);
    ui->clientIDLabel      ->setVisible(i == 3);
    ui->clientID           ->setVisible(i == 3);
    ui->clientEndDateLabel ->setVisible(i == 3);
    ui->clientEndDate      ->setVisible(i == 3);
}

void MainWindow::on_saveFile() {
    if (mainList != nullptr) { // надо обязательно проверять...
        if (mainList->store(file)) { // пробуем сохранить последний
            statusLabel->setText(QString("[  OK  ] Данные сохранены в '" + QString(file.c_str()) + "'"));
        } else {
            statusLabel->setText(QString("[FAILED] Возникла проблема при сохранении"));
        }
    }
}

void MainWindow::on_openFile() {
    // откроем диалог для выбора файла
    QString f = QFileDialog::getOpenFileName(this, tr("Открыть..."), QString(), tr("KR-File (*.krdb);;All Files (*)"));
    if (f.isEmpty()) return;
    if (!mainList) mainList = new MyList();
    if (mainList->load(f.toStdString())) {
        // таблица будет заполнена при обновлении поиска
        clearForm();
        on_searchAbort();
        statusLabel->setText(QString("[  OK  ] Данные загружены из '" + f + "'"));
        file = f.toStdString();
    } else {
        statusLabel->setText(QString("[FAILED] Ошибка загрузки данных из '" + f + "'"));
    }
}

void MainWindow::on_newFile() {
    if (!mainList) mainList = new MyList();
    mainList->clear();
    clearForm();
    on_searchAbort();
}

void MainWindow::on_saveAs() {
    // получим строку, куда сохранять файл
    QString f = QFileDialog::getSaveFileName(this, tr("Сохранить как..."), QString(), tr("KR-File (*.krdb);;All Files (*)"));
    file = f.toStdString();
    if (f.isEmpty() || !mainList) return;
    if (mainList->store(f.toStdString())) {
        statusLabel->setText(QString("[  OK  ] Данные экспортированы в '" + f + "'"));
    } else {
        statusLabel->setText(QString("[FAILED] Возникла ошибка при экспорте данных в '" + f + "'"));
    }
}

void MainWindow::on_searchAbort() {
    // сначала обновим таблицу из оригинального контроллера
    reloadTableFromDB(mainList);

    // потом установим настройки поиска по умолчанию
    ui->searchName       ->setText("");
    ui->searchGenderMan  ->setChecked(true);
    ui->searchGenderWoman->setChecked(true);
    ui->searchGenderNone ->setChecked(true);
    ui->searchAgeHi      ->setMaximum(maxAge);
    ui->searchAgeLo      ->setMaximum(maxAge);
    ui->searchWeightHi   ->setMaximum(maxWeight);
    ui->searchWeightLo   ->setMaximum(maxWeight);
    ui->searchAgeHi      ->setMinimum(minAge);
    ui->searchAgeLo      ->setMinimum(minAge);
    ui->searchWeightHi   ->setMinimum(minWeight);
    ui->searchWeightLo   ->setMinimum(minWeight);
    ui->searchAgeHi      ->setValue(maxAge);
    ui->searchAgeLo      ->setValue(minAge);
    ui->searchWeightHi   ->setValue(maxWeight);
    ui->searchWeightLo   ->setValue(minWeight);
    ui->searchTypeHuman  ->setChecked(true);
    ui->searchTypeTeacher->setChecked(true);
    ui->searchTypeManager->setChecked(true);
    ui->searchTypeClient ->setChecked(true);
}

void MainWindow::on_search() { // поиск
    MyList res; // временный список

    // получаем возраст
    auto max_a = ui->searchAgeHi->value();
    auto min_a = ui->searchAgeLo->value();
    // если возраст заведомо неверный
    if (max_a < min_a) {
        statusLabel->setText(QString("[  OK  ] Найдено 0 записей. Похоже, возраст указан некорректно"));
        reloadTableFromDB(&res);
        return;
    }

    // получаем вес
    auto max_w = ui->searchWeightHi->value();
    auto min_w = ui->searchWeightLo->value();
    // если вес заведомо неверный
    if (max_w < min_w) {
        statusLabel->setText(QString("[  OK  ] Найдено 0 записей. Похоже, вес указан некорректно"));
        reloadTableFromDB(&res);
        return;
    }

    // пол
    bool noMan   = !(ui->searchGenderMan  ->isChecked());
    bool noWoman = !(ui->searchGenderWoman->isChecked());
    bool noNone  = !(ui->searchGenderNone ->isChecked());
    if (noNone && noMan && noWoman) {
        statusLabel->setText(QString("[  OK  ] Найдено 0 записей. Похоже, не выбран пол"));
        reloadTableFromDB(&res);
        return;
    }

    // пол
    bool noHuman   = !(ui->searchTypeHuman  ->isChecked());
    bool noTeacher = !(ui->searchTypeTeacher->isChecked());
    bool noManager = !(ui->searchTypeManager->isChecked());
    bool noClient  = !(ui->searchTypeClient ->isChecked());
    if (noHuman && noTeacher && noManager && noClient) {
        statusLabel->setText(QString("[  OK  ] Найдено 0 записей. Похоже, не выбраны типы"));
        reloadTableFromDB(&res);
        return;
    }

    // ФИО (может быть пустым, тогда при поиске не учитывается)
    auto name = ui->searchName->text().toStdString();
    bool empty_name = name.empty();


    for (auto item = mainList->get(); item != nullptr; item = item->next()) {
        auto i = item->get(); // получаем человека по ссылке (ВАЖНО)
        size_t id = item->id; // получаем его ID
        // для начала смотрим возраст
        if (i->getAge() > max_a || i->getAge() < min_a) continue;

        // дальшее вес
        if (i->getWeight() > max_w || i->getWeight() < min_w) continue;

        // посмотрим пол
        bool con = true;
        switch (i->getGender()) {
            case MAN:     con = noMan  ; break;
            case WOMAN:   con = noWoman; break;
            case UNKNOWN: con = noNone ; break;
        }
        if (con) continue;

        // посмотрим тип
        con = true;
        switch (i->getType()) {
            case BASE:    con = noHuman  ; break;
            case TEACHER: con = noTeacher; break;
            case MANAGER: con = noManager; break;
            case CLIENT:  con = noClient ; break;
        }
        if (con) continue;

        // посмотрим на частичное совпадение ФИО (для поиска например по имени)
        if (!empty_name) {
            if (i->getName().find(name) == std::string::npos) continue;
        }
        // все условия выполнены, так что добавляем человека в нашу временную базу и идем дальше
        res.add(id, i);
    }
    statusLabel->setText(QString("[  OK  ] Найдено " + QString::number(res.size()) + " записей"));
    reloadTableFromDB(&res);
}

void MainWindow::reloadTableFromDB(const MyList *db) {
    ui->table->clearContents();
    ui->table->setRowCount(0);
    int min_a = 0, max_a = 0;
    double min_w = 0.0, max_w = 0.0;
    //int min_a, max_a;
    //double min_w, max_w;
    if (db->get() != nullptr) {
        max_a = min_a = db->get()->get()->getAge();
        max_w = min_w = db->get()->get()->getWeight();
        for (auto i = db->get(); i != nullptr; i = i->next()) {
            auto v = i->get()->getAge();
            if (v < min_a) min_a = v;
            if (v > max_a) max_a = v;

            auto w = i->get()->getWeight();
            if (w < min_w) min_w = w;
            if (w > max_w) max_w = w;

            insertElementToTable(*i->get(), i->id);
        }
    }
    if (db == mainList) {
        ui->searchWeightHi->setMaximum(maxWeight = max_w);
        ui->searchWeightLo->setMinimum(minWeight = min_w);
        ui->searchAgeHi   ->setMaximum(maxAge    = max_a);
        ui->searchAgeLo   ->setMinimum(minAge    = min_a);
    }
}

int MainWindow::insertElementToTable(const Human &man, size_t id) { // добавление в таблицу
    int row = ui->table->rowCount(); // получим новую кол-во строк, которое и будет индексо новой строки
    ui->table->insertRow(row); // создадим строку, а потом наполним ее данными
    ui->table->setItem(row, 0, (new QTableWidgetItem(QString::number(id))));
    ui->table->setItem(row, 1, (new QTableWidgetItem(man.getName().c_str())));
    switch (man.getGender()) {
        case MAN:      ui->table->setItem(row, 2, (new QTableWidgetItem("Муж"))); break;
        case WOMAN:    ui->table->setItem(row, 2, (new QTableWidgetItem("Жен"))); break;
        case UNKNOWN:
        default:       ui->table->setItem(row, 2, (new QTableWidgetItem("Нет")));
    }
    ui->table->setItem(row, 3, (new QTableWidgetItem(QString::number(man.getAge()))));
    ui->table->setItem(row, 4, (new QTableWidgetItem(QString::number(man.getWeight()))));
    switch (man.getType()) {
        case BASE:     ui->table->setItem(row, 5, (new QTableWidgetItem("Человек" ))); break;
        case TEACHER:  ui->table->setItem(row, 5, (new QTableWidgetItem("Тренер"  ))); break;
        case MANAGER:  ui->table->setItem(row, 5, (new QTableWidgetItem("Менеджер"))); break;
        case CLIENT:   ui->table->setItem(row, 5, (new QTableWidgetItem("Клиент"  ))); break;
    }
    // запретим редактирование
    for (int i = 0; i < 6; i++) {
        QTableWidgetItem *item = ui->table->item(row, i);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }
    return row;
}

void MainWindow::clearForm() {
    // установим невалидный ID
    editID = MyList::INVALID_ID;
    // установим титл
    ui->title         ->setText("id=" + QString::number(editID));
    // сбросим ФИО, пол, возраст, вес и тип
    ui->name         ->clear();
    ui->gender       ->setCurrentIndex(0);
    ui->age          ->clear();
    ui->weight       ->clear();
    ui->type         ->setCurrentIndex(0);
    // почистим поля всех типов
    ui->teacherID    ->clear();
    ui->teacherPhone ->clear();
    ui->salary       ->clear();
    ui->managerEdu   ->setChecked(false);
    ui->clientID     ->clear();
    ui->clientEndDate->clear();
    // скроем лишние поля
    on_formTypeChange();
}

Human * MainWindow::createFromForm() {
    // определим основные поля
    auto name   = ui->name->text().toStdString();
    auto gender = (Gender)ui->gender->currentIndex();
    auto age    = ui->age->value();
    auto weight = ui->weight->value();

    // теперь определимся что создавать
    switch (ui->type->currentIndex()) {
        case BASE:
            return new Human(name, gender, age, weight);
        case TEACHER:
            return new HumanTeacher(name, gender, age, weight,
                                    ui->teacherID->value(), ui->teacherPhone->text().toStdString(), ui->salary->value());
        case MANAGER:
            return new HumanManager(name, gender, age, weight, ui->salary->value(), ui->managerEdu->isChecked());
        case CLIENT:
            return new HumanClient(name, gender, age, weight, ui->clientID->value(), ui->clientEndDate->dateTime().toSecsSinceEpoch());
        default:
            // а раз мы тут, значит тип какой-то левый
            return nullptr;
    }
}

void MainWindow::fillFormFromDB(size_t id) {
    // сначала почистим форму
    clearForm();
    // теперь подгрузим данные
    editID = id;
    auto element = mainList->get(id);
    if (element == nullptr) return;
    auto man = element->get();

    // установим поля
    ui->title      ->setText("id=" + QString::number(editID));
    ui->name       ->setText(man->getName().c_str());
    ui->gender     ->setCurrentIndex(man->getGender());
    ui->age        ->setValue(man->getAge());
    ui->weight     ->setValue(man->getWeight());
    auto type = man->getType();
    ui->type->setCurrentIndex(type);
    on_formTypeChange(); // чтобы не писать лишний код для показа полей
    switch (type) {
        case BASE:
            // базовый класс, все поля уже есть
            break;
        case TEACHER:
            // заполним поля
            {
                auto* teacher = dynamic_cast<const HumanTeacher*>(man); // пробуем апкаст
                if (teacher != nullptr) {
                    ui->teacherID->setValue(teacher->getId());
                    ui->teacherPhone->setText(teacher->getPhone().c_str());
                    ui->salary->setValue(teacher->getSalary());
                } else {
                    statusLabel->setText("[ WARM ] Объект типа HumanTeacher некорректный");
                }
            }
            break;
        case MANAGER:
            // заполним поля
            {
                auto* manager = dynamic_cast<const HumanManager*>(man); // пробуем апкаст
                if (manager != nullptr) {
                    ui->salary->setValue(manager->getSalary());
                    ui->managerEdu->setChecked(manager->isHaveEdu());
                } else {
                    statusLabel->setText("[ WARM ] Объект типа HumanManager некорректный");
                }
            }
            break;
        case CLIENT:
            // заполним поля
            {
                auto* client = dynamic_cast<const HumanClient*>(man); // пробуем апкаст
                if (client != nullptr) {
                    ui->clientID->setValue((int)client->getId());
                    QDateTime ed;
                    ed.setMSecsSinceEpoch(client->getEndOfTerm()*1000);
                    ui->clientEndDate->setDateTime(ed);
                } else {
                    statusLabel->setText("[ WARM ] Объект типа HumanClient некорректный");
                }
            }
            break;
        default:
            // а раз мы тут, значит тип какой-то левый
            statusLabel->setText("[ WARM ] Объект имеет неизвестный тип");
            clearForm();
            break;
    }
}

