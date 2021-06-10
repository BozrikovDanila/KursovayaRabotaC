#ifndef OBJECT_H
#define OBJECT_H

#include<string>
#include<iostream>
#include <QTextStream>


class Object_of
{

private:
    enum Type{
        CLOTHES = 0,
        HOME_ELECTONIC = 1,
        HEALTH_CARE = 2
    };
    Type type;
    int code;
    QString name;
    int cost;
    QStringList arend_time{"",""};


public:
    virtual ~Object_of();

    void set_type(int var);
    void set_name(QString str);
    void set_code(int number);
    void set_cost(int number);
    void set_arend_time(QString str1, QString str2);

    QString get_name(){ return name; }
    int get_code(){ return code; }
    int get_cost(){ return cost; }
    QString get_arend_time_1(){ return arend_time[0]; }
    QString get_arend_time_2(){ return arend_time[1]; }
    int get_type(){ return type; }

    virtual int get_variety() = 0;
    virtual int get_size() = 0;
    virtual int get_number() = 0;
    virtual bool get_true() = 0;

};





class Home_electr: public Object_of
{
private:
    enum Variety{
        WASHER,
        IRON,
        HOOVER
    };
    Variety variety;
    int size_container;
    int power;
public:
    Home_electr();
    ~Home_electr();

    void set_size_container(int number);
    void set_power(int number);
    void set_variety(int var);

    int get_size() override{ return size_container; }
    int get_number() override{ return power; }
    int get_variety() override{ return variety; }
    bool get_true() override{return 0;}
};

class Health_care: public Object_of
{
private:
    enum Variety{
      INHALER,
      TOOTHBRUSH,
      MASSAGER
    };
    enum Var_of{
        PAROVOY = 1,
        ULTRAZVUK = 11,
        COMPRESSION = 111,
        SOFT = 2,
        MIDDLE = 22,
        HARD = 222,
        FEET = 3,
        BODY = 33,
        BACK = 333
    };
    Variety variety;
    Var_of variety_of;
    int size_container;
    bool intencity_change;

public:
    Health_care();
    ~Health_care();

    void set_variety(int var);
    void set_variety_of(int var);
    void set_size_container(int number);
    void set_intencity_change(bool value);

    int get_variety() override{ return variety; }
    int get_number() override{ return variety_of; }
    int get_size() override{ return size_container; }
    bool get_true() override{ return intencity_change; }
};




template<typename T>
class List
{
private:

    template<typename S>
    class Node
    {
    public:
        Node* pNext;
        Node* pPrev;
        S data;

        Node(S data = S(), Node* pNext = nullptr, Node* pPrev = nullptr)
        {
            this->data = data;
            this->pNext = pNext;
            this->pPrev = pPrev;
        }

    };

    Node<T>* head;
    Node<T>* tail;
    int Size;
public:
    List<T>();
    ~List<T>();

    void push_back(T data);
    void pop_back();
    void replace(T data,const int index);
    bool remove(const int index);

    int size() { return Size; }



    T& last();
    T& first();
    T& at(const int index);
    T& operator [](const int index);
    T* element(const int index);

};

template<typename T>
List<T>::List()
{
    Size = 0;
    head = tail = nullptr;
}

template<typename T>
List<T>::~List()
{
    head = tail = nullptr;
}

template<typename T>
T* List<T>::element(const int index)
{
    if(index < (Size / 2))
    {
        Node<T> *temp = this->head;

        for(int i = 0; i > index - 1; i++)
            temp->pNext;

        return temp;
    }
    else
    {
        Node<T> *temp = this->tail;

        for(int i = Size; i > index + 1; i--)
            temp->pPrev;

        return temp;
    }
}

template<typename T>
void List<T>::push_back(T data)
{
    if (head == nullptr)
    {
        head = tail = new Node<T>(data);
    }
    else
    {
        tail = this->tail->pNext = new Node<T>(data, nullptr, this->tail);
    }

    Size++;
};


template<typename T>
T& List<T>::last()
{
    return tail->data;
}


template<typename T>
void List<T>::pop_back()
{
    Node<T>* current = this->tail->pPrev;

    this->pPrev->pNext = nullptr;

    delete this->tail;

    tail = current;

    Size--;
}

template<typename T>
bool List<T>::remove(const int index)
{
    if(head == nullptr || index >= Size || index < 0);
}
/*
 * if(index < (Size / 2))
    {
        Node<T>* current = this->head;

        if(index == 0)
        {
            head = head->pNext;
            head->pPrev = nullptr;
            delete current;
        }
        else
        {
            for(int i = 0; i < index - 1; i++)
                current = current->pNext;

            current->pPrev->pNext = current->pNext;
            current->pNext->pPrev = current->pPrev;
            delete current;
            return;
        }
    }
    else
    {
        Node<T>* current = this->tail;

        if(index == Size - 1)
        {
            if(tail->pPrev != nullptr)tail = tail->pPrev;
            tail->pNext = nullptr;
            delete current;
        }
        else
        {
            for(int i = Size; i > index + 1; i--)
                current = current->pPrev;

            current->pPrev->pNext = current->pNext;
            current->pNext->pPrev = current->pPrev;
            delete current;
            return;
        }
    }

    Size--;
*/
template<typename T>
void List<T>::replace(T data,const int index)
{
   this->operator[](index) = data;
}


template<typename T>
T& List<T>::first()
{
    return head->data;
}


template<typename T>
T& List<T>::at(const int index)
{
    if(index < (Size / 2))
    {
        int counter = 0;
        Node<T>* current = this->head;
        while (current->pNext != nullptr)
        {
            if (counter == index)
            {
                return current->data;
            }

            current = current->pNext;
            counter++;
        }
    }
    else
    {
        int counter = Size;
        Node<T>* current = this->tail;
        while (current->pPrev != nullptr)
        {
            if (counter == index)
            {
                return current->data;
            }

            current = current->pPrev;
            counter--;
        }
    }
}

template<typename T>
T& List<T>::operator[](const int index)
{
    if(index <= size())
    {
        if(index < (Size / 2))
        {
            int counter = 0;
            Node<T>* current = this->head;
            while (current->pNext != nullptr)
            {
                if (counter == index)
                {
                    return current->data;
                }

                current = current->pNext;
                counter++;
            }
        }
        else
        {
            int counter = Size;
            Node<T>* current = this->tail;
            while (current->pPrev != nullptr)
            {
                if (counter == index)
                {
                    return current->data;
                }

                current = current->pPrev;
                counter--;
            }
        }
    }
}



#endif // OBJECT_H
