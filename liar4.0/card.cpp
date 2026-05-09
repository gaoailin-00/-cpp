#include "card.h"

Card::Card() : suit(""), num(0) {}

Card::Card(const QString& s, int n) : suit(s), num(n) {}

bool Card::isTargetQ() const {
    return num == 12;
}

bool Card::isRed() const {
    return suit == "红桃" || suit == "方片";
}

QString Card::getSuitSymbol() const {
    if (suit == "红桃") return "♥";
    if (suit == "黑桃") return "♠";
    if (suit == "方片") return "♦";
    if (suit == "梅花") return "♣";
    return "?";
}

QString Card::getNumText() const {
    switch (num) {
    case 11: return "J";
    case 12: return "Q";
    case 13: return "K";
    default: return QString::number(num);
    }
}

QString Card::toString() const {
    return QString("%1 %2").arg(suit, getNumText());
}