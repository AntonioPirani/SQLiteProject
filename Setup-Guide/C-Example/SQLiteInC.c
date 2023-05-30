#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

sqlite3 *db;

int openConnection(const char *url) {
    int rc = sqlite3_open(url, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    printf("Database opened successfully.\n");
    return SQLITE_OK;
}

void insert(const char *name, int number) {
    sqlite3_stmt *stmt;
    const char *query = "INSERT INTO PhoneContacts(name, number) VALUES(?, ?)";

    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, number);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error inserting data: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Data inserted successfully.\n");
    }

    sqlite3_finalize(stmt);
}

void deleteById(int id) {
    sqlite3_stmt *stmt;
    const char *query = "DELETE FROM PhoneContacts WHERE id = ?";

    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error deleting data: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Data deleted successfully.\n");
    }

    sqlite3_finalize(stmt);
}

void printEntries() {
    sqlite3_stmt *stmt;
    const char *query = "SELECT * FROM PhoneContacts";

    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *name = sqlite3_column_text(stmt, 1);
        int number = sqlite3_column_int(stmt, 2);
        printf("ID: %d, Name: %s, Number: %d\n", id, name, number);
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error retrieving data: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

int main() {
    const char *url = "test.db";
    openConnection(url);

    // Usage example
    insert("John Doe", 123456789);
    insert("Jane Smith", 987654321);
    printEntries();
    deleteById(1);
    printEntries();

    sqlite3_close(db);
    return 0;
}

