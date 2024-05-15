#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "args.hpp"
#include "sqlite3.h"
#include <vector>

class Generator {
public:
    /// Stores expected element data from data file.
    /// These fields are specific to current version of data file.
    typedef struct elements_data_t {
        int atomicNumber;
        std::string symbol;
        std::string name;
        std::string atomicMass;
        std::string hexColor;
        std::string electronConfiguration;
        std::string electronegativity;
        std::string atomicRadius;
        std::string ionizationEnergy;
        std::string electronAffinity;
        std::string oxidationStates;
        std::string standardState;
        std::string meltingPoint;
        std::string boilingPoint;
        std::string density;
        std::string groupBlock;
        std::string yearDiscovered;
    } elements_data;
public:
    explicit Generator(const Args&);
    ~Generator();
    void generateNewDatabase();
    std::vector<elements_data_t> getElements() const;
private:
    const Args &_args;
    std::vector<elements_data_t> _elements;
    sqlite3 *_db;
private:
    void _readElementsFromDataFile();
    void _setupSQLiteDatabaseFile();
    void _createElementDatabaseTable();
    void _insertElementsIntoDatabase();
};

#endif//GENERATOR_HPP