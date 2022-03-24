#ifndef CHARACTER_H
#define CHARACTER_H

#include <memory>
#include "staff.h"
#include "consumable.h"
#include "SList.h"

class Character
{    
public:
    enum statsUp {health, att, def};   // Statistiche livellabili del personaggio

private:
    std::string name;       // Nome del personaggio
    usInt level;            // Livello del personaggio
    usInt maxHealth;        // Punti vita massimi
    usInt currentHealth;    // Punti vita correnti
    usInt attack;           // Modificatore base del danno
    usInt currentAttack;    // Modificatore corrente del danno
    usInt defense;          // Modificatore della difesa
    usInt currentDefense;   // Modificatore corrente della difesa

    Equipment* weapon;                      // Arma principale
    Equipment* armor;                       // Armatura
    SList<std::shared_ptr<Item>> inventory; // Inventario, container di tutti gli item posseduti

    usInt damageReduction() const;  // Interazione con l'armatura
    void changeCHealth(short int);  // Modifica la vita corrente
    void changeCAttack(short int);  // modifica l'attacco corrente
    void changeCDefense(short int); // Modifica la difesa corrente
    void lvlUpHealth(usInt);        // Modifica la vita massima
    void lvlUpAttack(usInt);        // Modifica l'attacco massimo
    void lvlUpDefense(usInt);       // Modifica la difesa massima


public:
    Character(std::string, usInt, usInt, usInt, usInt, Equipment, Equipment);   // Costruttore di Character
    Character(const Character&);                                                // Costruttore di Copia
    Character& operator=(const Character&);                                     // Operatore di Assegnazione
    ~Character();                                                               // Distruttore
    std::string makeAttack(Character*) const;                                   // Calcola ed applica il danno ad un personaggio
    std::string use(Item*, Character* =nullptr);                                // Utilizza un item e applica l'effetto al personaggio target
    std::string addToInventory(SList<std::shared_ptr<Item>>&);                  // Aggiunge all'inventario un tipo item e ritorna i nomi degli Items che hanno "fatto powerUp"
    void swapEquipment(Equipment*);                                             // Cambia weapon o armor
    bool isUnconscious() const;                                                 // Controlla se health == 0
    bool isInjured() const;                                                     // Controlla se il personaggio ha meno di 40% della vita
    Item* bestHealingIt() const;                                                // Restituisce il migliore oggetto per la cura presente nell'inventario
    usInt getLevel() const;                                                     // Ritorna il livello del Character
    void levelUp();                                                             // Aumenta le statistiche e resetta quelle correnti
    std::string getName() const;                                                // Ritorna il nome del Character
    usInt getCurrentHealth() const;                                             // Restituisce gli HP correnti del Character
    const SList<std::shared_ptr<Item>>& getInventory() const;                   // Ritorna per riferimento costante l'inventario del Character
    void changeName(std::string);                                               // Modifica il nome del Character
};

#endif // CHARACTER_H
