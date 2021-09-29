#ifndef CLASSES_H
#define CLASSES_H

#include <string>
#include <istream>

class OutputObjectStream {
private:
    size_t allocated;
    size_t length;
    char* data;
public:
    explicit OutputObjectStream();

    const char* dump() const;

    size_t size() const;

    void write(const std::string& src);

    void write(const void* src, size_t len);

    void reset();

    ~OutputObjectStream();
};


class InputObjectStream {
private:
    const size_t length;
    size_t offset;
    char* data;
public:

    InputObjectStream(std::istream& in, size_t len);

    size_t size() const;

    void read(std::string& dest);

    void read(void *dest, size_t len);

    ~InputObjectStream();
};


enum ProductType{
    NOTYPE = 0, CLOTHES, ELECTRONIC, HEALTH
};

class Product{
protected:
    std::string name;
    int code{};
    double cost{};
    std::string dateFrom;
    std::string dateTo;
public:
    Product(std::string name, int code, double cost, std::string dateFrom, std::string dateTo);

    Product(const Product& src);

    Product(InputObjectStream& input);

    virtual ~Product();


    const std::string& get_name() const;
    int get_code() const;
    double get_cost() const;
    const std::string& get_term_from() const;
    const std::string& get_term_to() const;

    virtual void serialize(OutputObjectStream& output) const;

    virtual ProductType get_type() const;

    virtual Product* clone() const;
};


enum CategoriesClothes{
    UNDERWEAR = 0,
    JEANS,
    HAT,
    JACKET,
    SHIRT
};

enum Sex{
    DONT_INDICATE = 0, MALE, FEMALE
};

enum Size{
    XS = 0, S, M, L, XL
};

class ProductClothes: public Product{
protected:
    CategoriesClothes category = UNDERWEAR;
    Sex sex = DONT_INDICATE;
    Size size = XS;
public:
    ProductClothes(std::string name, int code, double cost, std::string dateFrom, std::string dateTo, CategoriesClothes category, Sex sex, Size size);

    ProductClothes(const ProductClothes& imp);

    ProductClothes(InputObjectStream& input);

    ~ProductClothes() override;

    CategoriesClothes get_category() const;
    Sex get_sex() const;
    Size get_size() const;

    void serialize(OutputObjectStream& output) const override;
    ProductType get_type() const override;
    Product* clone() const override;
};


enum CategoriesElectronic{
    WASHING = 0, IRON, VACUUM_CLEANER
};

class ProductElectronic: public Product{
protected:
    CategoriesElectronic category = WASHING;
    int power{};

public:
    ProductElectronic(std::string name, int code, double cost, std::string dateFrom, std::string dateTo, CategoriesElectronic category, int power);

    ProductElectronic(const ProductElectronic& imp);

    ProductElectronic(InputObjectStream& input);

    ~ProductElectronic() override;

    CategoriesElectronic get_category() const;
    int get_power() const;

    void serialize(OutputObjectStream& output) const override;
    ProductType get_type() const override;
    Product* clone() const override;
};


enum CategoriesHealth{
    INHALER = 0, TOOTHBRUSH, MASSAGER
};

enum Variety{
    STEAM = 0, ULTRASONIC, COMPRESSION,
    SOFT, MEDIUM, HARD,
    LEGS, NECK, BACK
};

class ProductHealth: public Product{
protected:
    CategoriesHealth category = INHALER;
    Variety variety = STEAM;
public:
    ProductHealth(std::string name, int code, double cost, std::string dateFrom, std::string dateTo, CategoriesHealth category, Variety variety);

    ProductHealth(const ProductHealth& imp);

    ProductHealth(InputObjectStream& input);

    ~ProductHealth() override;


    CategoriesHealth get_category() const;
    Variety get_variety() const;

    void serialize(OutputObjectStream& output) const override;
    ProductType get_type() const override;
    Product* clone() const override;
};

#endif //CLASSES_H
