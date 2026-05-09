#pragma once

#ifndef CARD_H
#define CARD_H

#include <QString>

// ==========================
// 卡牌类
// ==========================
class Card {
public:
    QString suit;
    int num;

    Card();
    Card(const QString& s, int n);

    bool isTargetQ() const;
    bool isRed() const;
    QString getSuitSymbol() const;
    QString getNumText() const;
    QString toString() const;
};

#endif // CARD_H