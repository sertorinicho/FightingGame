#include "character.h"

// Costruttore di Character
Character::Character(std::string n, usInt lvl, usInt h, usInt att, usInt def, Equipment w, Equipment ar) :
    name(n), level(lvl), maxHealth(h), currentHealth(h), attack(att), currentAttack(att), defense(def), currentDefense(def), weapon(new Equipment(w)), armor(new Equipment(ar)) {}

// Costruttore di copia
Character::Character(const Character& c) : name(c.name), level(c.level), maxHealth(c.maxHealth), currentHealth(c.currentHealth),
    attack(c.attack), currentAttack(c.currentAttack), defense(c.defense), currentDefense(c.currentDefense),
    weapon(new Equipment (*(c.weapon)) ), armor(new Equipment (*(c.armor)) ), inventory(c.inventory) {}


// Operatore di assegnazione
Character& Character::operator=(const Character& c) {
    if(this != &c)
    {
        delete weapon;
        delete armor;
        name = c.name;
        level = c.level;
        maxHealth = c.maxHealth;
        currentHealth = c.currentHealth;
        attack = c.attack;
        currentAttack = c.currentAttack;
        defense = c.defense;
        currentDefense = c.currentDefense;
        weapon = new Equipment (*(c.weapon));
        armor = new Equipment (*(c.armor));
        inventory = c.inventory;
    }
    return *this;
}

// Distruttore
Character::~Character() {
    if(!dynamic_cast<Item*>(weapon))
        delete weapon;
    delete armor;
}

// Rappresenta quanto il danno e' attutito
usInt Character::damageReduction() const {
    return currentDefense + armor->getValue();
}

// Modifica la vita corrente, con valore minimo 0
void Character::changeCHealth(short int val) {
    if(val < 0 && abs(val) > currentHealth)
        currentHealth = 0;
    else if( val > 0 && currentHealth + val > maxHealth)
        currentHealth = maxHealth;
    else
        currentHealth = currentHealth + val;
}

// Modifica l'attacco corrente, con valore minimo 1
void Character::changeCAttack(short int val) {
    if(val < 0 && abs(val) >= currentAttack)
        currentAttack = 1;
    else
        currentAttack = currentAttack + val;
}

// Modifica la difesa corrente, con valore minimo 1
void Character::changeCDefense(short int val) {
    if(val < 0 && abs(val) >= currentDefense)
        currentDefense = 1;
    else
        currentDefense = currentDefense + val;
}

// Modifica la vita massima
void Character::lvlUpHealth(usInt val) {
    maxHealth = maxHealth + val;
}

// Modifica l'attacco massimo
void Character::lvlUpAttack(usInt val) {
    attack = attack + val;
}

// Modifica la difesa massima
void Character::lvlUpDefense(usInt val) {
    defense = defense + val;
}

// Calcola ed applica il danno ad un personaggio
// Mantiene il danno minimo a 100
// Restituisce il danno calcolato come dmg = dmg +/- 25% di dmg o dmg = dmg*2 (colpo critico)
std::string Character::makeAttack(Character* e) const {
    usInt dmg = currentAttack + weapon->getValue();
    usInt dmgReduction = e->damageReduction() > 100 ? e->damageReduction(): 100;
    std::string crit;

    if(dmg <= dmgReduction || dmg-dmgReduction < 100)
        dmg = 100;
    else
        dmg = dmg - dmgReduction;

    if(rand()%100 < 90)
        dmg = dmg+((rand()%dmg/2)-dmg/4);
    else {
        dmg = dmg*2;
        crit = "<CRIT> ";
    }
    e->changeCHealth(-dmg);
    return crit + name + " deals " + std::to_string(dmg) + " damage!";
}

// Utilizza un item e applica l'effetto al personaggio target
std::string Character::use(Item* i, Character* e) {
    Item::Effect eff = i->effect();
    std::string effect_txt = name + " uses " + i->getName() + ": ";

    // Trova il personaggio target
    Character* target = this;
    if(!eff.isSelf()) {
        if(e) {
            target = e;
            effect_txt += e->getName() + ":";
        }
        else
            throw(std::logic_error("Error: missing target for Item effect"));
    }
    std::string sign;
    // Applica l'effetto
    if(short healthEff = eff.getHealthEff()) {
        target->changeCHealth(healthEff);
        sign = (healthEff > 0 ? "+" : "-");
        effect_txt += " Health " + sign + std::to_string(abs(healthEff));
    }

    if(short attackEff = eff.getAttackEff()) {
        target->changeCAttack(attackEff);
        sign = (attackEff > 0 ? "+" : "-");
        effect_txt += " Attack " + sign + std::to_string(abs(attackEff));
    }

    if(short defenseEff = eff.getDefenseEff()) {
        target->changeCDefense(defenseEff);
        sign = (defenseEff > 0 ? "+" : "-");
        effect_txt += " Defense " + sign + std::to_string(abs(defenseEff));
    }

    return effect_txt;
}

// Aggiunge all'inventario un tipo di item e ritorna i nomi degli Items che hanno "fatto powerUp"
std::string Character::addToInventory(SList<std::shared_ptr<Item>>& l) {
    std::string pUpItems = "";
    std::shared_ptr<Item> tmp;
    for(SList<std::shared_ptr<Item>>::iterator listIt = l.begin(); listIt != l.end(); listIt++) {   // Per ogni elemento da aggiungere
        tmp = *listIt;   // Estrae l'elemento dalla lista l
        bool found = false;
        for(SList<std::shared_ptr<Item>>::iterator invIt = inventory.begin(); invIt != inventory.end() && !found; invIt++) {    // Per ogni elemento dell'inventario
            if(tmp->getID() == (*invIt)->getID()) { // Controlla se esiste un Item nell' inventario con ID uguale all' Item da aggiungere
                found = true;   // Trovato un item nell' inventario con ID uguale
                (*invIt)->powerUp();    // Chiamata POLIMORFA, potenzia l'Item
                pUpItems += (*invIt)->getName() + ", ";
            }
        }
        if(!found) // Se non esiste un elemento con lo stesso ID, aggiunge all'inventario
            inventory.push_back(tmp);
    }

    if(pUpItems.size()>0) { // Sistema la stringa
        pUpItems.pop_back();
        pUpItems.pop_back();
    }
    return pUpItems;
}

// Cambia weapon o armor con eq
void Character::swapEquipment(Equipment* eq) {
    // Verifica tipo di eq e assegna corrispondentemente
    if(eq->getType() == Equipment::weapon) {
        if(!dynamic_cast<Item*>(weapon))    // Se e' una Staff non va eliminato, perche' presente nell'inventario
            delete weapon;

        weapon = eq;                        // Riassegna la weapon

        if(Item* staff = dynamic_cast<Item*>(eq))
            inventory.push_back(std::shared_ptr<Item>(staff));  // Aggiunge all'inventario eq se e' una Staff
    }

    else {
        delete armor;
        armor = eq; // Riassegna la armor
    }
}

// Controlla se health == 0
bool Character::isUnconscious() const {
    return currentHealth == 0;
}

// return true iff currentHealth <= 40% of maxHealth
bool Character::isInjured() const {
    return maxHealth * 40  > currentHealth * 100;
}

// Restituisce il miglore oggetto per la cura presente nell'inventario
Item* Character::bestHealingIt() const {
    Item* maxHI = nullptr;
    short max = 0;

    for(SList<std::shared_ptr<Item>>::const_iterator cit = inventory.begin(); cit != inventory.end(); cit++)    // Per ogni elemento dell'inventario
    {
        if((*cit)->isUsable() && (*cit)->getEffect().isSelf() && (*cit)->getEffect().getHealthEff() > max) { // Se l'item in (*it) e' utilizzabile, ha effetto su self e l'effetto di cura e' maggiore di max
            maxHI = (*cit).get();                       // Salva il puntatore al nuovo Item
            max = (*cit)->getEffect().getHealthEff();   // Salva il valore del healthEffect
        }
    }
    return maxHI;
}

// Ritorna il livello del Character
usInt Character::getLevel() const {
    return level;
}

// Aumenta le statische e resetta quelle correnti
void Character::levelUp() {
    maxHealth = maxHealth + 1000;
    attack = attack + 150;
    defense = defense + 200;

    // Reset
    currentHealth = maxHealth;
    currentAttack = attack;
    currentDefense = defense;
}

// Ritorna il nome del Character
std::string Character::getName() const {
    return name;
}

// Ritorna gli HP correnti del Character
usInt Character::getCurrentHealth() const {
    return currentHealth;
}

// Ritorna per riferimento costante l' inventario del Character
const SList<std::shared_ptr<Item>>& Character::getInventory() const {
    return inventory;
}

// Modifica il nome del Character
void Character::changeName(std::string txt) {
    if(txt != "")
        name = txt;
}
