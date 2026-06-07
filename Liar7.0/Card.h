#ifndef CARD_H
#define CARD_H

#include <QString>

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

#endif // CARD_H