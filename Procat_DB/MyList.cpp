#include "MyList.h"

MyList::MyList() {
    head = nullptr;
    tail = nullptr;
    nextID = INC_ID + 1;
    Size = 0;
}

MyList::MyList(const MyList &copy) {
    head = nullptr;
    tail = nullptr;

    nextID = copy.nextID;
    Size = 0;

    for (auto i = copy.get(); i != nullptr; i = i->next()) {
        add(i->id, i->get());
    }
}

void MyList::add(size_t id, const Product *el) {
    Size++;
    if (head == nullptr) {
        head = new Node(el, id);
        tail = head;
    } else {
        tail->pNext = new Node(el, id);
        tail = tail->pNext;
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

void MyList::Node::set(const Product *el) {
    delete obj;
    if (el) obj = el->clone();
    else obj = nullptr;
}

MyList::Node::~Node() {
    delete obj;
}

typedef struct {
    ProductType type;
    size_t id;
    size_t len;
} ObjHeader;

bool MyList::load(const std::string &file) {
    if (file.empty()) return false;

    std::fstream f(file, std::ios::in | std::ios::binary);

    if (f.good()) {
        clear(); // clear list
        f.seekg(0); // cursor on the start of file

        f.read((char*)&nextID, sizeof(size_t));
        if (f.eof()) return false;  // checking for end of file

        for (;;) {
            ObjHeader tmp;
            f.read((char*)&tmp, sizeof(ObjHeader));
            if (f.eof()) break; // if end of file break

            InputObjectStream in(f, tmp.len); // create input object

            switch (tmp.type) {
                case NOTYPE: {
                    Product obj(in);
                    add(tmp.id, &obj);
                    break;
                }
                case CLOTHES: {
                    ProductClothes obj(in);
                    add(tmp.id, &obj);
                    break;
                }
                case ELECTRONIC: {
                    ProductElectronic obj(in);
                    add(tmp.id, &obj);
                    break;
                }
                case HEALTH: {
                    ProductHealth obj(in);
                    add(tmp.id, &obj);
                    break;
                }
                default:
                    printf("[ INFO ] Неизвестный тип класса!\n");
            }
            if (in.size() != 0) {
                printf("[ FAILED ] Данные получены некорректно\n");
            }
        }
        f.close();
        return true;
    } else {
        if (f.is_open()) f.close();
        return false;
    }
}

bool MyList::reserve(const std::string &file) {
    if (file.empty()) return false;
    std::fstream f(file, std::ios::out | std::ios::binary);

    if (f.good()) {
        f.seekg(0);

        f.write((char*)&nextID, sizeof(size_t));
        OutputObjectStream out;
        for (auto i = get(); i != nullptr; i = i->next()) {
            ObjHeader tmp;

            tmp.id = i->id;
            tmp.type = i->get()->get_type();

            out.reset();
            i->get()->serialize(out);
            tmp.len = out.size();

            f.write((char*)&tmp, sizeof(tmp));

            f.write(out.dump(), (std::streamsize) tmp.len);
        }
        f.close();
        return true;
    } else {
        if (f.is_open()) f.close();
        return false;
    }
}

const MyList::Node *MyList::get(size_t id) {
    return at(id);
}

const MyList::Node *MyList::get() const {
    return head;
}

bool MyList::exists(size_t id) const {
    return at(id) != nullptr;
}

size_t MyList::add(const Product *val) {
    add(nextID, val);
    return nextID++;
}

bool MyList::remove(size_t id) {
    if (head == nullptr || id == INC_ID) return false;

    if (head->id == id) {
        auto* tmp = head->pNext;

        if (head == tail) tail = tmp; // here tmp == nullptr; start == end, if it`s only 1 element in list
        delete head;
        head = tmp;

        Size--;
        return true;
    } else {
        for (auto i = head; i->pNext != nullptr; i = i->pNext) {
            if (i->pNext->id == id) {

                if (i->pNext == tail) tail = i;

                if (i->pNext != nullptr) {
                    auto* tmp = i->pNext->pNext;
                    delete i->pNext;
                    i->pNext = tmp;
                }

                Size--;
                return true;
            }
        }
        return false;
    }
}

bool MyList::edit(size_t id, const Product *val) {
    auto* tmp = at(id);
    if (tmp == nullptr) return false;
    tmp->set(val);
    return true;
}

MyList& MyList::operator = (const MyList &src) {
    clear();
    nextID = src.nextID;
    for (auto i = src.get(); i != nullptr; i = i->next()) {
        add(i->id, i->get());
    }
    return *this;
}

size_t MyList::size() const {
    return Size;
}

void MyList::clear() {
    auto* del = head;

    while (del != nullptr) {
        auto* tmp = del->pNext;
        delete del;
        del = tmp;
    }

    head = nullptr;
    tail = nullptr;
    Size = 0;
}

MyList::~MyList() {
    clear();
}

MyList::Node *MyList::at(size_t id) const {
    if (head) {
        if (tail->id == id) return tail;
        for (auto i = head; i != nullptr; i = i->pNext) {
            if (i->id == id) return i;
        }
    }
    return nullptr;
}

