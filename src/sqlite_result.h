/**
 * @file sqlite_result.h
 * @brief Declaration of the SqliteResult class for handling SQLite query results.
 *
 * Defines the SqliteResult class to encapsulate the result of an SQLite SELECT query,
 * including the result set, header, and methods for printing formatted output.
 */

#ifndef SQLITE_RESULT_H
#define SQLITE_RESULT_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <numeric>

/**
 * @class SqliteResult
 * @brief Encapsulates the result of an SQLite SELECT query.
 * 
 * Represents the result of an SQLite query, including the header and data.
 */
class SqliteResult {
public:
    /**
     * @brief Constructor for SqliteResult.
     * @param[in] result The result set of the SELECT query.
     * @param[in] header The header with column names.
     */
    SqliteResult(const std::vector<std::vector<std::string>>& result, const std::vector<std::string>& header);

    /**
     * @brief Destructor for SqliteResult.
     */
    ~SqliteResult();

    /**
     * @brief Gets the number of rows in the result set.
     * @return The number of rows.
     */
    size_t getRowCount() const;

    /**
     * @brief Gets the number of columns in the result set.
     * @return The number of columns.
     */
    size_t getColumnCount() const;

    /**
     * @brief Checks if the result set is empty.
     * @return True if the result set is empty, false otherwise.
     */
    bool empty() const;


    /**
     * @brief Prints the result set with formatted output.
     */
    void print() const;

private:
    /**
     * @brief The result set of the SELECT query.
     */
    std::vector<std::vector<std::string>> result_;

    /**
     * @brief The header with column names.
     */
    std::vector<std::string> header_;
};

#endif // SQLITE_RESULT_H
