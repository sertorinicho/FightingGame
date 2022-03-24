#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "fightinggame.h"
#include "model.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(Model* m, QObject *parent = nullptr);   // Costruttore di Controller
    void setView(FightingGame*);                                // Funzione che imposta la view collegata al controller

    QString getPlayerName() const;          // Ritorna il nome del giocatore
    QString getEnemyName() const;           // Ritorna il nome dell'avversario
    usInt getCurrentHealth() const;         // Ritorna gli HP correnti del giocatore
    usInt getEnemyCurrentHealth() const;    // Ritorna gli HP correnti dell'avversario
    usInt getWinCount() const;              // Ritorna il numero di vittorie del giocatore
    const SList<std::shared_ptr<Item>>& getPlayerInventory() const; // Ritorna l'inventario del giocatore per riferimento costante
    void changePlayerName(std::string);     // Cambia il nome al giocatore

public slots:
    void playerAttack();                // Chiamata specifica dell'attacco del giocatore
    void playerUseItem(Item*);          // Chiamata specifica dell'uso di un item da parte del giocatore
    void resetGame() ;                  // Reset di Vista e Modello
    void prepareNextBattle(Equipment*); // Update di Modello e Vista per la prossima battaglia

signals:

private:
    FightingGame* view;     // Vista
    Model* model;           // Modello

    void turn(Model::PAction, Item* =0) const;  // Gestisce il turno di gioco e aggiorna la Vista

};

#endif // CONTROLLER_H
