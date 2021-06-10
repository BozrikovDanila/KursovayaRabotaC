#include "object.h"

Health_care::Health_care()
{

}

Health_care::~Health_care()
{

}


void Health_care::set_variety(int var)
{
    switch(var)
    {
    case 0:
        variety = Variety::INHALER;
        break;
    case 1:
        variety = Variety::TOOTHBRUSH;
        break;
    case 2:
        variety = Variety::MASSAGER;
        break;
    }
}

void Health_care::set_variety_of(int var)
{
    switch (var) {
    case 1:
        variety_of = Var_of::PAROVOY;
        break;
    case 11:
        variety_of = Var_of::ULTRAZVUK;
        break;
    case 111:
        variety_of = Var_of::COMPRESSION;
        break;
    case 2:
        variety_of = Var_of::SOFT;
        break;
    case 22:
        variety_of = Var_of::MIDDLE;
        break;
    case 222:
        variety_of = Var_of::HARD;
        break;
    case 3:
        variety_of = Var_of::FEET;
        break;
    case 33:
        variety_of = Var_of::BODY;
        break;
    case 333:
        variety_of = Var_of::BACK;
        break;
    }
}

void Health_care::set_intencity_change(bool value)
{
    intencity_change = value;
}

void Health_care::set_size_container(int number)
{
    size_container = number;
}

