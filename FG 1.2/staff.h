#ifndef STAFF_H
#define STAFF_H

#include "item.h"
#include "equipment.h"

class Staff : public Item, public Equipment {       //Multiple Inheritance a Diamante
public:
    Staff();                                    // Costruttore a zero parametri
    Staff(std::string, usInt id, std::string, usInt, usInt =10, short int =0, short int =0, short int =0, bool =true);  // Costruttore di Staff
    virtual Staff* clone() const;               // Metodo Clone
    virtual void assign(std::stringstream&);    // Funzione di Parsing e Assegnazione da una stringstream
    virtual std::string getDesc() const;        // Ritorna una overview della staff
    virtual Effect effect();                    // Effettua l'azione caratteristica dell'oggetto
    virtual usInt getRemainingUses() const;     // Ritorna il numero di utilizzi rimasti (con il significato che ha nella sottoclasse)
    virtual void powerUp();                     // Potenzia la staff
    virtual bool isUsable() const;              // Controlla se la staff è ancora utilizzabile
};

#endif // STAFF_H
