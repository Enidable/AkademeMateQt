/**
 * @file sqlite_helper.cpp
 * @brief Implementation of the SqliteHelper class.
 */

#include "sqlite_helper.h"

SqliteHelper::SqliteHelper(const char *dbName) : db(nullptr) {
    openDatabase(dbName);
}

SqliteHelper::~SqliteHelper() {
    if (db) {
        sqlite3_close(db);
    }
}

void SqliteHelper::openDatabase(const char *dbName) {
    int rc = sqlite3_open(dbName, &db);
    if (rc != SQLITE_OK) {
        throw SQLiteException("Error opening the database: " + std::string(sqlite3_errmsg(db)));
    }
}

SqliteResult SqliteHelper::executeSqlStatement(const char *sqlStatement) {
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sqlStatement, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        throw SQLiteException("Error preparing SQL statement: " + std::string(sqlite3_errmsg(db)));
    }

    int columns = sqlite3_column_count(stmt);

    // Collect header information
    std::vector<std::string> header;
    for (int i = 0; i < columns; i++) {
        header.push_back(sqlite3_column_name(stmt, i));
    }

    // Create a local variable to store the results
    std::vector<std::vector<std::string>> result;

    // Collect result data
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::vector<std::string> row;

        for (int i = 0; i < columns; i++) {
            const char *columnValue = reinterpret_cast<const char *>(sqlite3_column_text(stmt, i));
            row.push_back(columnValue ? columnValue : "NULL");
        }

        result.push_back(row);
    }

    sqlite3_finalize(stmt);

    return SqliteResult(result, header);
}
