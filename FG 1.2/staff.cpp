#include "staff.h"

// Costruttore di default
Staff::Staff() {}

// Costruttore di Staff
Staff::Staff(std::string n, usInt id, std::string desc, usInt val, usInt dur, short h, short att, short def, bool s) :
    Loot(n, id), Item(n, id, desc, h, att, def, s), Equipment(n, id, val, Equipment::weapon, dur) {}

// Metodo clone
Staff* Staff::clone() const {
    return new Staff(*this);
}

// Funzione di parsing e assegnazione da uno stringstream
// La stringa e' nel formato "name id description E(health,att,def,self) baseValue durability
// Ogni campo e' diviso da uno spazio
// description rappresenta la descrizione, ed e' una stringa di parole separate da underscore
// E(health,att,def,self) rappresenta l'effetto, identificato da:
//      E maiuscola seguito da aperta parentesi
//      usInt per l' healthEff, seguito da una virgola
//      usInt per l' attackEff, seguito da una virgola
//      usInt per l' defenseEff,seguito da una virgola
//      un carattere per identificare il booleano self, con 'S' = true e 'N' = false
//      chiusa parentesi
void Staff::assign(std::stringstream& ss) {
    Item::assign(ss);   // assign sul sotto-oggetto Item

    // parsing parte di Equipment
    usInt baseValue;
    ss >> baseValue;
    if(!ss.good())
        throw(std::runtime_error(" Parsing failed (base value)"));

    usInt durability;
    ss >> durability;
    if(ss.bad())
        throw(std::runtime_error(" Parsing failed (durability)"));

    Equipment::set(baseValue, Equipment::weapon, durability);
}

// Restituisce una overview dell'item
std::string Staff::getDesc() const {
    return Equipment::getDesc() + '\n' + "Activate:" + Item::getDesc();
}

// Effettua l'azione caratteristica dell'oggetto
Item::Effect Staff::effect() {
    decreaseDurability();
    return Item::effect();
}

// Ritorna il numero di utilizzi rimasti (con il significato che ha nella sottoclasse)
usInt Staff::getRemainingUses() const {   
    return getDurability();
}

void Staff::powerUp() {
    Item::powerUp();
    increaseDurability(2);
}

// Ritorna true iff getDurability() > 0
bool Staff::isUsable() const
{
    return getDurability() > 0;
}

