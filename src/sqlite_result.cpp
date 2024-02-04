/**
 * @file sqlite_result.cpp
 * @brief Implementation of the SqliteResult class.
 */

#include "sqlite_helper.h"

SqliteResult::SqliteResult(const std::vector<std::vector<std::string>>& result, const std::vector<std::string>& header)
    : result_(result), header_(header) {}

SqliteResult::~SqliteResult() {}

size_t SqliteResult::getRowCount() const {
    return result_.size();
}

size_t SqliteResult::getColumnCount() const {
    if (!result_.empty()) {
        return result_[0].size();
    }
    return 0;
}

bool SqliteResult::empty() const {
    return result_.empty();
}

void SqliteResult::print() const {
    if (result_.empty()) {
        std::cout << "Empty result set." << std::endl;
        return;
    }

    int columns = header_.size();

    // Calculate maximum width of each column
    std::vector<int> columnWidths(columns, 0);

    // Iterate through the rows to find the maximum width of data in each column
    for (const auto &row : result_) {
        for (int i = 0; i < columns; i++) {
            columnWidths[i] = std::max(columnWidths[i], static_cast<int>(row[i].length()));
        }
    }

    // Print header with column names
    for (int i = 0; i < columns; i++) {
        std::cout << std::setw(columnWidths[i]) << std::left << header_[i];
        if (i < columns - 1) {
            std::cout << " | ";
        }
    }
    std::cout << std::endl;
    
    // Print line of dashes
    for (int i = 0; i < 3 * columns + std::accumulate(columnWidths.begin(), columnWidths.end(), 0) - 1; i++) {
        std::cout << "-";
    }
    std::cout << std::endl;

    // Print table
    for (size_t rowIndex = 0; rowIndex < result_.size(); ++rowIndex) {
        const auto &row = result_[rowIndex];
        for (int i = 0; i < columns; i++) {
            std::cout << std::setw(columnWidths[i]) << std::left << row[i];
            if (i < columns - 1) {
                std::cout << " | ";
            }
        }
        std::cout << std::endl;

    }
}
