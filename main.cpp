#include <iostream>
#include <string>
extern "C" {
#include "sqlite3.h"
}


using namespace std;

// Callback para listar resultados do SELECT
static int callback(void* NotUsed, int argc, char** argv, char** azColName){
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    cout << "-----------------------\n";
    return 0;
}

// Executa comandos SQL simples (sem retorno)
void executeSQL(sqlite3* db, const string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), callback, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Erro SQL: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void createTable(sqlite3* db) {
    string sql = R"(
        CREATE TABLE IF NOT EXISTS contactos (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            nome TEXT NOT NULL,
            telefone TEXT,
            email TEXT
        );
    )";
    executeSQL(db, sql);
}

void addContact(sqlite3* db) {
    string nome, telefone, email;
    cout << "Nome: ";
    getline(cin, nome);
    cout << "Telefone: ";
    getline(cin, telefone);
    cout << "Email: ";
    getline(cin, email);

    string sql = "INSERT INTO contactos (nome, telefone, email) VALUES ('" 
                 + nome + "', '" + telefone + "', '" + email + "');";

    executeSQL(db, sql);
    cout << "Contacto adicionado!\n";
}

void listContacts(sqlite3* db) {
    cout << "\nLista de contactos:\n";
    string sql = "SELECT * FROM contactos;";
    executeSQL(db, sql);
}

void searchContact(sqlite3* db) {
    cout << "Digite o nome para buscar: ";
    string nome;
    getline(cin, nome);

    string sql = "SELECT * FROM contactos WHERE nome LIKE '%" + nome + "%';";
    executeSQL(db, sql);
}

void editContact(sqlite3* db) {
    cout << "Digite o ID do contacto para editar: ";
    string id;
    getline(cin, id);

    cout << "Novo nome (enter para manter): ";
    string nome; getline(cin, nome);
    cout << "Novo telefone (enter para manter): ";
    string telefone; getline(cin, telefone);
    cout << "Novo email (enter para manter): ";
    string email; getline(cin, email);

    if (!nome.empty()) {
        executeSQL(db, "UPDATE contactos SET nome = '" + nome + "' WHERE id = " + id + ";");
    }
    if (!telefone.empty()) {
        executeSQL(db, "UPDATE contactos SET telefone = '" + telefone + "' WHERE id = " + id + ";");
    }
    if (!email.empty()) {
        executeSQL(db, "UPDATE contactos SET email = '" + email + "' WHERE id = " + id + ";");
    }
    cout << "Contacto atualizado!\n";
}

void removeContact(sqlite3* db) {
    cout << "Digite o ID do contacto para remover: ";
    string id; 
    getline(cin, id);

    string sql = "DELETE FROM contactos WHERE id = " + id + ";";
    executeSQL(db, sql);
    cout << "Contacto removido!\n";
}

int main() {
    sqlite3* db;
    int rc = sqlite3_open("agenda.db", &db);
    if (rc) {
        cerr << "Não foi possível abrir o banco de dados: " << sqlite3_errmsg(db) << "\n";
        return 1;
    }

    createTable(db);

    int choice;
    do {
        cout << "\n--- Agenda de Contactos ---\n";
        cout << "1. Adicionar contacto\n";
        cout << "2. Listar contactos\n";
        cout << "3. Buscar contacto\n";
        cout << "4. Editar contacto\n";
        cout << "5. Remover contacto\n";
        cout << "6. Sair\n";
        cout << "Escolha uma opção: ";
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1: addContact(db); break;
            case 2: listContacts(db); break;
            case 3: searchContact(db); break;
            case 4: editContact(db); break;
            case 5: removeContact(db); break;
            case 6: cout << "Saindo...\n"; break;
            default: cout << "Opção inválida!\n";
        }
    } while(choice != 6);

    sqlite3_close(db);
    return 0;
}
