#pragma once

#ifndef NPC_DATA_H
#define NPC_DATA_H

#include <QString>
#include <QVector>
#include "card.h"

// ==========================
// NPC数据
// ==========================
class NPCData {
public:
    QString name;
    QVector<Card> handcards;

    NPCData(const QString& n = "");

    void clearCards();
    void addCard(const Card& c);
    int cardCount() const;
    void removeCardsByIndexes(QVector<int> indexes);
};

#endif // NPC_DATA_H