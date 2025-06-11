#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

struct Contacto {
    std::string nome;
    std::string telefone;
    std::string email;
};

std::vector<Contacto> contactos;

void carregarContactos(const std::string& filename) {
    contactos.clear();
    std::ifstream file(filename);
    if (!file) return;

    std::string linha;
    while (std::getline(file, linha)) {
        std::stringstream ss(linha);
        Contacto c;
        std::getline(ss, c.nome, ';');
        std::getline(ss, c.telefone, ';');
        std::getline(ss, c.email, ';');
        contactos.push_back(c);
    }
    file.close();
}

void salvarContactos(const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& c : contactos) {
        file << c.nome << ";" << c.telefone << ";" << c.email << ";\n";
    }
    file.close();
}

void adicionarContacto() {
    Contacto c;
    std::cout << "Nome: ";
    std::getline(std::cin, c.nome);
    std::cout << "Telefone: ";
    std::getline(std::cin, c.telefone);
    std::cout << "Email: ";
    std::getline(std::cin, c.email);

    contactos.push_back(c);
    std::cout << "Contacto adicionado com sucesso!\n";
}

void listarContactos() {
    std::cout << "\n--- Lista de Contactos ---\n";
    for (size_t i = 0; i < contactos.size(); i++) {
        std::cout << i + 1 << ". " << contactos[i].nome << " | " 
                  << contactos[i].telefone << " | " 
                  << contactos[i].email << "\n";
    }
    if (contactos.empty()) {
        std::cout << "Nenhum contacto encontrado.\n";
    }
}

void buscarContacto() {
    std::cout << "Digite o nome para buscar: ";
    std::string nomeBusca;
    std::getline(std::cin, nomeBusca);
    bool encontrado = false;
    for (const auto& c : contactos) {
        if (c.nome.find(nomeBusca) != std::string::npos) {
            std::cout << "Encontrado: " << c.nome << " | " << c.telefone << " | " << c.email << "\n";
            encontrado = true;
        }
    }
    if (!encontrado) {
        std::cout << "Nenhum contacto com esse nome encontrado.\n";
    }
}

void editarContacto() {
    listarContactos();
    if (contactos.empty()) return;

    std::cout << "Digite o número do contacto que deseja editar: ";
    std::string input;
    std::getline(std::cin, input);
    int indice = std::stoi(input) - 1;

    if (indice >= 0 && indice < (int)contactos.size()) {
        Contacto& c = contactos[indice];
        std::cout << "Novo nome (atual: " << c.nome << "): ";
        std::string novoNome;
        std::getline(std::cin, novoNome);
        if (!novoNome.empty()) c.nome = novoNome;

        std::cout << "Novo telefone (atual: " << c.telefone << "): ";
        std::string novoTelefone;
        std::getline(std::cin, novoTelefone);
        if (!novoTelefone.empty()) c.telefone = novoTelefone;

        std::cout << "Novo email (atual: " << c.email << "): ";
        std::string novoEmail;
        std::getline(std::cin, novoEmail);
        if (!novoEmail.empty()) c.email = novoEmail;

        std::cout << "Contacto atualizado com sucesso!\n";
    } else {
        std::cout << "Número inválido.\n";
    }
}

void removerContacto() {
    listarContactos();
    if (contactos.empty()) return;

    std::cout << "Digite o número do contacto que deseja remover: ";
    std::string input;
    std::getline(std::cin, input);
    int indice = std::stoi(input) - 1;

    if (indice >= 0 && indice < (int)contactos.size()) {
        contactos.erase(contactos.begin() + indice);
        std::cout << "Contacto removido com sucesso!\n";
    } else {
        std::cout << "Número inválido.\n";
    }
}

int main() {
    const std::string filename = "agenda.txt";
    carregarContactos(filename);

    while (true) {
        std::cout << "\n--- Agenda de Contactos ---\n";
        std::cout << "1. Adicionar contacto\n";
        std::cout << "2. Listar contactos\n";
        std::cout << "3. Buscar contacto\n";
        std::cout << "4. Editar contacto\n";
        std::cout << "5. Remover contacto\n";
        std::cout << "6. Sair\n";
        std::cout << "Escolha uma opção: ";

        std::string opcao;
        std::getline(std::cin, opcao);

        if (opcao == "1") {
            adicionarContacto();
            salvarContactos(filename);
        } else if (opcao == "2") {
            listarContactos();
        } else if (opcao == "3") {
            buscarContacto();
        } else if (opcao == "4") {
            editarContacto();
            salvarContactos(filename);
        } else if (opcao == "5") {
            removerContacto();
            salvarContactos(filename);
        } else if (opcao == "6") {
            std::cout << "Saindo...\n";
            break;
        } else {
            std::cout << "Opção inválida, tente novamente.\n";
        }
    }

    return 0;
}
