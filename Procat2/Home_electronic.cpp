#include "object.h"

Home_electr::Home_electr()
{

}

Home_electr::~Home_electr()
{

}

void Home_electr::set_power(int number)
{
    power = number;
}

void Home_electr::set_size_container(int number)
{
    size_container = number;
}

void Home_electr::set_variety(int var)
{
    switch (var) {
    case 0:
        variety = Variety::WASHER;
        break;
    case 1:
        variety = Variety::IRON;
        break;
    case 2:
        variety = Variety::HOOVER;
        break;
    }
}
