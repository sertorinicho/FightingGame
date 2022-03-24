#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "character.h"

class Model
{
public:
    enum PAction {attack, useItem};     // Azione effettuata dal Player

private:
    Character* Player;                  // Giocatore
    Character* enemy;                   // Avversario
    usInt winCount;                     // Contatore delle vittorie del Giocatore

    static void generateRewards(SList<std::shared_ptr<Loot>>&, std::string, usInt, usInt, std::shared_ptr<Loot>);   // Genera le rewards
    static std::vector<int> generateFileLines(usInt nLines, usInt numEl);   // Genera un vettore composto da nLines interi compresi tra 1 e numEl e li ordina
    std::string playerAction(PAction, Item*) const;                         // Azione effettuata dal Giocatore
    std::string enemyAction() const;                                        // Azione effettuata dall'Avversario

public:
    Model();                            // Costruttore a zero parametri
    Model(const Model&);                // Costruttore di Copia
    Model& operator=(const Model&);     // Assegnazione di Model
    ~Model();                           // Distruttore di Model
    void reset();                       // Reset del modello
    bool isOver() const;                // Ritorna true sse un personaggio ha raggiunto 0 health
    bool PWon() const;                  // true sse il Giocatore ha vinto la battaglia

    std::string turn(PAction, Item* =0) const;      // Gestore del turno di combattimento
    void levelUp();                                 // Effettua il level up del Player
    SList<std::shared_ptr<Equipment>> generateEqRewards() const;  // Genera la ricompensa di tipo Equipment
    std::string generateItRewards() const;          // Aggiunge e comunica gli Item ricevuti
    void gameOver();                                // Il Player ha perso
    void createEnemy();                             // Crea l'avversario
    std::string getPlayerName() const;              // Ritorna il nome del Giocatore
    std::string getEnemyName() const;               // Ritorna il nome dell'Avversario
    usInt getWinCount() const;                      // Ritorna il numero di vittorie
    usInt getCurrentHealth() const;                 // Ritorna gli HP correnti del Giocatore
    usInt getEnemyCurrentHealth() const;            // Ritorna gli HP correnti dell'Avversario
    const SList<std::shared_ptr<Item>>& getPlayerInventory() const; // Ritorna l'inventario del Giocatore per rifeerimento costante
    void changePlayerName(std::string);             // Modifica il nome del Giocatore
    void swapEquipment(Equipment*);                 // Sostituisce l'equipment del Giocatore
};

#endif // MODEL_H
