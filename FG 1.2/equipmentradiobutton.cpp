#include "equipmentradiobutton.h"

EquipmentRadioButton::EquipmentRadioButton(Equipment* eq, QWidget* parent) : QRadioButton(parent), equipment(eq->clone()) {}  // eq->clone per compatibilita' con std::shared_ptr

// Slot che permette l'emissione di un segnale con il proprio Equipment associato
void EquipmentRadioButton::emitEquip() {
    if(isChecked())
        emit clicked(equipment);
}
