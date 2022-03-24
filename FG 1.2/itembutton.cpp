#include "itembutton.h"

// Costruttore ItemButton
ItemButton::ItemButton(Item* it,  QWidget* parent) : QPushButton(parent), item(it) {
    connect(this, SIGNAL(clicked()), this, SLOT(emitter()));
    setText(item->getName().c_str());
    setToolTip(item->getDesc().c_str());
    setStyleSheet("background-color:DarkCyan");
}

// Slot che permette l'emissione di un segnale con il proprio Item associato
void ItemButton::emitter() {
    emit clicked(item);
}

// Ripristina le proprieta' dell' ItemButton
void ItemButton::tryToEnable() {
    setToolTip(item->getDesc().c_str());    // Possible PowerUp
    if(!isEnabled() && item->isUsable()) {
        setEnabled(true);
        setStyleSheet("background-color:DarkCyan");
    }
}

// Ritorna l'Item associato
const Item* ItemButton::getItem() const {
    return item;
}

// Aggiorna le proprieta' dell' ItemButton
void ItemButton::update()
{
    setToolTip(item->getDesc().c_str());
    if(!item->isUsable()) {
        setEnabled(false);
        setStyleSheet("background-color:gray");
    }
}
