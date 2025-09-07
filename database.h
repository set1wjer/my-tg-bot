#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>

class Database {
private:
    sqlite3* db;

public:
    Database(const std::string& filename);
    ~Database();
    void createTables();
    void addUser(int64_t userId, const std::string& username);
    double getBalance(int64_t userId);
    std::string myTransaction(int64_t userId);
    void addTransaction(int64_t userId, double amount, const std::string& description);
};

#endif