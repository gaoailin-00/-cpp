#pragma once

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
#include <QTextEdit>

// 引入拆分后的数据模型
#include "player_data.h"
#include "npc_data.h"

// ==========================
// 主窗口
// ==========================
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

    // ===== 场景 =====
    QStackedWidget* stack = nullptr;
    QWidget* introScene = nullptr;
    QWidget* startScene = nullptr;
    QWidget* historyScene = nullptr;
    QWidget* ruleScene = nullptr;
    QWidget* tavernScene = nullptr;
    QWidget* battleScene = nullptr;
    QWidget* roundResultScene = nullptr;
    QWidget* endScene = nullptr;

    // ===== 游戏数据 =====
    PlayerData player;
    NPCData currentNpc;
    QVector<Card> currentOutCards;
    QVector<Card> cardPool;
    bool playerWonLastGame = false;
    TurnState turnState = PlayerTurn;

    // ===== 酒馆大厅 UI =====
    QLabel* iouLabel = nullptr;

    // ===== 战斗界面 UI =====
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

    // ===== 单局结算界面 UI =====
    QLabel* roundResultImageLabel = nullptr;
    QLabel* roundResultTextLabel = nullptr;

    // ===== 结局界面 UI =====
    QLabel* endImageLabel = nullptr;

    // ===== 定时器 =====
    QTimer* battleMessageTimer = nullptr;

    // ===== 牌库方法 =====
    void initCardPool();
    void draw5Cards(PlayerData& p);
    void draw5Cards(NPCData& npc);

    // ===== NPC 资源与台词 =====
    QString npcSpeak(const QString& name);
    QString getNpcImagePath(const QString& name);
    QString getNpcResultImagePath(const QString& name);
    QString getNpcWinDialogue(const QString& name);
    QString getNpcLoseDialogue(const QString& name);

    // ===== 战斗逻辑 =====
    void startBattle(const QString& npcName);
    void doPlayerPlayCard();
    void resolveNpcChallenge();
    void doNpcPlayCard();
    void resolvePlayerChallenge();
    void doPlayerPass();
    void handleRoundWin(const QString& message);
    void handleRoundLose(const QString& message);

    // ===== UI 更新与场景切换 =====
    void goEndScene(bool win);
    void restartToTavern();
    void updateTavernInfo();
    void updateBattleUI();
    void updateEndScene();
    void updateActionButtons();

    // ===== 场景创建 =====
    void createIntroScene();
    void createStartScene();
    void createHistoryScene();
    void createRuleScene();
    void createTavernScene();
    void createBattleScene();
    void createRoundResultScene();
    void createEndScene();
};

#endif // MAINWINDOW_H