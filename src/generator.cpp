#include "generator.hpp"
#include "json.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Generator::Generator(const Args &args): _args(args) {
    _readElementsFromDataFile();
}

Generator::~Generator() {
    if (_db) {
        sqlite3_close(_db);
        _db = nullptr;
    }
}

void Generator::generateNewDatabase() {
    _setupSQLiteDatabaseFile();
    _createElementDatabaseTable();
    _insertElementsIntoDatabase();
}

std::vector<Generator::elements_data_t> Generator::getElements() const {
    return _elements;
}

void Generator::_readElementsFromDataFile() {
    // Note: the official data file from NLM uses JSON-encoding
    std::cout << "Reading all elements from data file…";

    std::ifstream inputFile(_args.getPathToInputFile());
    nlohmann::json json;
    inputFile >> json;

    const auto &rows = json["Table"]["Row"];
    for (const auto &rowItem : rows) {
        const auto &cell = rowItem["Cell"];

        elements_data_t element;
        element.atomicNumber = atoi(((std::string) cell[0]).c_str());
        element.symbol = (std::string) cell[1];
        element.name = (std::string) cell[2];
        element.atomicMass = (std::string) cell[3];
        element.hexColor = (std::string) cell[4];
        element.electronConfiguration = (std::string) cell[5];
        element.electronegativity = (std::string) cell[6];
        element.atomicRadius = (std::string) cell[7];
        element.ionizationEnergy = (std::string) cell[8];
        element.electronAffinity = (std::string) cell[9];
        element.oxidationStates = (std::string) cell[10];
        element.standardState = (std::string) cell[11];
        element.meltingPoint = (std::string) cell[12];
        element.boilingPoint = (std::string) cell[13];
        element.density = (std::string) cell[14];
        element.groupBlock = (std::string) cell[15];
        element.yearDiscovered = (std::string) cell[16];
        
        _elements.push_back(element);
    }

    std::cout << " success!\n";
}

void Generator::_setupSQLiteDatabaseFile() {
    // Note: always delete any existing database file first!

    std::cout << "Setting up SQLite database file…";

    const std::string &outputFilePath = _args.getPathToOutputFile();
    std::filesystem::remove(outputFilePath);
    if (sqlite3_open(outputFilePath.c_str(), &_db) != SQLITE_OK) {
        throw std::runtime_error("Failed to create and open SQLite database!");
    }

    std::cout << " success!\n";
}

void Generator::_createElementDatabaseTable() {
    // Note: uses the definition based on NLM's current version of data file

    std::cout << "Creating elements database table…";

    const char *elementTableCreateSql = "CREATE TABLE \"element\"(\"atomic_number\" INTEGER UNIQUE NOT NULL PRIMARY KEY,\"symbol\" TEXT UNIQUE NOT NULL,\"name\" TEXT UNIQUE NOT NULL,\"atomic_mass\" TEXT,\"electron_configuration\" TEXT,\"electro_negativity\" TEXT,\"atomic_radius\" TEXT,\"ionization_energy\" TEXT,\"oxidation_states\" TEXT,\"standard_state\" TEXT,\"melting_point\" TEXT,\"boiling_point\" TEXT,\"density\" TEXT,\"group_block\" TEXT,\"year_discovered\" TEXT);";
    const size_t len = strlen(elementTableCreateSql);

    int rc;
    char *errMsg;

    rc = sqlite3_exec(_db, elementTableCreateSql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << errMsg << '\n';
        throw std::runtime_error("Failed to create element database table!");
    }

     std::cout << " success!\n";
}

void Generator::_insertElementsIntoDatabase() {
    std::cout << "Inserting all elements into database…";

    int rc;
    char *errMsg;

    sqlite3_stmt *stmt;
    const char *elementInsertSql = "INSERT INTO \"element\" VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    const size_t elementInsertSqlLen = strlen(elementInsertSql);

    rc = sqlite3_prepare_v2(_db, elementInsertSql, elementInsertSqlLen + 1, &stmt, 0);
    if (rc != SQLITE_OK) {
        std::cerr << sqlite3_errmsg(_db) << '\n';
        throw std::runtime_error("Invalid insert SQL for element!");
    }

    rc = sqlite3_exec(_db, "BEGIN TRANSACTION;", 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << errMsg << '\n';
        throw std::runtime_error("Failed to start database transaction!");
    }
    
    for (const auto &element : _elements) {
        rc = sqlite3_bind_int(stmt, 1, element.atomicNumber);
        rc = sqlite3_bind_text(stmt, 2, element.symbol.c_str(), element.symbol.size() + 1, 0);
        rc = sqlite3_bind_text(stmt, 3, element.name.c_str(), element.name.size() + 1, 0);
        rc = sqlite3_bind_text(stmt, 4, element.atomicMass.c_str(), element.atomicMass.size() + 1, 0);
        rc = sqlite3_bind_text(stmt, 5, element.electronConfiguration.c_str(), element.electronConfiguration.size() + 1, 0);
        rc = sqlite3_bind_text(stmt, 6, element.electronegativity.c_str(), element.electronegativity.size() + 1, 0);
        rc = sqlite3_bind_text(stmt, 7, element.atomicRadius.c_str(), element.atomicRadius.size() + 1, 0);
        rc = sqlite3_bind_text(stmt, 8, element.ionizationEnergy.c_str(), element.ionizationEnergy.size() + 1, 0);
        rc = sqlite3_bind_text(stmt, 9, element.oxidationStates.c_str(), element.oxidationStates.size() + 1, 0);
        rc = sqlite3_bind_text(stmt, 10, element.standardState.c_str(), element.standardState.size() + 1, 0);
        rc = sqlite3_bind_text(stmt, 11, element.meltingPoint.c_str(), element.meltingPoint.size() + 1, 0);
        rc = sqlite3_bind_text(stmt, 12, element.boilingPoint.c_str(), element.boilingPoint.size() + 1, 0);
        rc = sqlite3_bind_text(stmt, 13, element.density.c_str(), element.density.size() + 1, 0);
        rc = sqlite3_bind_text(stmt, 14, element.groupBlock.c_str(), element.groupBlock.size() + 1, 0);
        rc = sqlite3_bind_text(stmt, 15, element.yearDiscovered.c_str(), element.yearDiscovered.size() + 1, 0);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << sqlite3_errmsg(_db) << '\n';
            throw std::runtime_error("Failed to insert element into table!");
        }

        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
    }
    sqlite3_finalize(stmt);

    rc = sqlite3_exec(_db, "COMMIT;", 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << errMsg << '\n';
        throw std::runtime_error("Failed to end database transaction!");
    }

    std::cout << " success!\n";
}
