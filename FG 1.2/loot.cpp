#include "loot.h"

// Costruttone di Loot
Loot::Loot(std::string n, usInt id) :
    name(n), ID(id) {}

// Costruttore di default di Loot
Loot::Loot() {}

// Distruttore virtuale
Loot::~Loot() {}

// Funzione di parsing e assegnazione da uno stringstream
void Loot::assign(std::stringstream& ss) {
    ss >> name;
    if(!ss.good())
        throw(std::runtime_error(" Parsing failed (name)"));
    normalizeString(name);

    ss >> ID;
    if(!ss.good())
        throw(std::runtime_error(" Parsing failed (ID)"));
}

// Assegna i campi di Loot
void Loot::set(std::string n, usInt id) {
    name = n;
    ID = id;
}

// Ritorna il name
std::string Loot::getName() const {
    return name;
}

// ritorna l' ID
usInt Loot::getID() const {
    return ID;
}

// Rimuove tutte le occorrenze del simbolo speciale '_'
void Loot::normalizeString(std::string& s) {
    std::size_t pos = s.find('_');

    while(pos != std::string::npos) {
        s.replace(pos, 1, " ");
        pos = s.find('_', pos+1);
    }
}
