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
#include <stdexcept>
#include <algorithm> // for std::count

// Constants for file names and table names
const std::string CSV_FILE_NAME = "modules-export-new.csv";
const std::string DATABASE_FILE_NAME = "Module.db";
const std::string TABLE_NAME = "Module";

// Constant for skipping empty lines
const bool SKIP_EMPTY_LINES = true;

/**
 * @struct StudiumData
 * @brief Structure to hold data for a study module.
 */
struct StudiumData
{
    std::string Module;
    std::string Abbrev;
    int Semester;
    std::string Start;
    std::string End;
    int Minutes;
    std::string Note;
    bool SOK;
    bool TOK;
    int Assignment;
    bool LAB;
    int ECTS;
    std::string Status;
};

/**
 * @brief Callback function for SQLite queries.
 * @param notUsed Unused parameter.
 * @param argc Number of columns in the result set.
 * @param argv Array of strings representing column values.
 * @param azColName Array of strings representing column names.
 * @return 0 to continue processing rows.
 */
int callback(void *notUsed, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    return 0;
}

/**
 * @brief Executes an SQL command on the SQLite database.
 * @param db SQLite database connection.
 * @param sql SQL command to execute.
 */
void executeSQLCommand(sqlite3 *db, const std::string &sql)
{
    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

/**
 * @brief Inserts study module data into the SQLite database.
 * @param db SQLite database connection.
 * @param data Study module data to insert.
 */
void insertData(sqlite3 *db, const StudiumData &data)
{
    std::stringstream ss;
    ss << "INSERT INTO " << TABLE_NAME << " VALUES ('" << data.Module << "', '"
       << data.Abbrev << "', " << data.Semester << ", '"
       << data.Start << "', '" << data.End << "', "
       << data.Minutes << ", '" << data.Note << "', "
       << data.SOK << ", " << data.TOK << ", " << data.Assignment << ", " << data.LAB << ", "
       << data.ECTS << ", '" << data.Status << "');";

    executeSQLCommand(db, ss.str());
}

/**
 * @brief Main function to read data from CSV and import into the SQLite database.
 * @return 0 on success, non-zero on failure.
 */
int main()
{
    sqlite3 *db;
    int rc = sqlite3_open(DATABASE_FILE_NAME.c_str(), &db);
    if (rc)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    // Create the table if it does not exist
    executeSQLCommand(db, "CREATE TABLE IF NOT EXISTS " + TABLE_NAME +
                              " (Module TEXT, Abbreviation TEXT, Semester INTEGER, Start DATE, End DATE, Minutes INTEGER, Note TEXT, SOK BOOLEAN, TOK BOOLEAN, Assignment INTEGER, LAB BOOLEAN, ECTS INTEGER, Status TEXT);");

    // Open the CSV file for reading
    std::ifstream file(CSV_FILE_NAME);
    if (!file.is_open())
    {
        std::cerr << "Error opening CSV file." << std::endl;
        sqlite3_close(db);
        return -1;
    }

    // Process each line in the CSV file
    std::string line;
    while (std::getline(file, line))
    {
        // Skip empty lines if required
        if (SKIP_EMPTY_LINES && line.find_first_not_of(", \t\n") == std::string::npos)
        {
            continue;
        }

        // Parse data from the CSV line
        std::istringstream ss(line);
        StudiumData data;

        // Debug: Output the entire CSV line
        std::cout << "CSV Line: " << line << std::endl;

        // Read Module
        std::getline(ss >> std::ws, data.Module, ',');
        if (!data.Module.empty() && data.Module.front() == '"')
        {
            // Remove leading quote
            data.Module = data.Module.substr(1);
            // Module starts with a quote, read until the closing quote
            std::string temp;
            std::getline(ss, temp, '"');

            data.Module += temp;

            // Consume remaining comma
            std::getline(ss >> std::ws, line, ',');
        }
        std::cout << "Module: " << data.Module << std::endl;

        // Read Abbreviation
        std::getline(ss, data.Abbrev, ',');
        std::cout << "Abbreviation: " << data.Abbrev << std::endl;

        // Read Semester as string
        std::string semesterStr;
        std::getline(ss, semesterStr, ',');
        // Convert Semester to integer or set to NULL if empty
        data.Semester = (!semesterStr.empty()) ? std::stoi(semesterStr) : 0;
        std::cout << "Semester: " << data.Semester << std::endl;

        // Read Start
        std::getline(ss, data.Start, ',');
        std::cout << "Start: " << data.Start << std::endl;

        // Read End
        std::getline(ss, data.End, ',');
        std::cout << "End: " << data.End << std::endl;

        // Read Minutes as string
        std::string benoetigteZeitStr;
        std::getline(ss, benoetigteZeitStr, ',');
        // Convert Benoetigte_Zeit_Minuten to integer or set to NULL if empty
        data.Minutes = (!benoetigteZeitStr.empty()) ? std::stoi(benoetigteZeitStr) : 0;
        std::cout << "Minutes: " << data.Minutes << std::endl;

        // Hours
        std::getline(ss >> std::ws, line, ',');

        // Read Note
        std::getline(ss, data.Note, ',');
        std::cout << "Note: " << data.Note << std::endl;

        // Read SOK and TOK as strings
        std::string SOKStr, TOKStr;
        std::getline(ss, SOKStr, ',');
        std::getline(ss, TOKStr, ',');
        // Convert SOK and TOK to boolean
        data.SOK = (SOKStr == "x");
        data.TOK = (TOKStr == "x");
        std::cout << "SOK: " << data.SOK << ", TOK: " << data.TOK << std::endl;

        // Read Assignment
        std::string assignment;
        std::getline(ss, assignment, ',');
        data.Assignment = std::count(assignment.begin(), assignment.end(), 'x');
        std::cout << "ASS: " << data.Assignment << std::endl;

        // Read Laboratory as string
        std::string LABStr;
        std::getline(ss, LABStr, ',');
        // Convert LAB to boolean
        data.LAB = (LABStr == "x");
        std::cout << "LAB: " << data.LAB << std::endl;

        // Read ECTS as string
        std::string ectsStr;
        ss >> ectsStr;

        // Convert ECTS to integer or set to 0 if empty or invalid
        try {
            if (!ectsStr.empty()) {
                data.ECTS = std::stoi(ectsStr);
            } else {
                data.ECTS = 0;
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid ECTS value: " << ectsStr << ". Setting ECTS to 0." << std::endl;
            data.ECTS = 0;
        } catch (const std::out_of_range& e) {
            std::cerr << "ECTS value out of range: " << ectsStr << ". Setting ECTS to 0." << std::endl;
            data.ECTS = 0;
        }

        std::cout << "ECTS: " << data.ECTS << std::endl;


        // Read Status
        std::getline(ss, data.Status, ',');
        std::cout << "Status: " << data.Status << std::endl;

        // Debug: Output a separator line
        std::cout << "------------------------" << std::endl;

        // Insert data into the database
        insertData(db, data);
    }

    // Close the SQLite database
    sqlite3_close(db);
    return 0;
}
