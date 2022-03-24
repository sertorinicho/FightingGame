#ifndef LOOT_H
#define LOOT_H
#include <string>
#include <sstream>

#define usInt unsigned short int

class Loot
{
private:
    std::string name;       // Nome dell'oggetto
    usInt ID;               // ID dell'oggetto

public:
    Loot(std::string, usInt);                   // Costruttore di Loot
    Loot();                                     // Costruttore a zero parametri
    virtual Loot* clone() const =0;             // Metodo Clone
    virtual ~Loot();                            // Distruttore virtuale di Loot
    virtual void assign(std::stringstream&);    // Funzione di Parsing e Assegnazione da una stringstream
    void set(std::string, usInt);               // Assegnazione dei campi di Loot
    std::string getName() const;                // Ritorna il nome del loot
    usInt getID() const;                        // Ritorna l'ID del loot
    virtual std::string getDesc() const =0;     // Ritorna una overview del loot
    void static normalizeString(std::string&);  // Rimuove tutte le occorrenze del simbolo speciale '_'
};

#endif // LOOT_H
