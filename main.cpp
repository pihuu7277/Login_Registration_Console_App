#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>

MYSQL* conn;

bool initializeDatabase() {
    conn = mysql_init(NULL);
    if (conn == NULL) {
        std::cerr << "mysql_init() failed\n";
        return false;
    }

    if (mysql_real_connect(conn, "localhost", "root", "Aditya@7277", "login_app", 3306, NULL, 0) == NULL) {
        std::cerr << "mysql_real_connect() failed\n";
        mysql_close(conn);
        return false;
    }

    std::cout << "Connected to database successfully!" << std::endl;
    return true;
}

void registerUser() {
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    std::string query = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "')";

    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Registration failed: " << mysql_error(conn) << std::endl;
    } else {
        std::cout << "Registration successful!" << std::endl;
    }
}

bool loginUser() {
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    std::string query = "SELECT password FROM users WHERE username = '" + username + "'";

    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Login failed: " << mysql_error(conn) << std::endl;
        return false;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res == NULL) {
        std::cerr << "mysql_store_result() failed\n";
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    if (row) {
        std::string storedPassword = row[0];
        if (password == storedPassword) {
            std::cout << "Login successful!" << std::endl;
            mysql_free_result(res);
            return true;
        } else {
            std::cout << "Invalid username or password." << std::endl;
        }
    } else {
        std::cout << "Invalid username or password." << std::endl;
    }

    mysql_free_result(res);
    return false;
}
int main()
{
if (!initializeDatabase()) {
        return -1;
    }

    int choice;
    while (true) {
        std::cout << "1. Register\n2. Login\n3. Exit\nChoose an option: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser()) {
                    std::cout << "Welcome!\n";
                }
                break;
            case 3:
                std::cout << "Exiting...\n";
                mysql_close(conn);
                return 0;
            default:
                std::cout << "Invalid option. Try again.\n";
        }
    }

    mysql_close(conn);
    return 0;
}
