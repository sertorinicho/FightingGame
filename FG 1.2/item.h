#ifndef ITEM_H
#define ITEM_H
#include <vector>
#include <stdexcept>
#include <sstream>

#include "loot.h"

//
class Item : virtual public Loot // Derivazione virtuale
{
public:
    class Effect {                          // Classe annidata che rappresenta l'effetto di un item
        friend class Item;
    private:
        bool self;                          // Identifica il target dell'effetto (se stessi o l'avversiìario)
        short int healthEff;                // Modificatore sul campo "salute"
        short int attackEff;                // Modificatore sul campo "attacco"
        short int defenseEff;               // Modificatore sul campo "difesa"

        Effect();                           // Costruttore a zero parametri
        Effect(bool, short int, short int, short int); // Costruttore di Item

    public:
        Effect(const Effect&);              // Costruttore di Copia pubblico, rende possibile la costruzione di istanze di Effect al di fuori della classe Item

        bool isSelf() const;                // Ritorna true sse il target è "se stessi"
        short int getHealthEff() const;     // Ritorna il Mod. sulla "salute"
        short int getAttackEff() const;     // Ritorna il Mod. su "attacco"
        short int getDefenseEff() const;    // Ritorna il Mod. su "difesa"
    };

private:
    Effect itemEffect;                      // Contiene gli effetti dell'item
    std::string description;                // Descrizione dell'item

public:
    Item();                                             // Costruttore a zero parametri
    Item(std::string, usInt, std::string, short int =0, short int =0, short int =0, bool =true); // Costruttore di Item
    virtual Item* clone() const =0;                     // Metodo Clone
    virtual void assign(std::stringstream&);            // Parsing e Assegnazione da uno stringstream opportuno
    void set(bool, short, short, short, std::string);   // Assegna i campi di item
    virtual Effect effect();                            // Effettua l'azione caratteristica dell'oggetto
    Effect getEffect() const;                           // Ritorna l'effetto dell'item
    virtual usInt getRemainingUses() const =0;          // Ritorna il numero di utilizzi rimasti (con il significato che ha nella sottoclasse)
    virtual void powerUp();                             // Potenziamento dell'item
    virtual std::string getDesc() const;                // Restituisce una overview dell'item
    virtual bool isUsable() const =0;                   // Controlla se l'item è utilizzabile (con il significato che ha nella sottoclasse)
};

#endif // ITEM_H
