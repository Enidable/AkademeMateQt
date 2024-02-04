/**
 * @file sqlite_helper.h
 * @brief Declaration of the SqliteHelper class and related exceptions.
 *
 * Defines the SqliteHelper class for interacting with SQLite databases.
 * Defines SQLiteException class for handling SQLite-related errors.
 */

#ifndef SQLITE_HELPER_H
#define SQLITE_HELPER_H

#include <cstring>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sqlite3.h>
#include <stdexcept>
#include <vector>
#include "sqlite_result.h"

/**
 * @brief Custom exception class for SQLite-related errors.
 *
 * This exception class is derived from std::runtime_error and is used to handle errors
 * that occur during SQLite operations.
 */
class SQLiteException : public std::runtime_error {
  public:
    /**
     * @brief Constructs an SQLiteException with a given error message.
     * @param[in] message The error message.
     */
    SQLiteException(const std::string &message) : std::runtime_error(message) {}
};

/**
 * @class SqliteHelper
 * @brief Provides functionality to interact with SQLite databases.
 *
 * The SqliteHelper class encapsulates common SQLite operations, such as opening a database,
 * executing SQL statements, and retrieving results from SELECT queries.
 */
class SqliteHelper {
  public:
    /**
     * @brief Constructor for SqliteHelper.
     * @param[in] dbName Name of the SQLite database file.
     */
    SqliteHelper(const char *dbName);

    /**
     * @brief Destructor for SqliteHelper.
     * 
     * Destroys the SqliteHelper object and closes the SQLite database connection.
     */
    ~SqliteHelper();

    /**
     * @brief Executes an SQL statement that does not return results (e.g., INSERT, UPDATE, DELETE).
     * @param[in] sqlStatement The SQL statement to be executed.
     * @throw SQLiteException if the execution of the statement fails.
     */
    // void executeSqlStatement(const char *sqlStatement);

    /**
     * @brief Executes an SQL statement and returns a result object.
     * @param[in] sqlStatement The SQL statement to be executed.
     * @return A SqliteResult object representing the result set.
     * @throw SQLiteException if the execution of the statement fails.
     */
    SqliteResult executeSqlStatement(const char *sqlStatement);
 

private:
    /**
     * @brief SQLite Database Connection.
     * 
     * The 'db' pointer encapsulates the SQLite database connection for each SqliteHelper instance.
     * The pointer is initialized in the constructor and closed in the destructor.
     */
    sqlite3 *db;

    /**
     * @brief Opens a connection to an SQLite database.
     * @param[in] dbName Name of the database.
     * @throw SQLiteException if opening the database fails.
     */
    void openDatabase(const char *dbName);
};

#endif // SQLITE_HELPER_H
