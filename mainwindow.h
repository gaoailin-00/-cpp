#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <algorithm>
#include <random>
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

// 卡牌类
class Card {
public:
    QString suit;
    int num;

    Card() : suit(""), num(0) {}
    Card(const QString& s, int n) : suit(s), num(n) {}

    bool isTargetQ() const {
        return num == 12;
    }

    bool isRed() const {
        return suit == "红桃" || suit == "方片";
    }

    QString getSuitSymbol() const {
        if (suit == "红桃") return "♥";
        if (suit == "黑桃") return "♠";
        if (suit == "方片") return "♦";
        if (suit == "梅花") return "♣";
        return "?";
    }

    QString getNumText() const {
        switch (num) {
        case 11: return "J";
        case 12: return "Q";
        case 13: return "K";
        default: return QString::number(num);
        }
    }

    QString toString() const {
        return QString("%1 %2").arg(suit, getNumText());
    }
};

// 玩家数据
class PlayerData {
public:
    QString name;
    QVector<Card> handcards;
    int iouCount = 0;

    PlayerData(const QString& n = "旅人") : name(n) {}

    void clearCards() { handcards.clear(); }
    void addCard(const Card& c) { handcards.push_back(c); }
    int cardCount() const { return handcards.size(); }

    void removeCardsByIndexes(QVector<int> indexes) {
        std::sort(indexes.begin(), indexes.end());
        indexes.erase(std::unique(indexes.begin(), indexes.end()), indexes.end());
        std::sort(indexes.begin(), indexes.end(), std::greater<int>());

        for (int idx : indexes) {
            if (idx >= 0 && idx < handcards.size()) {
                handcards.removeAt(idx);
            }
        }
    }
};

// NPC数据
class NPCData {
public:
    QString name;
    QVector<Card> handcards;

    NPCData(const QString& n = "") : name(n) {}

    void clearCards() { handcards.clear(); }
    void addCard(const Card& c) { handcards.push_back(c); }
    int cardCount() const { return handcards.size(); }

    void removeCardsByIndexes(QVector<int> indexes) {
        std::sort(indexes.begin(), indexes.end());
        indexes.erase(std::unique(indexes.begin(), indexes.end()), indexes.end());
        std::sort(indexes.begin(), indexes.end(), std::greater<int>());

        for (int idx : indexes) {
            if (idx >= 0 && idx < handcards.size()) {
                handcards.removeAt(idx);
            }
        }
    }
};

// 主窗口
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

private:
    QStackedWidget* stack = nullptr;
    QWidget* introScene = nullptr;
    QWidget* startScene = nullptr;
    QWidget* tavernScene = nullptr;
    QWidget* battleScene = nullptr;
    QWidget* roundResultScene = nullptr; //单局结算全屏场景
    QWidget* endScene = nullptr;

    PlayerData player;
    NPCData currentNpc;
    QVector<Card> currentOutCards;
    QVector<Card> cardPool;
    bool playerWonLastGame = false;
    TurnState turnState = PlayerTurn;

    QLabel* iouLabel = nullptr;

    // 战斗界面
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

    // 单局结算界面组件
    QLabel* roundResultImageLabel = nullptr;
    QLabel* roundResultTextLabel = nullptr;

    // 结局界面
    QLabel* endImageLabel = nullptr;

    QTimer* battleMessageTimer = nullptr;

private:

    void initCardPool();
    void draw5Cards(PlayerData& p);
    void draw5Cards(NPCData& npc);

    QString npcSpeak(const QString& name);
    QString getNpcImagePath(const QString& name);
    QString getNpcResultImagePath(const QString& name); // 新增：获取结算专属大图

    // NPC台词
    QString getNpcWinDialogue(const QString& name);
    QString getNpcLoseDialogue(const QString& name);

    void startBattle(const QString& npcName);

    // 核心逻辑
    void doPlayerPlayCard();
    void resolveNpcChallenge();
    void doNpcPlayCard();
    void resolvePlayerChallenge();
    void doPlayerPass();

    void handleRoundWin(const QString& message);
    void handleRoundLose(const QString& message);

    // 流程
    void goEndScene(bool win);
    void restartToTavern();
    void updateTavernInfo();
    void updateBattleUI();
    void updateEndScene();
    void updateActionButtons();

    void createIntroScene();
    void createStartScene();
    void createTavernScene();
    void createBattleScene();
    void createRoundResultScene(); // 单局结算场景
    void createEndScene();
};

#endif