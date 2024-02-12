# CMakeLists.txt for src/sqlimport

# Set the executable filename
set(SQL_IMPORT_EXECUTABLE sqlimport)

# Add all source files in the directory
add_executable(${SQL_IMPORT_EXECUTABLE} sqlimport.cpp)

# Link against SQLite (assuming SQLite has been added as a library)
target_link_libraries(${SQL_IMPORT_EXECUTABLE} sqlite3)
