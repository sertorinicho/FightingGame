#ifndef FIGHTINGGAME_H
#define FIGHTINGGAME_H

#include <QWidget>
#include <QPushButton>
#include <QIcon>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QFile>
#include <QTextStream>
#include <QLabel>
#include <QDialog>
#include <QDialogButtonBox>
#include <QApplication>
#include <QProgressBar>
#include <QStatusBar>
#include <QGridLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QGroupBox>
#include <QButtonGroup>

#include <memory>
#include <stdexcept>

#include "staff.h"
#include "itembutton.h"
#include "equipmentradiobutton.h"
#include "SList.h"

class Controller;

class FightingGame : public QWidget
{
    Q_OBJECT

public:
    FightingGame(Controller*, QWidget *parent = nullptr);               // Costruttore di FightingGame
    ~FightingGame();                        // Distruttore di FightingGame

    void updateHP();                        // Aggiorna le barre della vita di Giocatore e Avversario
    void showMessage(QString);              // Mostra l'esito del turno
    void youWon();                          // Dialog che notifica il termine della partita
    void youLost();                         // Dialog che notifica il termine della partita
    void rewardPhase(SList<std::shared_ptr<Equipment>>, std::string);   // Dialog che implementa la fase di Reward dopo una vittoria
    void updateGame();                      // Aggiorna la vista dopo una vittoria
    void updateInventory(const SList<std::shared_ptr<Item>>&);          // Aggiorna (e aggiunge) i bottoni degli item nell'inventario dopo una vittoria
    void updateItemButton(Item*);           // aggiorna il bottone dell'item utilizzato dal player
    void errorDialog(std::runtime_error);   // Dialog di notifica di errore a runtime
    void reset();                           // Reset del gioco

public slots:
    void setMainMenu();         // Costruisce il menu principale del gioco, o lo imposta come currentWidget
    void setGame();             // Costruisce la schermata di gioco per il combattimento, o la imposta come currentWidget
    void setInfo();             // Costruisce la schermata di informazioni sul gioco, o la imposta come currentWidget
    void confirmQuit();         // Dialog per la conferma della chiusura dell'applicazione
    void changePlayerName();    // Costruisce il widget per l'inserimento del nome del Giocatore, o lo imposta come currentWidget
    void savePlayerName();      // Modifica il nome del Giocatore

private:
    Controller* controller;     // Puntatore a Controller
    QLayout* mainLayout;        // Puntatore a MainLayout
    QStackedWidget* stack;      // StackedWidget per tutte le schermate di FightingGame

    // Puntatori ai Widget nello stack di FightingGame
    QWidget* mainMenu;
    QWidget* info;
    QWidget* changeName;
    QWidget* game;

    static void clearLayout(QLayout*);          // Elimina il Layout e tutti i suoi elementi
    QGroupBox* createEnemyWidget();             // Crea e restituisce il widget dell'enemy
    void setEnemyWidget(QWidget*);              // Aggiorna il widget dell'enemy dopo una vittoria
    QScrollArea* createScrollAreaInventory();   // Crea la scrollArea contenente l'inventario
    static QDialog* txtDialog(QString, QString, QLayout*);  // Template per la creazione parziale di Dialog con testo
    QVBoxLayout* createEqRewBox(SList<std::shared_ptr<Equipment>>&) const;  // Funzione ausiliaria a rewardPhase


private slots:
    void setBackInv();      // Imposta il currentWidget dello stack del giocatore al widget degli action buttons
    void setFWDInv();       // Imposta il currentWidget dello stack del giocatore al widget dell'inventario
};
#endif // FIGHTINGGAME_H
