#ifndef NPCDATA_H
#define NPCDATA_H

#include <QVector>
#include <QString>
#include <algorithm>
#include "card.h"

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

#endif // NPCDATA_H