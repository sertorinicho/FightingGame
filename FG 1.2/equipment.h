#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "loot.h"
#include <sstream>
#include <ctime>

class Equipment : virtual public Loot { //Derivazione Virtuale
public:
    enum ty {weapon, armor};    // Distingue armi e armature

private:
    usInt baseValue;            // Valore intrinseco dell'oggetto (determina danno/resistenza ai danni)
    ty type;                    // Determina il tipo di equipment
    usInt durability;           // Valore che rappresenta la condizione dell'equipment

public:
    Equipment();                                            // Costruttore a zero parametri
    Equipment(std::string, usInt, usInt, ty, usInt =10);    // Costruttore di Equipment
    virtual Equipment* clone() const;                       // Metodo Clone
    virtual void assign(std::stringstream&);                // Funzione di Parsing e Assegnazione da una stringstream
    void set(usInt, ty, usInt);                 // Assegna i campi di Equipment
    usInt getBaseValue() const;                 // Ritorna la base value
    usInt getValue();                           // Funzione caratteristica per l'utilizzo dell'equipment
    usInt getDurability() const;                // Ritorna la durability
    void increaseDurability(usInt);             // Aumenta la durability
    void decreaseDurability();                  // Peggiora la condizione dell'equipment
    ty getType() const;                         // Ritorna il tipo
    virtual std::string getDesc() const;        // Restituisce una overview dell'equipment
};

#endif // EQUIPMENT_H
