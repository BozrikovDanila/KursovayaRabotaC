#include <string>
#include <cstring>
#include <utility>
#include "Classes.h"

/*********** ВСПОМОГАТЕЛЬНЫЕ ИНСТРУМЕНТЫ ***********/

OutputObjectStream::OutputObjectStream() {
    _allocated = 512;
    _data = (char*) malloc(_allocated);
    _len = 0;
}

const char* OutputObjectStream::dump() const {
    return _data;
}

size_t OutputObjectStream::size() const {
    return _len;
}

void OutputObjectStream::write(const std::string& src) {
    size_t size = src.size();
    write(&size, sizeof(size_t));
    write(src.data(), size);
}

void OutputObjectStream::write(const void* src, size_t len) {
    if (len < 1) return;
    // определим, надо ли еще памяти
    const size_t _end = _len + len;
    if (_end > _allocated) {
        if (_end & 0x01FF) {
            _allocated = (_end & 0x01FF) + 512;
        } else {
            _allocated = _end;
        }
        _data = (char*) realloc(_data, _allocated);
    }
    memcpy(_data + _len, src, len);
    _len = _end;
}

void OutputObjectStream::reset() {
    _len = 0;
}

OutputObjectStream::~OutputObjectStream() {
    free(_data);
}

InputObjectStream::InputObjectStream(std::istream& in, size_t len) : _len(len) {
    _offset = 0;
    _data = (char*)malloc(_len);
    memset(_data, 0, _len);
    in.read(_data, (std::streamsize)_len);
}

size_t InputObjectStream::size() const {
    return _len - _offset;
}

void InputObjectStream::read(std::string& dest) {
    size_t size = 0;
    read(&size, sizeof(size_t));
    if (size == 0) {
        dest = "";
    } else {
        char tmp[size + 1];
        read(tmp, size);
        tmp[size] = 0;
        dest = tmp;
    }
}

void InputObjectStream::read(void* dest, size_t len) {
    if (size() >= len) {
        memcpy(dest, _data + _offset, len);
        _offset += len;
    } else {
        _offset = _len - 1;
        memset(dest, 0, len);
    }
}

InputObjectStream::~InputObjectStream() {
    free(_data);
}

/************* ВСПОМОГАТЕЛЬНЫЕ МАКРОСЫ *************/

#define STR(p) p(std::move(p))
#define INIT_PARAMS Human(std::move(name), gender, age, weight)

/***************** КЛАСС  ЧЕЛОВЕКА *****************/

Human::Human(const Human& src) : name(src.name) {
    gender = src.gender;
    age = src.age;
    weight = src.weight;
}

Human::Human(InputObjectStream& in) {
    in.read(name);
    in.read(&gender, sizeof(gender));
    in.read(&age, sizeof(age));
    in.read(&weight, sizeof(weight));
}

Human::Human(BASE_PARAMS) : name(std::move(name)) {
    this->gender = gender;
    this->age = age;
    this->weight = weight;
}

const std::string& Human::getName() const {
    return name;
}

Gender Human::getGender() const {
    return gender;
}

int Human::getAge() const {
    return age;
}

double Human::getWeight() const {
    return weight;
}

void Human::serialize(OutputObjectStream& out) const {
    out.write(name);
    out.write(&gender, sizeof(gender));
    out.write(&age, sizeof(age));
    out.write(&weight, sizeof(weight));
}

HumanType Human::getType() const {
    return BASE;
}

Human* Human::clone() const {
    return new Human(*this);
}

Human::~Human() = default;

/***************** КЛАСС   ТРЕНЕРА *****************/

HumanTeacher::HumanTeacher(const HumanTeacher& src) : Human(src), phone(src.phone) {
    id = src.id;
    salary = src.salary;
}

HumanTeacher::HumanTeacher(InputObjectStream& in) : Human(in) {
    in.read(&id, sizeof(id));
    in.read(phone);
    in.read(&salary, sizeof(salary));
}

HumanTeacher::HumanTeacher(BASE_PARAMS, int id, std::string phone, int salary) : INIT_PARAMS, STR(phone) {
    this->id = id;
    this->salary = salary;
}

int HumanTeacher::getId() const {
    return id;
}

const std::string& HumanTeacher::getPhone() const {
    return phone;
}

int HumanTeacher::getSalary() const {
    return salary;
}

void HumanTeacher::serialize(OutputObjectStream& out) const {
    Human::serialize(out);
    out.write(&id, sizeof(id));
    out.write(phone);
    out.write(&salary, sizeof(salary));
}

HumanType HumanTeacher::getType() const {
    return TEACHER;
}

Human* HumanTeacher::clone() const {
    return new HumanTeacher(*this);
}

HumanTeacher::~HumanTeacher() = default;

/***************** КЛАСС МЕНЕДЖЕРА *****************/

HumanManager::HumanManager(const HumanManager& src) : Human(src) {
    salary = src.salary;
    haveEdu = src.haveEdu;
}

HumanManager::HumanManager(InputObjectStream& in) : Human(in) {
    in.read(&salary, sizeof(salary));
    in.read(&haveEdu, sizeof(haveEdu));
}

HumanManager::HumanManager(BASE_PARAMS, int salary, bool haveEdu) : INIT_PARAMS {
    this->salary = salary;
    this->haveEdu = haveEdu;
}

int HumanManager::getSalary() const {
    return salary;
}

bool HumanManager::isHaveEdu() const {
    return haveEdu;
}

void HumanManager::serialize(OutputObjectStream& out) const {
    Human::serialize(out);
    out.write(&salary, sizeof(salary));
    out.write(&haveEdu, sizeof(haveEdu));
}

HumanType HumanManager::getType() const {
    return MANAGER;
}

Human* HumanManager::clone() const {
    return new HumanManager(*this);
}

HumanManager::~HumanManager() = default;

/***************** КЛАСС   КЛИЕНТА *****************/

HumanClient::HumanClient(const HumanClient& src) : Human(src) {
    id = src.id;
    endOfTerm = src.endOfTerm;
}

HumanClient::HumanClient(InputObjectStream& in) : Human(in) {
    in.read(&id, sizeof(id));
    in.read(&endOfTerm, sizeof(endOfTerm));
}

HumanClient::HumanClient(BASE_PARAMS, size_t id, time_t endOfTerm) : INIT_PARAMS {
    this->id = id;
    this->endOfTerm = endOfTerm;
}

size_t HumanClient::getId() const {
    return id;
}

time_t HumanClient::getEndOfTerm() const {
    return endOfTerm;
}

void HumanClient::serialize(OutputObjectStream& out) const {
    Human::serialize(out);
    out.write(&id, sizeof(id));
    out.write(&endOfTerm, sizeof(endOfTerm));
}

HumanType HumanClient::getType() const {
    return CLIENT;
}

Human* HumanClient::clone() const {
    return new HumanClient(*this);
}

HumanClient::~HumanClient() = default;
