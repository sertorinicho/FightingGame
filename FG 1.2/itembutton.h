#ifndef ITEMBUTTON_H
#define ITEMBUTTON_H

#include <QPushButton>
#include "consumable.h"

// Classe derivata da QPushButton
// Permette di avere un Item associato

class ItemButton : public QPushButton {
    Q_OBJECT
public:
    ItemButton(Item*, QWidget* =nullptr);   // Costruttore di Itembutton
    void tryToEnable();                     // Ripristina le proprietà dell'itemButton
    const Item* getItem() const;            // Ritorna l'item associato
    void update();                          // Aggiorna le proprietà dell'itemButton


signals:
    void clicked(Item*);    // Segnale che emette l'Item associato

private:
    Item* item;

private slots:
    void emitter(); // Slot che permette l'emissione di un segnale con il proprio Item associato
};

#endif // ITEMBUTTON_H
