#include "controller.h"

// Costruttore di Controller
Controller::Controller(Model* m, QObject *parent) : QObject(parent), model(m) {}

// Imposta la view
void Controller::setView(FightingGame* v) { view = v;}

// Ritorna il Player name
QString Controller::getPlayerName() const {
    return model->getPlayerName().c_str();
}

// Ritorna l' Enemy name
QString Controller::getEnemyName() const {
    return model->getEnemyName().c_str();
}

// Ritorna la currentHealth del Player
usInt Controller::getCurrentHealth() const {
    return model->getCurrentHealth();
}

//Ritorna l' Enemy currentHealth
usInt Controller::getEnemyCurrentHealth() const {
    return model->getEnemyCurrentHealth();
}

// Ritorna il winCount
usInt Controller::getWinCount() const {
    return model->getWinCount();
}

// Ritorna l'inventario del Player per riferimento costante
const SList<std::shared_ptr<Item>>& Controller::getPlayerInventory() const {
    return model->getPlayerInventory();
}

// Chiamata specifica dell'attacco del giocatore
void Controller::playerAttack() {
    turn(Model::attack);
}

// Chiamata specifica dell'uso dell'Item del giocatore
void Controller::playerUseItem(Item* item) {
    turn(Model::useItem, item);
    view->updateItemButton(item);
}

// Gestisce il turon di gioco e aggiorna la view
void Controller::turn(Model::PAction pa, Item* item) const try {
    std::string turn_txt = model->turn(pa, item);
    view->updateHP();
    view->showMessage(QString(turn_txt.c_str()));
    if(model->isOver()) {
        if(model->PWon() && model->getWinCount() == 2)
            view->youWon();
        else if(model->PWon())
            view->rewardPhase(model->generateEqRewards(), model->generateItRewards());
        else
            view->youLost();
    }
}
catch(std::runtime_error e) {
    view->errorDialog(e);   // Catch per exceptions durante il turno
}

// Modifica il nome del Player
void Controller::changePlayerName(std::string txt) {
    model->changePlayerName(txt);
}

//  Resetta la view ed il model
void Controller::resetGame() {
    model->reset();
    view->reset();
}

//  Update model e view per la prossima battaglia
void Controller::prepareNextBattle(Equipment* equipment) {
    model->swapEquipment(equipment);
    view->updateInventory(model->getPlayerInventory());
    model->levelUp();
    model->createEnemy();
    view->updateGame();
}
