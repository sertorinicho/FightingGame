#include "fightinggame.h"
#include "controller.h"

#include <iostream>

// Costruttore di FightingGame
FightingGame::FightingGame(Controller* c, QWidget *parent) : QWidget(parent), controller(c), mainLayout(0), mainMenu(0), info(0), changeName(0), game(0)
{
    srand(time(0));
    setWindowTitle("Fighting Game");

    stack = new QStackedWidget;
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(stack);
    setLayout(mainLayout);

    setMainMenu();  // Pagina iniziale

    setStyleSheet("FightingGame { background-color:DimGray}"
                  "QPushButton, QLabel, QLineEdit { color:black}"
                  "QPushButton, QLineEdit { background-color:DarkCyan}"
                  "QScrollArea { background-color:#606060}"
                  "QScrollBar { background-color: #606060}");
}


// Distruttore
FightingGame::~FightingGame() {
    clearLayout(mainLayout);
}

// Aggiorna le barre della vita di Player ed enemy
void FightingGame::updateHP() {
    usInt currentHP;
    // Aggiorna la barra dell'enemy
    QProgressBar* enemyHP = dynamic_cast<QProgressBar*>(game->layout()->itemAt(0)->widget()->layout()->itemAt(1)->layout()->itemAt(1)->widget());
    enemyHP->setValue(currentHP = controller->getEnemyCurrentHealth());     
    if(currentHP < enemyHP->maximum()/4)
        enemyHP->setStyleSheet("QProgressBar{Background-color:DarkGray; border: 1px solid black; border-radius: 2px;}"
                                   "QProgressBar::chunk{background-color:red; margin:0px;}");
    else if(currentHP < enemyHP->maximum()/2)
        enemyHP->setStyleSheet("QProgressBar{Background-color:DarkGray; border: 1px solid black; border-radius: 2px;}"
                                   "QProgressBar::chunk{background-color:gold; margin:0px;}");
    else
        enemyHP->setStyleSheet("QProgressBar{Background-color:DarkGray; border: 1px solid black; border-radius: 2px;}"
                                   "QProgressBar::chunk{background-color:SpringGreen; margin:0px;}");

    // Aggiorna la barra del Player
    QProgressBar* PlayerHP = dynamic_cast<QProgressBar*>(game->layout()->itemAt(1)->layout()->itemAt(1)->widget());
    PlayerHP->setValue(currentHP = controller->getCurrentHealth());
    if(currentHP < PlayerHP->maximum()/4)
        PlayerHP->setStyleSheet("QProgressBar{Background-color:DarkGray; border: 1px solid black; border-radius: 2px;}"
                                   "QProgressBar::chunk{background-color:red; margin:0px;}");
    else if(currentHP < PlayerHP->maximum()/2)
        PlayerHP->setStyleSheet("QProgressBar{Background-color:DarkGray; border: 1px solid black; border-radius: 2px;}"
                                   "QProgressBar::chunk{background-color:gold; margin:0px;}");
    else
        PlayerHP->setStyleSheet("QProgressBar{Background-color:DarkGray; border: 1px solid black; border-radius: 2px;}"
                                   "QProgressBar::chunk{background-color:SpringGreen; margin:0px;}");
}

// Aggiorna la vista dopo una vittoria
void FightingGame::updateGame() {
    setEnemyWidget(game->layout()->itemAt(0)->widget());
    // Aggiorna la barra del Player
    QProgressBar* PlayerHP = dynamic_cast<QProgressBar*>(game->layout()->itemAt(1)->layout()->itemAt(1)->widget());
    PlayerHP->setRange(0, controller->getCurrentHealth());
    // Aggiorna la barra dell'enemy
    QProgressBar* enemyHP = dynamic_cast<QProgressBar*>(game->layout()->itemAt(0)->widget()->layout()->itemAt(1)->layout()->itemAt(1)->widget());
    enemyHP->setRange(0, controller->getEnemyCurrentHealth());
    updateHP();

    // Aggiorna il battleLog
    if(QLabel* battleLog = dynamic_cast<QLabel*>(game->layout()->itemAt(3)->widget()))
        battleLog->setText(controller->getEnemyName() + " Approches");
}

// Costruisce la schermata di gioco per il combattimento o la imposta come currentWidget dello stack
void FightingGame::setGame() {
    if(game) {  // Se e' gia' stata creata
        stack->setCurrentWidget(game);  // La imposta
        return;
    }

    // enemy Widget
    QWidget* enemyWidget = createEnemyWidget();
    setEnemyWidget(enemyWidget);

    QStackedWidget* inventoryStack = new QStackedWidget;

// actionButtons
    QPushButton* attackButton = new QPushButton("Attack");
    QPushButton* openInvButton = new QPushButton("Open Inventory");
    connect(attackButton, SIGNAL(clicked()), controller, SLOT(playerAttack()));
    connect(openInvButton, SIGNAL(clicked()), this, SLOT(setFWDInv()));
    // actionButtonsWidget
    QHBoxLayout* actionButtonsLayout = new QHBoxLayout;
    actionButtonsLayout->addWidget(attackButton, 0, Qt::AlignTop);
    actionButtonsLayout->addWidget(openInvButton, 0, Qt::AlignTop);
    QWidget* actionButtonsWidget = new QWidget;
    actionButtonsWidget->setLayout(actionButtonsLayout);

    inventoryStack->addWidget(actionButtonsWidget);

// fullInventoryWidget
    // Back Button
    QPushButton* back = new QPushButton("back");
    connect(back, SIGNAL(clicked()), this, SLOT(setBackInv()));
    // Scroll Area
        // inventoryLayout
    QVBoxLayout* fullInventoryLayout = new QVBoxLayout;
    fullInventoryLayout->addWidget(back, 0, Qt::AlignRight);
    fullInventoryLayout->addWidget(createScrollAreaInventory(), 0, Qt::AlignTop);
        // Inventory Widget
    QWidget* fullInventoryWidget = new QWidget;
    fullInventoryWidget->setLayout(fullInventoryLayout);
    inventoryStack->addWidget(fullInventoryWidget);
    inventoryStack->setCurrentIndex(0);

    // Player
    QLabel* PlayerName = new QLabel(controller->getPlayerName());
     // Player HP
    QProgressBar* PlayerHP = new QProgressBar;
    PlayerHP->setRange(0, controller->getCurrentHealth());
    PlayerHP->setValue(PlayerHP->maximum());
    PlayerHP->setTextVisible(false);
    PlayerHP->setMinimumWidth(400);
    PlayerHP->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    PlayerHP->setStyleSheet("QProgressBar{Background-color:DarkGray; border: 1px solid black; border-radius: 2px;}"
                            "QProgressBar::chunk{background-color:SpringGreen; margin:0px;}");

    // Player info Layout
    QHBoxLayout* PlayerInfo = new QHBoxLayout;
    PlayerInfo->addWidget(PlayerName);
    PlayerInfo->addWidget(PlayerHP, 0, Qt::AlignCenter);

    // battleLog
    QLabel* battleLog = new QLabel;
    battleLog->setStyleSheet("QLabel { background-color:#606060; border: 1px solid #505050;}");
    battleLog->setFont(QFont("arial", 12));
    battleLog->setAlignment(Qt::AlignVCenter);
    battleLog->setMargin(10);
    battleLog->setIndent(10);

    // gameLayout
    QGridLayout* gameLayout = new QGridLayout;
    gameLayout->addWidget(enemyWidget, 0, 0, Qt::AlignCenter | Qt::AlignTop);
    gameLayout->addLayout(PlayerInfo, 1, 0, Qt::AlignCenter);
    gameLayout->addWidget(inventoryStack, 2, 0, -1, 1);
    inventoryStack->setMinimumHeight(200);
    gameLayout->addWidget(battleLog, 3, 0, 1, 1, Qt::AlignTop);

    // game
    game = new QWidget;
    game->setLayout(gameLayout);
    stack->addWidget(game);
    stack->setCurrentWidget(game);

    if(!isFullScreen() && !isMaximized())
        resize(1200, 800);

}

// Costruisce la schermata di informazioni sul gioco o la imposta come currentWidget dello stack
void FightingGame::setInfo() {
    if(info) {
        stack->setCurrentWidget(info);
        return;
    }

    // backButton
    QPushButton* back = new QPushButton("Back");
    connect(back, SIGNAL(clicked()), this, SLOT(setMainMenu()));

    //infoTxt
    QFile file(":/Resources/info.txt");
    QLabel* text = new QLabel;
    text->setFont(QFont("arial", 16));
    text->setWordWrap(true);
    text->setMargin(30);
    text->setStyleSheet("background-color:#606060");

    QString line;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while(!stream.atEnd())
            line.append(stream.readLine() + "\n");
        text->setText(line);
    }
    file.close();

    // infoTxt ScrollArea
    QScrollArea* textArea = new QScrollArea;
    textArea->setWidget(text);
    textArea->setWidgetResizable(true);

    QVBoxLayout* infoLayout = new QVBoxLayout;
    infoLayout->addWidget(back);
    infoLayout->addWidget(textArea);

    // info
    info = new QWidget;
    info->setLayout(infoLayout);

    stack->addWidget(info);
    stack->setCurrentWidget(info);

    if(!isFullScreen() && !isMaximized())
        resize(800, 500);
}

// Costruisce il menu' principale del gioco o lo imposta come currentWidget dello stack
void FightingGame::setMainMenu() {
    if(mainMenu) {
        stack->setCurrentWidget(mainMenu);
        this->setMinimumSize(mainMenu->sizeHint().width()+25, mainMenu->sizeHint().height());
        if(!isFullScreen() && !isMaximized())
            resize(0,0);
        return;
    }

    // Logo
    QLabel* gameLogo = new QLabel;
    QImage image(QString(":/Resources/Images/fglogo.png"));
    gameLogo->setPixmap(QPixmap::fromImage(image));
    gameLogo->setMinimumSize(800,230);

    // mainMenuButtons
    QPushButton* startButton = new QPushButton("Start Game");
    QPushButton* infoButton = new QPushButton("How to Play");
    startButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    QPushButton* quit = new QPushButton("Quit Game");
    startButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    connect(startButton, SIGNAL(clicked()), this, SLOT(changePlayerName()));
    connect(infoButton, SIGNAL(clicked()), this, SLOT(setInfo()));
    connect(quit, SIGNAL(clicked()), this, SLOT(confirmQuit()));

    // mainMenuLayout
    QVBoxLayout* mainMenuLayout = new QVBoxLayout;
    mainMenuLayout->addWidget(gameLogo, 0, Qt::AlignCenter);
    mainMenuLayout->addWidget(startButton);
    mainMenuLayout->addWidget(infoButton);
    mainMenuLayout->addWidget(quit);

    // mainMenu
    mainMenu = new QWidget;
    mainMenu->setLayout(mainMenuLayout);

    stack->addWidget(mainMenu);
    stack->setCurrentWidget(mainMenu);
}

// Elimina il layout e tutti i suoi elementi
void FightingGame::clearLayout(QLayout* layout) {
    QLayoutItem *item;
    while ((item = layout->takeAt(0))) {    // Per ogni elemento

        if(item->layout())         // Se layout
            clearLayout(item->layout());    // delete layout
        else {
            delete item->widget(); // delete the widget (delete su nullptr no-op)
            delete item;   // delete the layout item (non possibile se delete layoutItem->layout() )
        }
    }
    delete layout;  // delete layout
}

// Crea e restituisce il widget dell'enemy
QGroupBox* FightingGame::createEnemyWidget() {
    // enemyImage
    QLabel *enemyImage = new QLabel;

    // Enemy name
    QLabel* enemyName = new QLabel;
    enemyName->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    // Enemy HP
    QProgressBar* enemyHP = new QProgressBar;
    enemyHP->setTextVisible(false);
    enemyHP->setMinimumWidth(250);
    enemyHP->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    enemyHP->setStyleSheet("QProgressBar{Background-color:DarkGray; border: 1px solid black; border-radius: 2px;}"
                               "QProgressBar::chunk{background-color:SpringGreen; margin:0px;}");

    // enemy info Layout
    QVBoxLayout* enemyInfo = new QVBoxLayout;
    enemyInfo->addWidget(enemyName);
    enemyInfo->addWidget(enemyHP);

    // enemyWidget
    QGroupBox* enemyWidget = new QGroupBox;
    enemyWidget->setStyleSheet("QGroupBox { background-color:#606060; border: 1px solid #505050;}");


    // enemyWidget layout
    QGridLayout* enemyWLayout = new QGridLayout;
    enemyWLayout->addWidget(enemyImage, 0, 0);
    enemyWLayout->addLayout(enemyInfo, 0, 1, Qt::AlignCenter | Qt::AlignLeft);
    enemyWidget->setLayout(enemyWLayout);

    return enemyWidget;
}

// Aggiorna l'enemyWidget dopo una vittoria
void FightingGame::setEnemyWidget(QWidget* enemyWidget) {
    // Aggiorna l'enemyImage
    QLabel* enemyImage = dynamic_cast<QLabel*>(enemyWidget->layout()->itemAt(0)->widget());
    QImage image(QString(":/Resources/Images/enemy" + QString(std::to_string(controller->getWinCount()+1).c_str()) + ".png"));
    enemyImage->setPixmap(QPixmap::fromImage(image));

    // Aggiorna l'enemyName
    QLabel* enemyName = dynamic_cast<QLabel*>(enemyWidget->layout()->itemAt(1)->layout()->itemAt(0)->widget());
    enemyName->setText(controller->getEnemyName());

    // Aggiorna enemyHP
    QProgressBar* enemyHP = dynamic_cast<QProgressBar*>(enemyWidget->layout()->itemAt(1)->layout()->itemAt(1)->widget());
    enemyHP->setRange(0, controller->getEnemyCurrentHealth());
    enemyHP->setValue(enemyHP->maximum());
}

// Crea la ScrollArea contenente l'inventario del Player
QScrollArea* FightingGame::createScrollAreaInventory() {
//    QScrollArea* scrollArea = new QScrollArea;
    QHBoxLayout* inventoryLayout = new QHBoxLayout;

    SList<std::shared_ptr<Item>> PlayerInv = controller->getPlayerInventory();

    // itemButtons
    for(SList<std::shared_ptr<Item>>::iterator it = PlayerInv.begin(); it != PlayerInv.end(); it++) {
        ItemButton* itemButton = new ItemButton((*it).get());
        connect(itemButton, SIGNAL(clicked(Item*)), controller, SLOT(playerUseItem(Item*)));
        inventoryLayout->addWidget(itemButton, 0, Qt::AlignBottom);
    }

    // inventoryWidget
    QWidget* inventoryWidget = new QWidget;
    inventoryWidget->setLayout(inventoryLayout);


    // scrollArea
    QScrollArea* scrollAreaInventory = new QScrollArea;
    scrollAreaInventory->setWidget(inventoryWidget);
    scrollAreaInventory->setMaximumHeight(55);
    scrollAreaInventory->setStyleSheet("background-color: #606060");
    scrollAreaInventory->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    return scrollAreaInventory;
}

// Imposta il currentWidget di inventoryStack agli actionButtons
void FightingGame::setBackInv() {
    if(QStackedWidget* inventoryStack = dynamic_cast<QStackedWidget*>(game->layout()->itemAt(2)->widget()))
        inventoryStack->setCurrentIndex(0);
}

// Imposta il currentWidget di inventoryStack all' inventario
void FightingGame::setFWDInv() {
    if(QStackedWidget* inventoryStack = dynamic_cast<QStackedWidget*>(game->layout()->itemAt(2)->widget()))
        inventoryStack->setCurrentIndex(1);
}

// Aggiorna il battleLog
void FightingGame::showMessage(QString message) {
    if(QLabel* battleLog = dynamic_cast<QLabel*>(game->layout()->itemAt(3)->widget()))
        battleLog->setText(message);
}

// Dialog che notifica il termine della partita
void FightingGame::youWon() {
    //Dialog
    QVBoxLayout* dLayout = new QVBoxLayout;
    QDialog* dialog = txtDialog("Victory", "YOU WON!", dLayout);
    dialog->setFixedSize(180, 100);

    //ButtonBox
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    buttonBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(close()));
    connect(buttonBox, SIGNAL(accepted()), controller, SLOT(resetGame()));

    //Layout
    dLayout->addWidget(buttonBox);
    dLayout ->setAlignment(dLayout, Qt::AlignRight);

    dialog->show();
}

// Dialog che notifica il termine della partita
void FightingGame::youLost() {
    //Dialog
    QVBoxLayout* dLayout = new QVBoxLayout;
    QDialog* dialog = txtDialog("Game Over", "You Lost!", dLayout);
    dialog->setFixedSize(200, 100); //toReview


    //ButtonBox
    QPushButton* button = new QPushButton(":(");
    connect(button, SIGNAL(clicked()), dialog, SLOT(close()));
    connect(button, SIGNAL(clicked()), controller, SLOT(resetGame()));

    //Layout
    dLayout->addWidget(button);
    dLayout ->setAlignment(dLayout, Qt::AlignRight);

    dialog->show();
}

// Dialog che implementa la fase di reward dopo una vittoria
void FightingGame::rewardPhase(SList<std::shared_ptr<Equipment>> eqRew, std::string itemRew_txt) {
    // eqRewDialog
    QVBoxLayout* eqRewLayout = new QVBoxLayout;
    QDialog* eqRewDialog = txtDialog("Reward", "Choose your reward:", eqRewLayout);

    // eqRewButtons
    QVBoxLayout* rewLayout = createEqRewBox(eqRew);
    QPushButton* nextButton = dynamic_cast<QPushButton*>(rewLayout->itemAt(1)->widget());
    connect(nextButton, SIGNAL(clicked()), eqRewDialog, SLOT(close()));
    eqRewLayout->addLayout(rewLayout);

    // eqRewItems
    QLabel* itemRew = new QLabel(QString(itemRew_txt.c_str()));
    itemRew->setWordWrap(true);
    itemRew->setAlignment(Qt::AlignCenter);
    itemRew->setFont(QFont("arial", 13, QFont::Bold));

    eqRewLayout->insertWidget(eqRewLayout->count()+1, itemRew);


    eqRewDialog->resize(600, 500);
    eqRewDialog->show();
}

// Dialog per la conferma della chiusura dell'applicazione
void FightingGame::confirmQuit() {
    //Dialog
    QVBoxLayout* dLayout = new QVBoxLayout;
    QDialog* dialog = txtDialog("Quit", "Are you sure you want to quit?", dLayout);
    dialog->setFixedSize(200, 150);

    //ButtonBox
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Yes | QDialogButtonBox::Cancel, dialog);
    buttonBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(buttonBox, SIGNAL(accepted()), qApp, SLOT(quit()));
    connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(close()));

    //Layout
    dLayout->addWidget(buttonBox);
    dLayout ->setAlignment(dLayout, Qt::AlignRight);

    dialog->show();
}

// Costruisce il widget per l'inserimento del Player name o lo imposta come currentWidget dello stack
void FightingGame::changePlayerName() {
    if(changeName) {
        setMinimumSize(0,0);
        stack->setCurrentWidget(changeName);
        setMinimumSize(0,0);
        return;
    }

    // txtLabel
    QLabel* label = new QLabel("Enter your character's name");

    // LineEdit
    QLineEdit* txt = new QLineEdit;
    txt->setMaxLength(15);
    txt->setAlignment(Qt::AlignCenter);

    // continueButton
    QPushButton* continueButton = new QPushButton("Continue");
    continueButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    connect(continueButton, SIGNAL(clicked()), this, SLOT(savePlayerName()));
    connect(continueButton, SIGNAL(clicked()), this, SLOT(setGame()));

    // lineButtonLayout
    QHBoxLayout* lineButtonLayout = new QHBoxLayout;
    lineButtonLayout->addWidget(txt, 0, Qt::AlignTop);
    lineButtonLayout->addWidget(continueButton, 0, Qt::AlignTop);

    // changeNameLayout
    QVBoxLayout* changeNameLayout = new QVBoxLayout;
    changeNameLayout->addWidget(label, 0, Qt::AlignCenter);
    changeNameLayout->addLayout(lineButtonLayout);

    // changeName
    changeName = new QWidget;
    changeName->setLayout(changeNameLayout);

    stack->addWidget(changeName);
    stack->setCurrentWidget(changeName);
}

// Modifica il nome del Player con quello inserito
void FightingGame::savePlayerName() {
    if(QLineEdit* w = dynamic_cast<QLineEdit*>(changeName->layout()->itemAt(1)->layout()->itemAt(0)->widget()))
            controller->changePlayerName(w->text().toStdString());
}

// Template per creazione parziale di dialog con testo
QDialog* FightingGame::txtDialog(QString title, QString text, QLayout* dialogLayout) {
    // txtDialog
    QDialog* txtDialog = new QDialog();
    txtDialog->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    txtDialog->setWindowTitle(title);
    txtDialog->setModal(true);
    txtDialog->setStyleSheet("background-color:DimGray");
    txtDialog->setAttribute(Qt::WA_DeleteOnClose);


    //Label
    QLabel* label = new QLabel(text, txtDialog);
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);
    label->setFont(QFont("arial", 14, QFont::Bold));

    dialogLayout->addWidget(label);
    txtDialog->setLayout(dialogLayout);

    return txtDialog;
}

// Funzione ausiliaria a rewardPhase
// Crea e restituisce il layout contenente i tre Equipment reward
QVBoxLayout* FightingGame::createEqRewBox(SList<std::shared_ptr<Equipment>>& eqList) const {
    //nextButton
    QPushButton* nextButton = new QPushButton("Next Battle");
    nextButton->hide();

    //buttonGroup
    QHBoxLayout* rewBoxLayout = new QHBoxLayout;
    QButtonGroup* buttonGroup = new QButtonGroup;

    for(SList<std::shared_ptr<Equipment>>::iterator it = eqList.begin(); it != eqList.end(); it++) {
        // Name
        QLabel* eqName = new QLabel(QString((*it)->getName().c_str()));
        // Desc
        QLabel* eqValue = new QLabel( QString((*it)->getDesc().c_str()) );

        // RadioButton
        EquipmentRadioButton* eqRButton = new EquipmentRadioButton( (*it).get() );
        buttonGroup->addButton(eqRButton);

        connect(eqRButton, SIGNAL(clicked()), nextButton, SLOT(show()));
        connect(nextButton, SIGNAL(clicked()), eqRButton, SLOT(emitEquip()));
        connect(eqRButton, SIGNAL(clicked(Equipment*)), controller, SLOT(prepareNextBattle(Equipment*)));

        // Layout
        QVBoxLayout* eqLayout = new QVBoxLayout;
        eqLayout->addWidget(eqName);
        eqLayout->addWidget(eqValue);
        eqLayout->addWidget(eqRButton);
        // GroupBox
        QGroupBox* eqBox = new QGroupBox;
        eqBox->setLayout(eqLayout);

        rewBoxLayout->addWidget(eqBox);
    }

    // rewLayout
    QVBoxLayout* rewLayout = new QVBoxLayout;
    rewLayout->addLayout(rewBoxLayout);
    rewLayout->addWidget(nextButton);

    return rewLayout;
}

// Aggiorna tutti (e possibilmente aggiunge) itemButtons dopo una vittoria
void FightingGame::updateInventory(const SList<std::shared_ptr<Item>>& inv) {
    // Ottiene gli widget necessari
    QStackedWidget* inventoryStack;
    QScrollArea* scrollAreaInventory;
    QHBoxLayout* itemButtonLayout;
    if ((inventoryStack = (dynamic_cast<QStackedWidget*>(game->layout()->itemAt(2)->widget()))))
        if((scrollAreaInventory = (dynamic_cast<QScrollArea*>(inventoryStack->widget(1)->layout()->itemAt(1)->widget()))))
            if((itemButtonLayout = (dynamic_cast<QHBoxLayout*>(scrollAreaInventory->widget()->layout())))) {

                inventoryStack->setCurrentIndex(0);

                ItemButton* itemButton;
                for(int i =0; i < itemButtonLayout->count(); i++) { // Per ogni itemButton
                    itemButton = dynamic_cast<ItemButton*>(itemButtonLayout->itemAt(i)->widget());
                    if(!itemButton->isEnabled())    // Se e' disattivato
                        itemButton->tryToEnable();      // Prova a riattivarlo
                }

                bool idMatch = false;
                for(SList<std::shared_ptr<Item>>::const_iterator it = inv.begin(); it != inv.end(); it++) { // Per ogni Item nell'inventario
                    usInt invItemID = (*it)->getID();
                    idMatch = false;

                                                // Confrnta l'Item dell'inventario in esame con tutti gli itemButtons
                    for(int i =0; i < itemButtonLayout->count() && !idMatch; i++) { // Per ogni itemButton
                        itemButton = dynamic_cast<ItemButton*>(itemButtonLayout->itemAt(i)->widget());
                        if(invItemID == itemButton->getItem()->getID()) // Se gli ID corrispondono
                            idMatch = true;                                 // Non fara' niente
                    }
                    if(!idMatch) {                                      // Se non esiste match per l'Item dell'inventario in esame con gli itemButtons gia' creati
                        ItemButton* itemButton = new ItemButton((*it).get());
                        itemButtonLayout->addWidget(itemButton);            // Si aggiunge tale Item come itemButton
                        connect(itemButton, SIGNAL(clicked(Item*)), controller, SLOT(playerUseItem(Item*)));
                    }
                }

                // Aggiorna la width di inventoryWidget
                QWidget* inventoryWidget = scrollAreaInventory->widget();
                inventoryWidget->setMinimumWidth(inventoryWidget->sizeHint().width());
                inventoryWidget->adjustSize();
            }
}

// Aggiorna l'itemButton corrispondente all'Item utilizzato dal Player
void FightingGame::updateItemButton(Item* item)
{
    if(item) {
        QStackedWidget* inventoryStack;
        QScrollArea* scrollAreaInventory;
        QHBoxLayout* itemButtonLayout;
        if ((inventoryStack = dynamic_cast<QStackedWidget*>(game->layout()->itemAt(2)->widget())))
            if((scrollAreaInventory = dynamic_cast<QScrollArea*>(inventoryStack->widget(1)->layout()->itemAt(1)->widget())))
                if((itemButtonLayout = dynamic_cast<QHBoxLayout*>(scrollAreaInventory->widget()->layout()))) {
                    for(int i = 0; i < itemButtonLayout->count(); i++)  // Cerca l'itemButton con ID corrispondente all' ID dell' Item utilizzato dal Player
                        if(ItemButton* itemButton = dynamic_cast<ItemButton*>(itemButtonLayout->itemAt(i)->widget()))
                            if(item->getID() == itemButton->getItem()->getID())
                                itemButton->update();                       // Aggiorna l'itemButton
                }
    }
}

// Dialog di notifica di errore runtime
void FightingGame::errorDialog(std::runtime_error e)
{
    // errorDialog
    QVBoxLayout* errorDialogLayout = new QVBoxLayout;
    QDialog* errorDialog = txtDialog("Error", e.what(), errorDialogLayout);

    // closeButton
    QPushButton* closeButton = new QPushButton("Close");
    connect(closeButton, SIGNAL(clicked()), qApp, SLOT(quit()));
    errorDialogLayout->addWidget(closeButton);

    errorDialog->show();
}

// reset del game
void FightingGame::reset() {
    stack->removeWidget(game);
    clearLayout(game->layout());
    delete game;
    game = 0;
    setGame();
    setMainMenu();
}
