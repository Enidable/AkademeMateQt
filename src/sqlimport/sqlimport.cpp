#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include <sstream>
#include <iomanip>

struct StudiumData {
    std::string Module;
    std::string Abkuerzung;
    int Semester;
    std::string Anfang;
    std::string Abschluss;
    int Benoetigte_Zeit_Minuten;
    std::string Note;
    bool SOK;
    bool TOK;
    int Assignment;
    int ECTS;
};

int callback(void* notUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    return 0;
}

void executeSQLCommand(sqlite3* db, const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void insertData(sqlite3* db, const StudiumData& data) {
    std::stringstream ss;
    ss << "INSERT INTO Module VALUES ('" << data.Module << "', '"
       << data.Abkuerzung << "', " << data.Semester << ", '"
       << data.Anfang << "', '" << data.Abschluss << "', "
       << data.Benoetigte_Zeit_Minuten << ", '" << data.Note << "', "
       << data.SOK << ", " << data.TOK << ", " << data.Assignment << ", "
       << data.ECTS << ");";

    executeSQLCommand(db, ss.str());
}

int main() {
    sqlite3* db;
    int rc = sqlite3_open("Module.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    executeSQLCommand(db, "CREATE TABLE IF NOT EXISTS Module (Module TEXT, Abkuerzung TEXT, Semester INTEGER, Anfang DATE, Abschluss DATE, Benoetigte_Zeit_Minuten INTEGER, Note TEXT, SOK BOOLEAN, TOK BOOLEAN, Assignment INTEGER, ECTS INTEGER);");

    std::ifstream file("modules-export.csv");
    if (!file.is_open()) {
        std::cerr << "Error opening CSV file." << std::endl;
        sqlite3_close(db);
        return -1;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        StudiumData data;
        std::getline(ss, data.Module, ',');
        std::getline(ss, data.Abkuerzung, ',');
        ss >> data.Semester;
        std::getline(ss >> std::ws, data.Anfang, ',');
        std::getline(ss, data.Abschluss, ',');
        ss >> data.Benoetigte_Zeit_Minuten;
        std::getline(ss, data.Note, ',');
        char SOKChar, TOKChar;
        ss >> SOKChar >> TOKChar;
        data.SOK = (SOKChar == 'x');
        data.TOK = (TOKChar == 'x');
        std::string assignment;
        ss >> assignment;
        data.Assignment = assignment.length();
        ss >> data.ECTS;

        // Process data or insert directly into the database
        insertData(db, data);
    }

    sqlite3_close(db);
    return 0;
}
