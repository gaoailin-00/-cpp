#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QFrame>
#include <QPixmap>
#include <QCoreApplication>
#include <QSizePolicy>

#include "card.h"
#include "playerdata.h"
#include "npcdata.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();

private:
    enum TurnState {
        PlayerTurn,
        WaitingNpcChallenge,
        NpcTurn,
        WaitingPlayerDecision,
        BattleEnded
    };

    QStackedWidget* stack = nullptr;
    QWidget* introScene = nullptr;
    QWidget* startScene = nullptr;
    QWidget* historyScene = nullptr;
    QWidget* ruleScene = nullptr;
    QWidget* tavernScene = nullptr;
    QWidget* battleScene = nullptr;
    QWidget* roundResultScene = nullptr;
    QWidget* endScene = nullptr;

    // 新增：点餐菜单场景与结果场景
    QWidget* menuScene = nullptr;
    QWidget* orderResultScene = nullptr;

    PlayerData player;
    NPCData currentNpc;
    QVector<Card> currentOutCards;
    QVector<Card> cardPool;
    bool playerWonLastGame = false;
    TurnState turnState = PlayerTurn;

    QLabel* iouLabel = nullptr;

    QLabel* battleTitleLabel = nullptr;
    QLabel* roundInfoLabel = nullptr;
    QLabel* npcAvatarLabel = nullptr;
    QLabel* playerAvatarLabel = nullptr;
    QLabel* npcSpeechLabel = nullptr;

    QVector<QLabel*> npcCardLabels;
    QVector<QPushButton*> playerCardBtns;

    QPushButton* playCardBtn = nullptr;
    QPushButton* challengeNpcBtn = nullptr;
    QPushButton* passNpcBtn = nullptr;
    QPushButton* backBtn = nullptr;

    QLabel* roundResultImageLabel = nullptr;
    QLabel* roundResultTextLabel = nullptr;

    QLabel* endImageLabel = nullptr;

    // 新增：点餐结果图片展示
    QLabel* orderResultImageLabel = nullptr;

    QTimer* battleMessageTimer = nullptr;

    void initCardPool();
    void draw5Cards(PlayerData& p);
    void draw5Cards(NPCData& npc);

    QString npcSpeak(const QString& name);
    QString getNpcImagePath(const QString& name);
    QString getNpcResultImagePath(const QString& name);
    QString getNpcWinDialogue(const QString& name);
    QString getNpcLoseDialogue(const QString& name);

    void startBattle(const QString& npcName);
    void doPlayerPlayCard();
    void resolveNpcChallenge();
    void doNpcPlayCard();
    void resolvePlayerChallenge();
    void doPlayerPass();
    void handleRoundWin(const QString& message);
    void handleRoundLose(const QString& message);

    void goEndScene(bool win);
    void restartToTavern();
    void updateTavernInfo();
    void updateBattleUI();
    void updateEndScene();
    void updateActionButtons();

    // 新增：展示点餐结果
    void showOrderResult(int itemIndex);

    void createIntroScene();
    void createStartScene();
    void createHistoryScene();
    void createRuleScene();
    void createTavernScene();
    void createBattleScene();
    void createRoundResultScene();
    void createEndScene();

    // 新增：创建菜单场景与结果场景
    void createMenuScene();
    void createOrderResultScene();
};

#endif // MAINWINDOW_H
