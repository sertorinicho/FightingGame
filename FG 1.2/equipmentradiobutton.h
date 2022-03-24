#ifndef EQUIPMENTRADIOBUTTON_H
#define EQUIPMENTRADIOBUTTON_H

#include <QRadioButton>
#include <memory>
#include "equipment.h"

// Classe derivata da QRadioButton
// Permette di avere un Equipment associato

class EquipmentRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    EquipmentRadioButton(Equipment*, QWidget* =nullptr);

public slots:
    void emitEquip();           // Slot che permette l'emissione di un segnale con il proprio Equipment associato

signals:
    void clicked(Equipment*);   // Segnale che emette l'Equipment associato

private:
    Equipment* equipment;
};

#endif // EQUIPMENTRADIOBUTTON_H
