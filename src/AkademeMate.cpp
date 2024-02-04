/**
 * @file AkademeMate.cpp
 * @brief Demonstrates the usage of the SqliteHelper class to interact with an SQLite database.
 * @version 0.1
 */

#include "sqlite_helper.h"
#include "StudyTimer.h"
using namespace std;

/**
 * @brief Main function demonstrating SQLite database interactions.
 * @return 0 on success, 1 on SQLite exception.
 */
int main() {
    try {
        SqliteHelper sqliteHelper("Main_Table.db");

        // Create Table SQL Query
        const char *createTableSQL = "CREATE TABLE IF NOT EXISTS Module_table (M_ID INTEGER PRIMARY KEY, M_short_name TEXT, M_name TEXT, M_Semester INTEGER, M_start_date DATE NOT NULL, M_end_date DATE NOT NULL, M_Time_Min INTEGER, M_Time_h REAL, M_Note REAL, M_K_A TEXT);";
        // const char *createTableSQL = "CREATE TABLE IF NOT EXISTS example_table (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);";

        sqliteHelper.executeSqlStatement(createTableSQL);

        // Insert Data SQL Query
        const char *insertDataSQL =  "INSERT INTO Module_table (M_short_name, M_name, M_Semester, M_start_date, M_end_date, M_Time_Min, M_Time_h, M_Note, M_K_A) VALUES ('SQF24', 'Schluesselqualifikationen fuer Studium und Beruf', 1,'2022-07-06', '2022-10-03', 4800, 80, 1.0, 'Assignment'), ('BWL25', 'Grundlagen des Wirtschaftens', 1, '2022-07-07', '2022-08-28', 2388, 39.8, 2.0, 'Klausur'), ('PRG25', 'Grundlagen der Informatik und Programmierung fuer Ingenieure', 1, '2022-10-05', '2022-11-05', 2370, 39.5, 1.7, 'Klausur'), ('MAT32', 'Grundlagen Mathematik', 1, '2022-07-07', '2022-10-08', 3600, 60, 1, 'Klausur'), ('WST23', 'Grundlagen der Werkstoffkunde', 1, '2022-10-06', '2022-12-03', 3300, 55, 1.3, 'Klausur'), ('ELT01', 'Elektrotechnik Grundlagen', 1, '2022-08-09', '2023-01-28', 4962, 82.7, 2, 'Klausur');";
        sqliteHelper.executeSqlStatement(insertDataSQL);

        SqliteResult result = sqliteHelper.executeSqlStatement("SELECT * FROM Module_table;");
        printf("print result:\n\n");
        result.print(); // Print the result if it's a query

    } catch (const SQLiteException &ex) {
        std::cerr << "SQLite Exception: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}