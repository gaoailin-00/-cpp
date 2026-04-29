#pragma once//防止重复包含
#include<iostream>
#include<string>
#pragma comment(lib, "user32.lib") 
#include "card.h"
#include "player.h"
#include "npc.h"
#include "gameround.h"
#include "fapai.h"
#include "targetcard.h"
#include "choosenpc.h"
#include "ending.h"
using namespace std;
//卡牌类
class Card {
public://因为想在后续访问，所以设成public
    string cardsuit;
    int cardnum;
public:
    Card() {}
    Card(string a, int b) : cardsuit(a), cardnum(b) {}
    string getcardsuit()const {
        return cardsuit;
    }                 //只读不改花色
    int getcardnum()const {
        return cardnum;
    }                 //只读不改牌面
    void cardshow()const {//注意在class中加const的位置
        cout << cardsuit << "花色的" << cardnum << endl;
    }
    bool rightcard()const {//检验本张牌是否为目标牌Q）
        return(cardnum == 12);//需要规定Q=12 这种return也很惊艳
    }
};

