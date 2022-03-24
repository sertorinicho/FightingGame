#include "equipment.h"

// Costruttore di Default
Equipment::Equipment() {}

// Costruttore di Equipment
Equipment::Equipment(std::string n, usInt id, usInt val, ty t, usInt dur) :
    Loot(n, id), baseValue(val), type(t), durability(dur) {}

// Metodo clone
Equipment* Equipment::clone() const {
    return new Equipment(*this);
}

// Funzione di parsing e assegnazione da uno stringstream
// La stringstream e' nel formato "name baseValue type durability"
void Equipment::assign(std::stringstream& ss) {
    Loot::assign(ss);   // assign sul sotto-oggetto

    // Parsing proprio
    ss >> baseValue;
    if(!ss.good())
        throw(std::runtime_error(" Parsing failed (base value)"));

    //p
    char t;
    ss >> t;            // estrae il carattere che identifica il tipo
    if(t == 'W')        // tipo weapon
        type = Equipment::weapon;
    else if (t == 'A')  // tipo armor
        type = Equipment::armor;
    else                // carattere non valido
        throw(std::runtime_error(" Parsing failed (type)"));

    //p
    ss >> durability;
    if(ss.bad())
        throw(std::runtime_error(" Parsing failed (durability)"));
}

// Assegna i campi di Equipment
void Equipment::set(usInt bv, ty t, usInt dur) {
    baseValue = bv;
    type = t;
    durability = dur;
}

// Ritorna la baseValue
usInt Equipment::getBaseValue() const {
    return baseValue;
}

// Funizione caratteristica per l'utilizzo dell'Equipment
// Ritorna la baseValue modificata dalla condizione dell' Equipment e possibilmente peggiora quest'ultima
usInt Equipment::getValue() {
    usInt val = baseValue;

    if(durability < 10)
        val = val - 3*(val*(10-durability)/100);
    decreaseDurability();

    return val;
}

// Ritorna la durability
unsigned short Equipment::getDurability() const
{
    return durability;
}

// Aumenta la durability
void Equipment::increaseDurability(usInt amount)
{
    durability += amount;
}

// Peggiora la condizione dell'Equipment
void Equipment::decreaseDurability() {
    if(rand()% 10 > 7 && durability > 0)
        durability--;
}

// Ritorna type
Equipment::ty Equipment::getType() const {
    return type;
}

// Restituisce una overview dell' Equipment
std::string Equipment::getDesc() const {
    std::string eqDesc;
    bool isWeapon = true;
    if(type == Equipment::armor)
        isWeapon = false;    //

    eqDesc += (isWeapon ? "Weapon: " : "Armor:");
    eqDesc += std::to_string(baseValue);
    eqDesc += (isWeapon ? " Damage" : "Defense ");

    return eqDesc;
}
