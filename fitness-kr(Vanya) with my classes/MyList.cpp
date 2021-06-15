#include "MyList.h"

MyList::MyList() {
    _start = nullptr;
    _end = nullptr;
    _nextID = INVALID_ID + 1;
    _len = 0;
}

MyList::MyList(const MyList &src) {
    _start = nullptr;
    _end = nullptr;
    _nextID = src._nextID;
    _len = 0;
    for (auto i = src.get(); i != nullptr; i = i->next()) {
        add(i->id, i->get());
    }
}

void MyList::add(size_t id, const Product *val) {
    // будем вставлять элементы в конец
    _len++;
    if (_start == nullptr) {
        _start = new Node(val, id);
        _end = _start;
    } else {
        // предпологаем, что _end != nullptr
        _end->pNext = new Node(val, id);
        _end = _end->pNext;
    }
}

MyList::Node::Node(const Product *val, size_t id, MyList::Node *next) : id(id) {
    pNext = next;
    if (val) obj = val->clone();
    else obj = nullptr;
}

const Product *MyList::Node::get() const {
    return obj;
}

const MyList::Node *MyList::Node::next() const {
    return pNext;
}

void MyList::Node::set(const Product *val) {
    delete obj;
    if (val) obj = val->clone();
    else obj = nullptr;
}

MyList::Node::~Node() {
    delete obj;
    // следущий объект не трогаем
}

typedef struct {
    ProductType type; // тип данных
    size_t id;
    size_t len; // длинна данных (байт)
} ObjectHeader;

bool MyList::load(const std::string &file) {
    if (file.empty()) return false;
    std::fstream f(file, std::ios::in | std::ios::binary);
    if (f.good()) {
        clear();
        f.seekg(0);

        // загрузим _nextID
        f.read((char*)&_nextID, sizeof(size_t));
        if (f.eof()) return false; // файл пустой

        for (;;) {
            // структура метаданных сериализованного объекта
            ObjectHeader tmp;
            f.read((char*)&tmp, sizeof(ObjectHeader));
            if (f.eof()) break;

            InputObjectStream in(f, tmp.len);

            // дальше смотрим что с типом, ну и для каждого типа вызываем свой конструктор
            switch (tmp.type) {
                case NOTYPE:    { Product          obj(in); add(tmp.id, &obj); } break;
                case CLOTHES: { ProductClothes   obj(in); add(tmp.id, &obj); } break;
                case ELECTRONIC: { ProductElectronic   obj(in); add(tmp.id, &obj); } break;
                case HEALTH:  { ProductHealth    obj(in); add(tmp.id, &obj); } break;
                default:
                    printf("[WARM] Неизвестный типа класса!\n");
            }
            if (in.size() != 0) {
                printf("[WARM] Объект не использует все данные и скорее всего восстановлен некорректно\n");
            }
        }
        f.close();
        return true;
    } else {
        // не получилось, не фортануло
        if (f.is_open()) f.close();
        return false;
    }
}

bool MyList::store(const std::string &file) {
    if (file.empty()) return false;
    std::fstream f(file, std::ios::out | std::ios::binary);
    if (f.good()) {
        f.seekg(0); // переходим в начало файла

        // запишем _nextID, надо чтобы потом выдавать правильные ID объектам
        f.write((char*)&_nextID, sizeof(size_t));
        OutputObjectStream out;
        for (auto element = get(); element != nullptr; element = element->next()) {
            ObjectHeader tmp; // времненная структура для записи

            tmp.id = element->id; // копируем ID
            tmp.type = element->get()->get_type(); // копируем тип

            // сериализуем
            out.reset();
            element->get()->serialize(out);
            tmp.len = out.size();

            // пишем структуру
            f.write((char*)&tmp, sizeof(tmp));

            // пишем данные
            f.write(out.dump(), (std::streamsize) tmp.len);
        }
        f.close(); // файл надо закрыть
        return true;
    } else {
        // не получилось, не фортануло
        if (f.is_open()) f.close();
        return false;
    }
}

const MyList::Node *MyList::get(size_t id) {
    return search(id);
}

const MyList::Node *MyList::get() const {
    return _start;
}

bool MyList::contains(size_t id) const {
    return search(id) != nullptr;
}

size_t MyList::add(const Product *val) {
    add(_nextID, val);
    return _nextID++;
}

bool MyList::remove(size_t id) {
    if (_start == nullptr || id == INVALID_ID) return false;
    if (_start->id == id) {
        auto* tmp = _start->pNext;
        if (_start == _end) _end = tmp;
        delete _start;
        _start = tmp;
        _len--;
        return true;
    } else {
        for (auto i = _start; i->pNext != nullptr; i = i->pNext) {
            if (i->pNext->id == id) {
                if (i->pNext == _end) _end = i;
                if (i->pNext != nullptr) {
                    auto* tmp = i->pNext->pNext;
                    delete i->pNext;
                    i->pNext = tmp;
                }
                _len--;
                return true;
            }
        }
        return false;
    }
}

bool MyList::edit(size_t id, const Product *val) {
    auto* tmp = search(id);
    if (tmp == nullptr) return false;
    tmp->set(val);
    return true;
}

MyList& MyList::operator = (const MyList &src) {
    clear();
    _nextID = src._nextID;
    for (auto i = src.get(); i != nullptr; i = i->next()) {
        add(i->id, i->get());
    }
    return *this;
}

size_t MyList::size() const {
    return _len;
}

void MyList::clear() {
    // хитрый цикл
    auto* e = _start;
    while (e != nullptr) {
        auto* tmp = e->pNext;
        delete e;
        e = tmp;
    }
    _start = nullptr;
    _end = nullptr;
    _len = 0;
}

MyList::~MyList() {
    clear();
}

MyList::Node *MyList::search(size_t id) const {
    if (_start) {
        if (_end->id == id) return _end;
        for (auto i = _start; i != nullptr; i = i->pNext) {
            if (i->id == id) return i;
        }
    }
    return nullptr;
}

