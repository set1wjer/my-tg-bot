#include "database.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Database::Database(const std::string& filename) {
    int rc = sqlite3_open(filename.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    }
    else {
        std::cout << "Database opened successfully" << std::endl;
        createTables();
    }
}

Database::~Database() {
    sqlite3_close(db);
    std::cout << "Database closed";
}

void Database::createTables() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY,
            username TEXT,
            balance REAL DEFAULT 0
        );
        
        CREATE TABLE IF NOT EXISTS transactions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER,
            amount REAL,
            description TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(user_id) REFERENCES users(id)
        );
    )";

    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void Database::addUser(int64_t userId, const std::string& username) {
    const char* sql = "INSERT OR IGNORE INTO users (id, username) VALUES (?, ?)";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int64(stmt, 1, userId);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

double Database::getBalance(int64_t userId) {
    const char* sql = "SELECT balance FROM users WHERE id = ?";
    sqlite3_stmt* stmt;
    double balance = 0.0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int64(stmt, 1, userId);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            balance = sqlite3_column_double(stmt, 0);
        }
    }
    else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return balance;
}

std::string Database::myTransaction(int64_t userId)
{
    const char* sql = "SELECT amount, description, created_at FROM transactions WHERE user_id = ?";
    sqlite3_stmt* stmt;

    std::ostringstream oss;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int64(stmt, 1, userId);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            double amount = sqlite3_column_double(stmt, 0);
            std::string description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string createdAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

            // фикс ширины колонок
            oss << std::fixed << std::setprecision(2)
                << std::right << std::setw(8) << amount << " "
                << std::left << std::setw(15) << description
                << createdAt << "\n";

        }
    }

    sqlite3_finalize(stmt);

    std::string text = oss.str();
    if (text.empty()) {
        text = "No transactions yet.";
    }

    return text;
}

void Database::addTransaction(int64_t userId, double amount, const std::string& description)
{
    const char* sqlInsert = "INSERT INTO transactions (user_id, amount, description) VALUES (?, ?, ?)";
    sqlite3_stmt* stmtInsert;


    if (sqlite3_prepare_v2(db, sqlInsert, -1, &stmtInsert, NULL) == SQLITE_OK) {
        sqlite3_bind_int64(stmtInsert, 1, userId);
        sqlite3_bind_double(stmtInsert, 2, amount);
        sqlite3_bind_text(stmtInsert, 3, description.c_str(), -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmtInsert) != SQLITE_DONE) {
            std::cerr << "Insert transaction failed" << sqlite3_errmsg(db) << std::endl;
        }

    } else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmtInsert);




    const char* sqlUpdate = "UPDATE users SET balance = balance + ? WHERE id = ?";
    sqlite3_stmt* stmtUpdate;

    if (sqlite3_prepare_v2(db, sqlUpdate, -1, &stmtUpdate, NULL) == SQLITE_OK) {
        sqlite3_bind_double(stmtUpdate, 1, amount);
        sqlite3_bind_int64(stmtUpdate, 2, userId);

        if (sqlite3_step(stmtUpdate) != SQLITE_DONE) {
            std::cerr << "Update balance failed: " << sqlite3_errmsg(db) << std::endl;
        }

    } else { 

        std::cerr << "Failed to prepare update statment: " << sqlite3_errmsg(db) << std::endl;

    }

    sqlite3_finalize(stmtUpdate);

    

}
