#include "npc_data.h"
#include <algorithm>
#include <functional>

NPCData::NPCData(const QString& n) : name(n) {}

void NPCData::clearCards() {
    handcards.clear();
}

void NPCData::addCard(const Card& c) {
    handcards.push_back(c);
}

int NPCData::cardCount() const {
    return handcards.size();
}

void NPCData::removeCardsByIndexes(QVector<int> indexes) {
    std::sort(indexes.begin(), indexes.end());
    indexes.erase(std::unique(indexes.begin(), indexes.end()), indexes.end());
    std::sort(indexes.begin(), indexes.end(), std::greater<int>());
    for (int idx : indexes) {
        if (idx >= 0 && idx < handcards.size()) {
            handcards.removeAt(idx);
        }
    }
}