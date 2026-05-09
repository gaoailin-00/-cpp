#pragma once

#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include <QString>
#include <QVector>
#include "card.h"

// ==========================
// 玩家数据
// ==========================
class PlayerData {
public:
    QString name;
    QVector<Card> handcards;
    int iouCount;

    PlayerData(const QString& n = "旅人");

    void clearCards();
    void addCard(const Card& c);
    int cardCount() const;
    void removeCardsByIndexes(QVector<int> indexes);
};

#endif // PLAYER_DATA_H