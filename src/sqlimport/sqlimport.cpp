/**
 * @file sqlimport.cpp
 * @brief Program to import data from a CSV file into an SQLite database.
 * 
 * for simplicity, the executable should be in the same directory as the .csv file and the database file
 */


#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include <sstream>
#include <iomanip>
#include <algorithm>  // for std::count

// Constants for file names and table names
const std::string CSV_FILE_NAME = "modules-export.csv";
const std::string DATABASE_FILE_NAME = "Module.db";
const std::string TABLE_NAME = "Module";

// Constant for skipping empty lines
const bool SKIP_EMPTY_LINES = true;

/**
 * @struct StudiumData
 * @brief Structure to hold data for a study module.
 */
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


/**
 * @brief Callback function for SQLite queries.
 * @param notUsed Unused parameter.
 * @param argc Number of columns in the result set.
 * @param argv Array of strings representing column values.
 * @param azColName Array of strings representing column names.
 * @return 0 to continue processing rows.
 */
int callback(void* notUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    return 0;
}

/**
 * @brief Executes an SQL command on the SQLite database.
 * @param db SQLite database connection.
 * @param sql SQL command to execute.
 */
void executeSQLCommand(sqlite3* db, const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

/**
 * @brief Inserts study module data into the SQLite database.
 * @param db SQLite database connection.
 * @param data Study module data to insert.
 */
void insertData(sqlite3* db, const StudiumData& data) {
    std::stringstream ss;
    ss << "INSERT INTO " << TABLE_NAME << " VALUES ('" << data.Module << "', '"
       << data.Abkuerzung << "', " << data.Semester << ", '"
       << data.Anfang << "', '" << data.Abschluss << "', "
       << data.Benoetigte_Zeit_Minuten << ", '" << data.Note << "', "
       << data.SOK << ", " << data.TOK << ", " << data.Assignment << ", "
       << data.ECTS << ");";

    executeSQLCommand(db, ss.str());
}

/**
 * @brief Main function to read data from CSV and import into the SQLite database.
 * @return 0 on success, non-zero on failure.
 */
int main() {
    sqlite3* db;
    int rc = sqlite3_open(DATABASE_FILE_NAME.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    // Create the table if it does not exist
    executeSQLCommand(db, "CREATE TABLE IF NOT EXISTS " + TABLE_NAME +
                          " (Module TEXT, Abkuerzung TEXT, Semester INTEGER, Anfang DATE, Abschluss DATE, Benoetigte_Zeit_Minuten INTEGER, Note TEXT, SOK BOOLEAN, TOK BOOLEAN, Assignment INTEGER, ECTS INTEGER);");

    // Open the CSV file for reading
    std::ifstream file(CSV_FILE_NAME);
    if (!file.is_open()) {
        std::cerr << "Error opening CSV file." << std::endl;
        sqlite3_close(db);
        return -1;
    }

// Process each line in the CSV file
std::string line;
while (std::getline(file, line)) {
    // Skip empty lines if required
    if (SKIP_EMPTY_LINES && line.find_first_not_of(", \t\n") == std::string::npos) {
        continue;
    }

    // Parse data from the CSV line
    std::istringstream ss(line);
    StudiumData data;
    
    // Debug: Output the entire CSV line
    std::cout << "CSV Line: " << line << std::endl;

    std::getline(ss, data.Module, ',');
    std::cout << "Module: " << data.Module << std::endl;

    std::getline(ss, data.Abkuerzung, ',');
    std::cout << "Abkuerzung: " << data.Abkuerzung << std::endl;

    ss >> data.Semester;
    std::cout << "Semester: " << data.Semester << std::endl;
    // consume the comma
    std::getline(ss >> std::ws, line, ',');

    std::getline(ss >> std::ws, data.Anfang, ',');
    std::cout << "Anfang: " << data.Anfang << std::endl;

    std::getline(ss, data.Abschluss, ',');
    std::cout << "Abschluss: " << data.Abschluss << std::endl;

    ss >> data.Benoetigte_Zeit_Minuten;
    std::cout << "Benoetigte_Zeit_Minuten: " << data.Benoetigte_Zeit_Minuten << std::endl;
    // consume the comma
    std::getline(ss >> std::ws, line, ',');

    // consume hours
    std::getline(ss >> std::ws, line, ',');


    std::getline(ss, data.Note, ',');
    std::cout << "Note: " << data.Note << std::endl;

    char SOKChar, TOKChar;
    ss >> SOKChar >> TOKChar;
    data.SOK = (SOKChar == 'x');
    data.TOK = (TOKChar == 'x');
    std::cout << "SOK: " << data.SOK << ", TOK: " << data.TOK << std::endl;

    // Read the assignments line
    std::string assignmentLine;
    std::getline(ss, assignmentLine, ',');

    // Count the number of 'x' characters in the assignments line
    data.Assignment = std::count(assignmentLine.begin(), assignmentLine.end(), 'x');
    std::cout << "Assignment: " << data.Assignment << std::endl;

    // Debug: Output the remaining line
    std::string remainingLine;
    std::getline(ss, remainingLine);
    std::cout << "Remaining Line: " << remainingLine << std::endl;

    // Read the ECTS value
    ss >> data.ECTS;
    std::cout << "ECTS: " << data.ECTS << std::endl;

    // Debug: Output a separator line
    std::cout << "------------------------" << std::endl;

    // Insert data into the database
    insertData(db, data);
}

    // Close the SQLite database
    sqlite3_close(db);
    return 0;
}
