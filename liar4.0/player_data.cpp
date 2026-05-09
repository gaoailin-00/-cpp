#include "player_data.h"
#include <algorithm>
#include <functional>

PlayerData::PlayerData(const QString& n) : name(n), iouCount(0) {}

void PlayerData::clearCards() {
    handcards.clear();
}

void PlayerData::addCard(const Card& c) {
    handcards.push_back(c);
}

int PlayerData::cardCount() const {
    return handcards.size();
}

void PlayerData::removeCardsByIndexes(QVector<int> indexes) {
    std::sort(indexes.begin(), indexes.end());
    indexes.erase(std::unique(indexes.begin(), indexes.end()), indexes.end());
    std::sort(indexes.begin(), indexes.end(), std::greater<int>());
    for (int idx : indexes) {
        if (idx >= 0 && idx < handcards.size()) {
            handcards.removeAt(idx);
        }
    }
}