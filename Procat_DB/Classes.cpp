#include <string>
#include <cstring>
#include <utility>
#include "Classes.h"


OutputObjectStream::OutputObjectStream() {
    allocated = 512;
    data = (char*) malloc(allocated);
    length = 0;
}

const char* OutputObjectStream::dump() const {
    return data;
}

size_t OutputObjectStream::size() const {
    return length;
}

void OutputObjectStream::write(const std::string& src) {
    size_t size = src.size();
    write(&size, sizeof(size_t));
    write(src.data(), size);
}

void OutputObjectStream::write(const void* src, size_t len) {
    if (len < 1) return;

    const size_t _end = length + len;
    if (_end > allocated) {
        if (_end & 0x01FF) {
            allocated = (_end & 0x01FF) + 512;
        } else {
            allocated = _end;
        }
        data = (char*) realloc(data, allocated);
    }
    memcpy(data + length, src, len);
    length = _end;
}

void OutputObjectStream::reset() {
    length = 0;
}

OutputObjectStream::~OutputObjectStream() {
    free(data);
}

InputObjectStream::InputObjectStream(std::istream& in, size_t len) : length(len) {
    offset = 0;
    data = (char*)malloc(length);
    memset(data, 0, length);
    in.read(data, (std::streamsize)length);
}

size_t InputObjectStream::size() const {
    return length - offset;
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
        memcpy(dest, data + offset, len);
        offset += len;
    } else {
        offset = length - 1;
        memset(dest, 0, len);
    }
}

InputObjectStream::~InputObjectStream() {
    free(data);
}


Product::Product(std::string name, int code, double cost, std::string dateFrom, std::string dateTo) : name(std::move(name)), dateFrom(std::move(dateFrom)), dateTo(std::move(dateTo))
{
    this->code = code;
    this->cost = cost;
}

Product::Product(const Product &imp) : name(imp.name), dateFrom(imp.dateFrom), dateTo(imp.dateTo)
{
    code = imp.code;
    cost = imp.cost;
}

Product::Product(InputObjectStream &input)
{
    input.read(name);
    input.read(&code, sizeof(code));
    input.read(&cost, sizeof(cost));
    input.read(dateFrom);
    input.read(dateTo);
}

Product::~Product() = default;

const std::string &Product::get_name() const
{
    return name;
}

int Product::get_code() const
{
    return code;
}

double Product::get_cost() const
{
    return cost;
}

const std::string &Product::get_term_to() const
{
    return dateTo;
}

void Product::serialize(OutputObjectStream &output) const
{
    output.write(name);
    output.write(&code, sizeof(code));
    output.write(&cost, sizeof(cost));
    output.write(dateFrom);
    output.write(dateTo);
}

ProductType Product::get_type() const
{
    return NOTYPE;
}

Product *Product::clone() const
{
    return new Product(*this);
}

const std::string &Product::get_term_from() const
{
    return dateFrom;
}


/*****************Clothes***********************/

ProductClothes::ProductClothes(std::string name, int code, double cost, std::string dateFrom, std::string dateTo, CategoriesClothes category, Sex sex, Size size) : Product(std::move(name), code, cost, std::move(dateFrom), std::move(dateTo))
{
    this->category = category;
    this->size = size;
    this->sex = sex;
}

ProductClothes::ProductClothes(const ProductClothes &imp) : Product(imp)
{
    category = imp.category;
    size = imp.size;
    sex = imp.sex;
}

ProductClothes::~ProductClothes() = default;

ProductClothes::ProductClothes(InputObjectStream &input) : Product(input)
{
    input.read(&category, sizeof(category));
    input.read(&size, sizeof(size));
    input.read(&sex, sizeof(sex));
}

CategoriesClothes ProductClothes::get_category() const
{
    return category;
}

Sex ProductClothes::get_sex() const
{
    return sex;
}

Size ProductClothes::get_size() const
{
    return size;
}

void ProductClothes::serialize(OutputObjectStream &output) const
{
    Product::serialize(output);
    output.write(&category, sizeof(category));
    output.write(&size, sizeof(size));
    output.write(&sex, sizeof(sex));
}

ProductType ProductClothes::get_type() const
{
    return CLOTHES;
}

Product *ProductClothes::clone() const
{
    return new ProductClothes(*this);
}

/********************ELECTRONIC******************/

ProductElectronic::ProductElectronic(std::string name, int code, double cost, std::string dateFrom, std::string dateTo, CategoriesElectronic category, int power) : Product(std::move(name), code, cost, std::move(dateFrom), std::move(dateTo))
{
    this->category = category;
    this->power = power;
}

ProductElectronic::ProductElectronic(const ProductElectronic &imp) : Product(imp)
{
    category = imp.category;
    power = imp.power;
}

ProductElectronic::ProductElectronic(InputObjectStream &input) : Product(input)
{
    input.read(&category,sizeof(category));
    input.read(&power,sizeof(power));
}

CategoriesElectronic ProductElectronic::get_category() const
{
    return category;
}

int ProductElectronic::get_power() const
{
    return power;
}

void ProductElectronic::serialize(OutputObjectStream &output) const
{
    Product::serialize(output);
    output.write(&category, sizeof(category));
    output.write(&power, sizeof(power));
}

ProductType ProductElectronic::get_type() const
{
    return ELECTRONIC;
}

Product *ProductElectronic::clone() const
{
    return new ProductElectronic(*this);
}

ProductElectronic::~ProductElectronic() = default;

/******************HEALTH********************/


ProductHealth::ProductHealth(std::string name, int code, double cost, std::string dateFrom, std::string dateTo, CategoriesHealth category, Variety variety) : Product(std::move(name), code, cost, std::move(dateFrom), std::move(dateTo))
{
    this->category = category;
    this->variety = variety;
}

ProductHealth::ProductHealth(const ProductHealth &imp) : Product(imp)
{
    category = imp.category;
    variety = imp.variety;
}

ProductHealth::ProductHealth(InputObjectStream &input) : Product(input)
{
    input.read(&category, sizeof(category));
    input.read(&variety, sizeof(variety));
}

CategoriesHealth ProductHealth::get_category() const
{
    return category;
}

Variety ProductHealth::get_variety() const
{
    return variety;
}

void ProductHealth::serialize(OutputObjectStream &output) const
{
    Product::serialize(output);
    output.write(&category, sizeof (category));
    output.write(&variety, sizeof (variety));
}

ProductType ProductHealth::get_type() const
{
    return HEALTH;
}

Product *ProductHealth::clone() const
{
    return new ProductHealth(*this);
}

ProductHealth::~ProductHealth() = default;
