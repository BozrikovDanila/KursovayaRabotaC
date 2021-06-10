#include "object.h"

void Object_of::set_code(int number)
{
    code = number;
}

void Object_of::set_cost(int number)
{
    cost = number;
}

void Object_of::set_type(int var)
{
    switch (var) {
    case 0:
        type = Type::CLOTHES;
        break;
    case 1:
        type = Type::HOME_ELECTONIC;
        break;
    case 2:
        type = Type::HEALTH_CARE;
        break;
    }
}

void Object_of::set_name(QString str)
{
    name = str;
}

void Object_of::set_arend_time(QString str1, QString str2)
{
     arend_time[0] = str1;
     arend_time[1] = str2;
}


Object_of::~Object_of()
{
}
