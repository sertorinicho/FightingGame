#ifndef CONSUMABLE_H
#define CONSUMABLE_H
#include "item.h"


class Consumable : public Item
{
private:
    usInt maxUses;                  // Numero massimo di utilizzi
    usInt remainingUses;            // Numero di utilizzi rimasto
    void consume();                 // Abbassa il numero di utilizzi rimasti

public:
    Consumable();                               // Costruttore di Default a zero parmetri
    Consumable(std::string, usInt, std::string, usInt, short int =0, short int =0, short int =0, bool =true); // Costruttore di Consumable
    virtual Consumable* clone() const;          // Metodo Clone
    virtual void assign(std::stringstream&);    // Funzione di Parsing e Assegnazione da uno stringstream
    void set(usInt);                            // Modifica maxUses e resetta remainingUses
    virtual Effect effect();                    // Funzione caratteristica per l'utilizzo dell'item
    virtual usInt getRemainingUses() const;     // Ritorna il numero di usi rimasti
    virtual void powerUp();                     // Potenziamento del consumable e reset degli utilizzi
    virtual std::string getDesc() const;        // Restituisce una overview dell'oggetto
    virtual bool isUsable() const;              // Controlla se l'oggetto è ancora utilizzabile (ha ancora usi)
};

#endif // CONSUMABLE_H


