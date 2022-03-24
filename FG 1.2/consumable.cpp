#include "consumable.h"

// Costruttore a 0 parametri
Consumable::Consumable() {}

// Costruttore di Consumable
Consumable::Consumable(std::string n, usInt id, std::string desc, usInt u, short int h, short int att, short int def, bool s) :
    Loot(n, id), Item(n, id, desc, h, att, def, s), maxUses(u), remainingUses(u) {}

// Metodo Clone
Consumable* Consumable::clone() const {
    return new Consumable(*this);
}

// Funzione di parsing e assegnazione da uno stringstream
void Consumable::assign(std::stringstream& ss) {
    Item::assign(ss);   // Assign sul sotto-oggetto

    // Parsing proprio
    ss >> maxUses;
    if(ss.bad())
        throw(std::runtime_error(" Parsing failed (max uses)"));
    remainingUses = maxUses;
}

// Modifica maxUses e resetta remainingUses
void Consumable::set(usInt u) {
    remainingUses = maxUses = u;
}

// Diminuisce il numero di utilizzi rimasti
void Consumable::consume() {
    if(remainingUses>0)
        remainingUses--;
    else
        throw(std::domain_error("Error: item " + getName() + " remaing uses < 0"));
}

// Funizione caratteristica per l'utilizzo dell'item
Item::Effect Consumable::effect() {
    consume();              // Diminuisce remainingUses
    return Item::effect();  // Ritorna l'effetto
}

// Ritorna remainingUses
usInt Consumable::getRemainingUses() const {
    return remainingUses;
}

// Potenziamento del Consumable
void Consumable::powerUp() {
    Item::powerUp();            // Effettua il powerUp sul sottooggetto
    remainingUses = ++maxUses;  // Aumenta maxUses e resetta remainingUses
}

// Ritorna overview dell'oggetto
std::string Consumable::getDesc() const {
    return Item::getDesc() + "\nRemaning Uses: " + std::to_string(remainingUses);
}

// Ritorna true if remainingUses > 0
bool Consumable::isUsable() const
{
    return remainingUses > 0;
}
