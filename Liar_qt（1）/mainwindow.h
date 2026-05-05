#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <algorithm>
#include <QList>
#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QDialog>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QRandomGenerator>
#include <QVector>
#include <QString>
#include <QStringList>

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

    QString toString() const {
        QString numText;
        switch (num) {
        case 1: numText = "A"; break;
        case 11: numText = "J"; break;
        case 12: numText = "Q"; break;
        case 13: numText = "K"; break;
        default: numText = QString::number(num); break;
        }
        return QString("%1 %2").arg(suit, numText);
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
};

// 选牌对话框
class CardSelectDialog : public QDialog {
    Q_OBJECT
public:
    explicit CardSelectDialog(const QVector<Card>& cards, QWidget* parent = nullptr);
    QVector<int> selectedIndexes() const;

private slots:
    void onAccept();

private:
    QList<QCheckBox*> checkBoxes;
    QVector<int> m_selectedIndexes;
};

// 主窗口
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();

private:
    QStackedWidget* stack = nullptr;
    QWidget* startScene = nullptr;
    QWidget* tavernScene = nullptr;
    QWidget* battleScene = nullptr;
    QWidget* endScene = nullptr;

    PlayerData player;
    NPCData currentNpc;
    QVector<Card> currentOutCards;
    bool playerWonLastGame = false;

    QLabel* iouLabel = nullptr;
    QLabel* battleTitleLabel = nullptr;
    QLabel* npcNameLabel = nullptr;
    QLabel* playerNameLabel = nullptr;
    QLabel* roundInfoLabel = nullptr;
    QVector<QLabel*> npcCardLabels;
    QVector<QLabel*> playerCardLabels;

    QPushButton* playCardBtn = nullptr;
    QPushButton* viewHandBtn = nullptr;
    QPushButton* challengeBtn = nullptr;
    QPushButton* backBtn = nullptr;

    QLabel* endResultLabel = nullptr;
    QLabel* endTipLabel = nullptr;

    Card createRandomCard();
    void give5Cards(PlayerData& p);
    void give5Cards(NPCData& npc);
    QString npcSpeak(const QString& name);
    void startBattle(const QString& npcName);
    void doPlayerPlayCard();
    void resolveNpcChallenge();
    void playerChallengeNpc();
    void showPlayerHand();
    void goEndScene(bool win);
    void restartToTavern();
    void updateTavernInfo();
    void updateBattleUI();
    void updateEndScene();

    void createStartScene();
    void createTavernScene();
    void createBattleScene();
    void createEndScene();
};

#endif // MAINWINDOW_H
