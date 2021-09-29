#ifndef MYLIST_H
#define MYLIST_H

#include "Classes.h"
#include <fstream>

class MyList {
private:
    size_t nextID;
    size_t Size;
public:
    static const size_t INC_ID = 0;

    MyList();
    MyList(const MyList& copy);

    ~MyList();

    void add(size_t id, const Product* el);

    class Node {
    protected:
        Product* obj;
    public:

        const size_t id;

        Node* pNext;

        explicit Node(const Product* val, size_t id, Node* next = nullptr);

        const Product* get() const;
        const Node* next() const;
        void set(const Product* el);

        ~Node();
    };

    bool load(const std::string& file);
    bool reserve(const std::string& file);

    size_t size() const;
    bool exists(size_t id) const;

    void clear();
    bool remove(size_t id);
    size_t add(const Product* val);
    bool edit(size_t id, const Product* val);

    const Node* get(size_t id);
    const Node* get() const;

    MyList& operator = (const MyList& src);
private:
    Node* head;
    Node* tail;
    Node* at(size_t id) const;
};

#endif //MYLIST_H
