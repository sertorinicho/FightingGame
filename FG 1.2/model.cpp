#include "model.h"
#include <QString>
#include <QFile>
#include <QTextStream>

// Costruttore del Model
Model::Model() : Player(nullptr), enemy(nullptr) {
    reset();
}

// Costruttore di copia del Model
Model::Model(const Model& m) :
    Player(new Character( *(m.Player) )), enemy(new Character( *(m.enemy) )) {}

// Assegnazione
Model& Model::operator=(const Model& m) {
    if(this != &m) {
        delete m.Player;
        delete m.enemy;
        Player = new Character( *(m.Player) );
        enemy = new Character( *(m.enemy) );
    }
    return *this;
}

// Distruttore di Model
Model::~Model() {
    delete Player;
    delete enemy;
}

// Resetta il Model
void Model::reset() {
    // Crea il Player
    if(Player)
        delete Player;  // Delete su nullptr no-op
    Equipment startingSword("Commoner's Sword", 100, 400, Equipment::weapon, 10);
    Equipment startingArmor("Peasant's Clothes", 150, 400, Equipment::armor, 10);
    Player = new Character("Player", 1, 1000, 400, 300, startingSword, startingArmor);
    SList<std::shared_ptr<Item>> startingItems;
    startingItems.push_back(std::make_shared<Consumable>(Consumable("Potion of Healing", 404, "Heals some of your HP", 3, 300, 0, 0, true)));
    startingItems.push_back(std::make_shared<Consumable>(Consumable("Throwing Darts", 403, "Throwable deadly knives", 3, -200, 0, 0, false)));
    Player->addToInventory(startingItems);

    // Crea l'enemy
    if(enemy)
        delete enemy;   // Delete su nullptr no-op
    Equipment sSword("Commoner's Sword", 100, 400, Equipment::weapon);
    Equipment sArmor("Peasant's Clothes", 150, 400, Equipment::armor);
    enemy = new Character("Angry Peasant", 1, 1000, 350, 300, sSword, sArmor);
    startingItems.clear();
    startingItems.push_back(std::make_shared<Consumable>(Consumable("Poor Man Soup", 404, "Does not tase good", 1, 200, 15, 15, true)));
    enemy->addToInventory(startingItems);

    // Reset winCount
    winCount = 0;
}

// Un personaggio ha raggiunto 0 health
bool Model::isOver() const {
    return Player->isUnconscious() || enemy->isUnconscious();
}

// true iff il Player ha vinto
bool Model::PWon() const {
    return enemy->isUnconscious();
}

// Azione effettuata dal Player
std::string Model::playerAction(PAction pa, Item* it) const {
    if(pa == PAction::attack)
        return Player->makeAttack(enemy);
    else
        return Player->use(it, enemy);
}


// Azione effettuata dall' enemy
std::string Model::enemyAction() const {
    std::string enemyAction_txt;
    bool done = false;
    if(enemy->isInjured())                                      // Se e' ferito
        if(rand()%10 > 5 || enemy->getName() == "The Monarch") {    // 40% di probabilita' di cercare un Item di cura (o boss battle)
            if(Item* bestHI = enemy->bestHealingIt()){                  // Se e' presente ed utilizzabile un Item di cura nell' inventario
                enemyAction_txt = enemy->use(bestHI);                       // Utilizza l'Item di cura
                done = true;
            }
        }
    if(!done)                                       // Se non ha utilizzato un Item
        enemyAction_txt = enemy->makeAttack(Player);    // Effettua un attacco

    return  enemyAction_txt;
}

// Gestore del turon di combattimento
std::string Model::turn(PAction pa, Item* item) const {
    std::string turn_txt;
    turn_txt = playerAction(pa, item) + "\n";   // Azione del Player

    if(enemy->isUnconscious())                      // Se il Player ha vinto
        turn_txt += enemy->getName() + " is unconscious!";  // Segnala vittoria
    else {
        turn_txt += enemyAction();      // Azione dell' enemy
        if(Player->isUnconscious())         // Se l'enemy ha vinto
            turn_txt += " you fainted!";        // Segnala sconfitta
    }
    return turn_txt;
}

// Effettua il level up del Player
void Model::levelUp() {
    Player->levelUp();
    winCount++;
}

// Genera la ricompensa di tipo Equipment
// I file Equipment.txt e Staves.txt sono organizzati nel seguente modo:
// nella prima riga e' presente un intero che indica quanti elementi sono presenti nel file
// ogni riga successiva rappresenta un elemento
SList<std::shared_ptr<Equipment>> Model::generateEqRewards() const {
    SList<std::shared_ptr<Loot>> sRew; // contenitore Staff rewards
    SList<std::shared_ptr<Loot>> eRew; // contenitore Equipment rewards

    usInt rewC = 3;   // conta quante rewards da genere

    // 10% di possibilita' che una reward sia uno staff
    if (rand()%100 >= 90) {
        generateRewards(sRew, "Staves.txt", 1, 1, std::make_shared<Staff>(Staff()));    // Genera gli Staff
        rewC--; // Aggiorna il numero di rewards da generare
    }

    generateRewards(eRew, "Equipment.txt", 3, rewC, std::make_shared<Equipment>(Equipment()));  // Genera gli Equipment

    // Unisce le due liste
    SList<std::shared_ptr<Equipment>> SERew;
    for(SList<std::shared_ptr<Loot>>::iterator it = sRew.begin(); it != sRew.end(); it++)
        SERew.push_back( std::dynamic_pointer_cast<Equipment>(*it) );
    sRew.clear();
    for(SList<std::shared_ptr<Loot>>::iterator it = eRew.begin(); it != eRew.end(); it++)
        SERew.push_back( std::dynamic_pointer_cast<Equipment>(*it) );
    eRew.clear();

  return SERew;  // ritorna la lista con le rewards
}

// Genera le rewards
void Model::generateRewards(SList<std::shared_ptr<Loot>>& rew, std::string f, usInt minEl, usInt nLines, std::shared_ptr<Loot> loot) {
    // Crea una lista di rewards con tipo dinamico uguale al tipo dinamico di loot
    rew.push_back(loot);
    for(int i = 0; i < nLines-1; i++)
        rew.push_back(std::shared_ptr<Loot>(loot->clone()));

    std::string fp = ":/Resources/Loot/" + f;  // file path per gli Equipment da generare
    QFile qf(fp.c_str());  // costruisce un nuovo oggetto QFile a partire dal file indicato
    if(!qf.open(QFile::ReadOnly))  // apre il file in formato ReadOnly
        throw (std::runtime_error("File not found"));
    QTextStream l(&qf);    // crea uno QTextStream che opera sul QFile qfs
    QString qs = l.readLine();  // legge la prima riga
    bool* ok = new bool;
    usInt numEl = qs.toInt(ok);   // verifica che sia un intero
    if(!ok) {
        std::string err = " Format Error: invalid first line";
        throw (std::runtime_error(err.c_str()));
    }
    delete ok;
    if(numEl < minEl) { // Verifica numero minimo elementi
        std::string err = " Format Error: not enough lines ";
        throw (std::runtime_error(err.c_str()));
    }

    std::vector<int> fLine = generateFileLines(nLines, numEl);  // Genera gli interi corrispondenti alle righe da leggere dal file

    try {
        int el = 0; // el-esimo elemento nel file
        int i = 0;  // gli elementi di eqLine da [0..i-1] sono stati assegnati
        for(SList<std::shared_ptr<Loot>>::iterator it = rew.begin(); it != rew.end(); it++) {    // per nLines volte:
            for(; el < fLine[i]; el++) // salta i primi eqLine[i]-1 elementi del file
                qs = l.readLine();
            std::string s = qs.toStdString();   // converte l'eqLine[i]-esimo elemento da QString a std::string 
            std::stringstream ss;
            ss << s;
            (*it)->assign(ss); // genera l'nLines-esimo elemento  POLIMORFISMO
            i++;
        }
    }
    catch(std::runtime_error e){ // nel caso uno degli assign() fallisca, si intercetta l'eccezione con un catch in modo da poter chidere il file e gestire parzialmente l'eccezione
        qf.close();
        std::string error_txt = strcmp(e.what(),"File not found") == 0 ? "File not found" : "Error:\n\t file " + f + e.what();
        throw std::runtime_error(error_txt.c_str());  // rilancia l'eccezione
    } 
    qf.close();    // chiude il file
}

// Genera un vettore composto da nLines interi compresi tra 1 e numEl e li ordina
std::vector<int> Model::generateFileLines(usInt nLines, usInt numEl) {
    std::vector<int> fLine;   // array che contiene interi che rappresentano le righe corrispondenti agli elementi da generare
    bool done = false;
    while(!done) {  // ciclo che genera gli interi di eqLine
        for(int i = 0; i < nLines; i++) { // genera i numeri pseudocasuali per fLine
            fLine.push_back(rand()% numEl + 1);
        }
        done = true;
        for(int i = 0; i < nLines-1 && done; i++) {   //controlla che i valori generati siano diversi tra loro
            for(int j = i+1; j < nLines; j++)
                if(fLine[i] ==  fLine[j]) {
                    fLine.clear();
                    done = false;
                }
        }
    }
    std::sort(fLine.begin(), fLine.end()); // ordina gli elementi di fLine (per effettuare i getLine una sola volta)
    return fLine;
}

// Aggiunge e comunica gli Item ricevuti
std::string Model::generateItRewards() const {
    SList<std::shared_ptr<Loot>> lRew;

    // Calcola il numero di Consumables da generare
    usInt minEl = 5;
    usInt consumablesToGenerate;
    int numEl = rand()%100 +1;
    if (numEl < 40)
        consumablesToGenerate = 1;
    else if(numEl < 65)
        consumablesToGenerate = 2;
    else if(numEl < 85)
        consumablesToGenerate = 3;
    else
        consumablesToGenerate = 4;

    generateRewards(lRew, "Consumables.txt", minEl, consumablesToGenerate, std::make_shared<Consumable>(Consumable())); // Genera i Consumables

    // Converte la lista ad SList<std::shared_ptr<Item>>
    SList<std::shared_ptr<Item>> cRew;
    for(SList<std::shared_ptr<Loot>>::iterator it = lRew.begin(); it != lRew.end(); it++)
        cRew.push_back( std::dynamic_pointer_cast<Item>(*it) );
    lRew.clear();

    // Genera la stringa di Consumables generati
    std::string txt = "You Found: ";
    for(SList<std::shared_ptr<Item>>::iterator it = cRew.begin(); it != cRew.end(); it++)
            txt += (*it)->getName() + ", ";
    txt.pop_back();
    txt.pop_back();

    std::string pUpItems = Player->addToInventory(cRew); // Aggiunge all'inventario i Consumables
    if(!pUpItems.empty())   // Aggiorna la stringa con i Consumables potenziati
        txt = txt + ".\n The following Items have been enhanced: " + pUpItems;

    return txt;
}

// Il Player ha perso
void Model::gameOver() {
    delete Player;
    delete enemy;
    winCount = 0;
}

// Crea l'avversario
void Model::createEnemy() {
    if(winCount == 1) {
        delete enemy;
        Equipment SSword("Steel Sword", 102, 550, Equipment::weapon, 12);
        Equipment SArmor("Sentinel Armor", 151, 550, Equipment::armor, 12);
        SList<std::shared_ptr<Item>> startingItems;
        startingItems.push_back(std::make_shared<Consumable>(Consumable("Big Ruby", 413, "Emanates a relaxing aura", 3, 200, 25, 25, true)));
        startingItems.push_back(std::make_shared<Consumable>(Consumable("Healing Flask", 415, "Knights' favourite liquor", 2, 250, 50, 0, true)));
        enemy = new Character("Knight", 2, 2000, 500, 400, SSword, SArmor);
        enemy->addToInventory(startingItems);
    }
    else if(winCount == 2) {
        delete enemy;
        Equipment KSword("King's Glave", 107, 750, Equipment::weapon, 13);
        Equipment HArmor("Havel's_Armor", 157, 700, Equipment::armor, 15);
        SList<std::shared_ptr<Item>> startingItems;
        startingItems.push_back(std::make_shared<Consumable>(Consumable("King's Wrath", 416, "Bow before your King", 1, 2500, 100, -100, true)));
        enemy = new Character("The Monarch", 3, 7500, 550, 100, KSword, HArmor);
        enemy->addToInventory(startingItems);
    }
}

// Ritorna il nome del Player
std::string Model::getPlayerName() const {
    return Player->getName();
}

// Ritorna il nome dell' enemy
std::string Model::getEnemyName() const {
    return enemy->getName();
}

// Ritorna il winCount
usInt Model::getWinCount() const {
    return winCount;
}

// Ritorna la currentHelath del Player
usInt Model::getCurrentHealth() const {
    return Player->getCurrentHealth();
}

// Ritorna la currentHealth dell' enemy
usInt Model::getEnemyCurrentHealth() const {
    return enemy->getCurrentHealth();
}

// Ritorna l'inventario del Player per riferimento costante
const SList<std::shared_ptr<Item>>& Model::getPlayerInventory() const {
    return Player->getInventory();
}

// Modifica il nome del Player
void Model::changePlayerName(std::string txt) {
    Player->changeName(txt);
}

// Sostituisce l'Equipment del Player
void Model::swapEquipment(Equipment* equipment) {
    Player->swapEquipment(equipment);
}
