#ifndef KR_FITNESS_CLASSES_H
#define KR_FITNESS_CLASSES_H

#include <string>
#include <istream>

/**
 * Класс для упрощения сериализации объектов
 */
class OutputObjectStream {
private:
    size_t _allocated;
    size_t _len;
    char* _data;
public:
    explicit OutputObjectStream();

    /**
     * @return указатель на участок памяти с данными
     */
    const char* dump() const;

    /**
     * @return размер буффера в байтах
     */
    size_t size() const;

    /**
     * записать строку в поток
     * @param src строка, которую надо записать
     */
    void write(const std::string& src);

    /**
     * записать произвольный объект в поток
     * @param src указатель на объект
     * @param len размер объекта (в байтах)
     */
    void write(const void* src, size_t len);

    /**
     * сбросить данные потока, не освобождает память
     */
    void reset();

    ~OutputObjectStream();
};

/**
 * Класс для упрощения десериализации объектов
 */
class InputObjectStream {
private:
    const size_t _len;
    char* _data;
    size_t _offset;
public:
    /**
     * @param in поток, откуда читать данные
     * @param len длинна буффера
     */
    InputObjectStream(std::istream& in, size_t len);

    /**
     * @return разер буффера, доступного для чтения
     */
    size_t size() const;

    /**
     * Прочитать строку из потока
     * @param dest строка-приемник
     */
    void read(std::string& dest);

    /**
     *  прочитать произвольный объект из потока
     * @param dest указатель, куда поместить данные
     * @param len размер объекта
     */
    void read(void *dest, size_t len);
    ~InputObjectStream();
};

enum HumanType {
    BASE = 0,
    TEACHER,
    MANAGER,
    CLIENT
};

enum Gender {
    UNKNOWN = 0, MAN, WOMAN
};

#define BASE_PARAMS std::string name, Gender gender, int age, double weight

/***************** КЛАСС  ЧЕЛОВЕКА *****************/
// человек (базовый класс): фио, пол, возраст, вес
class Human {
protected:
    // фио
    std::string name;
    // пол
    Gender gender = UNKNOWN;
    // возраст
    int age{};
    // вес
    double weight{};
public:
    /**
     * Конструктор, позволяющий клонировать объект (небезопасно)
     * @param src объект для клонирования
     */
    Human(const Human& src);

    /**
     * Конструктор, позволяющий десериализовать объект
     * @param in объект, содержащий сериализованные поля
     */
    explicit Human(InputObjectStream& in);

    /**
     * Тривиальный конструктор
     */
    Human(BASE_PARAMS);

    const std::string& getName() const;
    Gender getGender() const;
    int getAge() const;
    double getWeight() const;

    /**
     * Сериализовать объект
     * @param out объект-поток, куда записывать вывод
     */
    virtual void serialize(OutputObjectStream& out) const;

    /**
     * @return Тип класса
     */
    virtual HumanType getType() const;

    /**
     * Безопасное клонирование объекта
     * @return клонированный объект
     */
    virtual Human* clone() const;

    virtual ~Human();
};

/***************** КЛАСС   ТРЕНЕРА *****************/
// тренер: ID, телефон, ЗП
class HumanTeacher : public Human {
protected:
    int id{};
    std::string phone;
    int salary{};
public:
    /**
     * Конструктор, позволяющий клонировать объект (небезопасно)
     * @param src объект для клонирования
     */
    HumanTeacher(const HumanTeacher& src);

    /**
     * Конструктор, позволяющий десериализовать объект
     * @param in объект, содержащий сериализованные поля
     */
    explicit HumanTeacher(InputObjectStream& in);

    /**
     * Тривиальный конструктор
     */
    HumanTeacher(BASE_PARAMS, int id, std::string phone, int salary);

    int getId() const;
    const std::string& getPhone() const;
    int getSalary() const;

    void serialize(OutputObjectStream& out) const override;
    HumanType getType() const override;
    Human* clone() const override;

    ~HumanTeacher() override;
};

/***************** КЛАСС МЕНЕДЖЕРА *****************/
// HumanManager (менеджер): ЗП, есть ли вышка
class HumanManager : public Human {
protected:
    int salary{};
    bool haveEdu{};
public:
    /**
     * Конструктор, позволяющий клонировать объект (небезопасно)
     * @param src объект для клонирования
     */
    HumanManager(const HumanManager& src);

    /**
     * Конструктор, позволяющий десериализовать объект
     * @param in объект, содержащий сериализованные поля
     */
    explicit HumanManager(InputObjectStream& in);

    /**
     * Тривиальный конструктор
     */
    HumanManager(BASE_PARAMS, int salary, bool haveEdu);

    int getSalary() const;
    bool isHaveEdu() const;

    void serialize(OutputObjectStream& out) const override;
    HumanType getType() const override;
    Human* clone() const override;

    ~HumanManager() override;
};

/***************** КЛАСС   КЛИЕНТА *****************/
// HumanClient (клиент): ID абонемента, срок окончания абонемента
class HumanClient : public Human {
protected:
    size_t id{};
    time_t endOfTerm{};
public:
    /**
     * Конструктор, позволяющий клонировать объект (небезопасно)
     * @param src объект для клонирования
     */
    HumanClient(const HumanClient& src);

    /**
     * Конструктор, позволяющий десериализовать объект
     * @param in объект, содержащий сериализованные поля
     */
    explicit HumanClient(InputObjectStream& in);

    /**
     * Тривиальный конструктор
     */
    HumanClient(BASE_PARAMS, size_t id, time_t endOfTerm);

    size_t getId() const;
    time_t getEndOfTerm() const;

    void serialize(OutputObjectStream& out) const override;
    HumanType getType() const override;
    Human* clone() const override;

    ~HumanClient() override;
};


/*****************************************************************/


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
    STEAM = 0, ULTRASONIC = 1, COMPRESSION = 2,
    SOFT = 10, MEDIUM = 11, HARD = 12,
    LEGS = 20, NECK = 21, BACK = 22
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

#endif //KR_FITNESS_CLASSES_H
