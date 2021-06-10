#ifndef CLOTHES_H
#define CLOTHES_H
#include "object.h"

class Clothes: public Object_of
{

private:
    enum Variety{
      UNDERWEAR,
      JEANS,
      HAT,
      JACKET,
      SHIRT
    };
    enum Size{
        XS,
        S,
        M,
        L,
        XL
    };

    Size size;
    Variety variety;
    bool sex;
public:
    Clothes();
    ~Clothes();

    void set_variety(int var);
    void set_size(int size);
    void set_sex(bool sex);

    int get_variety() override{ return variety; }
    int get_size() override{ return size; }
    bool get_true() override{ return sex; }
    int get_number() override{return 0;}
};

#endif // CLOTHES_H
