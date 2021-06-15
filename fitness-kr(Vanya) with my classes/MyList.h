#ifndef KR_FITNESS_MYLIST_H
#define KR_FITNESS_MYLIST_H

#include "Classes.h"
#include <fstream>

class MyList {
public:
private:
    size_t _nextID;
    size_t _len;
public:
    static const size_t INVALID_ID = 0;

    MyList();
    MyList(const MyList& src);

    void add(size_t id, const Product* val);

    class Node {
    protected:
        Product* obj;
    public:
        /**
         * ID объекта в базе данных
         */
        const size_t id;

        /**
         * Указатель на следущий элемент
         */
        Node* pNext;

        /**
         * @param val значение, которое надо хранить
         * @param id ID в базе данных
         * @param next следущий элемент базы данных
         */
        explicit Node(const Product* val, size_t id, Node* next = nullptr);

        const Product* get() const;
        const Node* next() const;
        void set(const Product* val);

        ~Node();
    };

    bool load(const std::string& file);
    bool store(const std::string& file);

    const Node* get(size_t id);
    const Node* get() const;
    bool contains(size_t id) const;
    size_t add(const Product* val);
    bool remove(size_t id);
    bool edit(size_t id, const Product* val);

    MyList& operator = (const MyList& src);

    size_t size() const;
    void clear();
    ~MyList();
private:
    Node* _start;
    Node* _end;
    Node* search(size_t id) const;
};

#endif //KR_FITNESS_MYLIST_H
