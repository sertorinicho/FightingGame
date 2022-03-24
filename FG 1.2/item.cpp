#include "item.h"

// Costruttore di Item
Item::Item(std::string n, usInt id, std::string desc, short int h, short int att, short int def, bool s) :
    Loot(n, id), itemEffect(s, h, att, def), description(desc) {}

// Costruttore di default
Item::Item() {}

// Funzione di parsing e assegnazione da uno stringstream
void Item::assign(std::stringstream& ss) {
    Loot::assign(ss);   // assign sul sotto-oggetto

    // Parsing proprio
    ss >> description;
    if(!ss.good())
        throw(std::runtime_error(" Parsing failed (description)"));
    normalizeString(description);


    // E(health,att,def,self) rappresenta l'effetto, identificato da:
    //      E maiuscola seguito da aperta parentesi
    //      usInt per l' healthEff, seguito da una virgola
    //      usInt per l' attackEff, seguito da una virgola
    //      usInt per l' defenseEff,seguito da una virgola
    //      un carattere per identificare il booleano self, con 'S' = true e 'N' = false
    //      chiusa parentesi
    char c;
    ss >> c;
    if(c != 'E')
        throw(std::runtime_error(" Parsing failed (item effect)"));
    ss >> c;
    if(c != '(')
        throw(std::runtime_error(" Parsing failed (item effect)"));
    ss >> itemEffect.healthEff;
    if(!ss.good())
        throw(std::runtime_error(" Parsing failed (item effect)"));
    ss >> c;
    if(c != ',')
        throw(std::runtime_error(" Parsing failed (item effect)"));
    ss >> itemEffect.attackEff;
    if(!ss.good())
        throw(std::runtime_error(" Parsing failed (item effect)"));
    ss >> c;
    if(c != ',')
        throw(std::runtime_error(" Parsing failed (item effect)"));
    ss >> itemEffect.defenseEff;
    if(!ss.good())
        throw(std::runtime_error(" Parsing failed (item effect)"));
    ss >> c;
    if(c != ',')
        throw(std::runtime_error(" Parsing failed (item effect)"));
    ss >> c;
    if(c == 'S')    // controlla che il carattere per self sia 'S' o 'N'
        itemEffect.self = true;
    else if (c == 'N')
        itemEffect.self = false;
    else            // carattere per self non valido
        throw(std::runtime_error(" Parsing failed (item effect)"));
    ss >> c;
    if(c != ')')
        throw(std::runtime_error(" Parsing failed (item effect)"));    // controlla che ci sia il carattere ')'
}

// Assegna i campi di Item
void Item::set(bool s, short h, short a, short d, std::string desc) {
    itemEffect.self = s;
    itemEffect.healthEff = h;
    itemEffect.attackEff = a;
    itemEffect.defenseEff = d;
    description = desc;
}

// Funizione caratteristica per l'utilizzo dell'Item
// Restituisce l'effetto
Item::Effect Item::effect() {
   return itemEffect;
}

// Costruttore di default di Effect
Item::Effect::Effect() {}

// Costruttore di Effect
Item::Effect::Effect(bool s, short int h, short int a, short int d) :
    self(s), healthEff(h), attackEff(a), defenseEff(d) {}

// Costruttore di copia di Effect
Item::Effect::Effect(const Effect& e) :
    self(e.self), healthEff(e.healthEff), attackEff(e.attackEff), defenseEff(e.defenseEff) {}

// Potenziamento dell' Item
void Item::powerUp() {
    itemEffect.attackEff = itemEffect.attackEff  + (itemEffect.attackEff * 10 / 100);
    itemEffect.defenseEff = itemEffect.defenseEff + (itemEffect.defenseEff * 10 / 100);
    itemEffect.healthEff = itemEffect.healthEff + (itemEffect.healthEff * 10 / 100);
}

// Ritorna true se il target e' "se stessi"
bool Item::Effect::isSelf() const {
    return self == true;
}

// Ritorna il modificatore sulla "salute"
short int Item::Effect::getHealthEff() const {
    return healthEff;
}

// Ritorna il modificatore sull' "attacco"
short int Item::Effect::getAttackEff() const {
    return attackEff;
}

// Ritorna il modificatore sulla "difesa"
short int Item::Effect::getDefenseEff() const {
    return defenseEff;
}

// Ritorna itemEffect
Item::Effect Item::getEffect() const {
    return itemEffect;
}

// Restituisce una overview dell'item
std::string Item::getDesc() const {
    std::string itemDesc = description;
    std::string sign;
    bool newLine = false;

    if(itemEffect.healthEff != 0) {
        sign = (itemEffect.healthEff > 0 ? "+" : "-");
        itemDesc += "\nHP " + sign + std::to_string(abs(itemEffect.healthEff));
        newLine = true;
    }

    if(itemEffect.attackEff != 0) {
        if(!newLine) {
            itemDesc += "\n"; newLine = true;
        }
        sign = (itemEffect.attackEff > 0 ? "+" : "-");
        itemDesc += " Attack " + sign + std::to_string(abs(itemEffect.attackEff));

    }

    if(itemEffect.defenseEff != 0) {
        if(!newLine) {
            itemDesc += "\n"; newLine = true;
        }
        sign = (itemEffect.defenseEff > 0 ? "+" : "-");
        itemDesc += " Defense " + sign + std::to_string(abs(itemEffect.defenseEff));
    }

    return itemDesc;
}
