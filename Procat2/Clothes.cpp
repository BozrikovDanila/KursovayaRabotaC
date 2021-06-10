#include "Clothes.h"

Clothes::Clothes()
{

}

Clothes::~Clothes()
{

}


void Clothes::set_variety(int var)
{
    switch(var){
    case 0:
        variety = Variety::UNDERWEAR;
        break;
    case 1:
        variety = Variety::JEANS;
        break;
    case 2:
        variety = Variety::HAT;
        break;
    case 3:
        variety = Variety::JACKET;
        break;
    case 4:
        variety = Variety::SHIRT;
        break;
    }
}

void Clothes::set_size(int size)
{
    switch(size){
    case 0:
        size = Size::XS;
        break;
    case 1:
        size = Size::S;
        break;
    case 2:
        size = Size::M;
        break;
    case 3:
        size = Size::L;
        break;
    case 4:
        size = Size::XL;
        break;
    }
}

void Clothes::set_sex(bool gender)
{
    sex = gender;
}


