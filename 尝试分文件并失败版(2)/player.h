#pragma once
#include<iostream>
#include <windows.h>
#include<vector>
#include<string>
#include<cstdlib>
#include<ctime>
#include <algorithm>
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
//玩家类
class Player {
private:
    string playername;
    vector<Card> handcards;
    int ioucount;
public:
    Player(string a = "") :playername(a) {}
    string getname()const {//get/set语法来自豆包
        return playername;
    }                 //只读不改人名

    void setname(string a) {
        playername = a;
    }                 //修改人名    ?留么

    Card getcard(int i)const {
        return handcards[i];
    }                 //只读不改卡牌

    void addcard(Card c) {
        handcards.push_back(c);
    }                 //添加卡牌

    int getcardcount()const {
        return handcards.size();
    }                 //查看手中卡牌数量

    void  showhandcards()const {
        cout << "玩家" << playername << " 的手牌如下" << endl;
        for (int i = 0; i < handcards.size(); i++) {
            handcards[i].cardshow();
            cout << " ";
        }
        cout << endl;
    }                 //展示手中的牌
    void givecard(vector<int>chupai) {//参数也要写vector
        sort(chupai.rbegin(), chupai.rend());//vector需要加一个人，
        //必须排序，然后从后往前删
        for (int i = chupai.size() - 1; i >= 0; i--) {
            handcards.erase(handcards.begin() + chupai[i]);
        }
    }//出牌
    void clear() {
        handcards.clear();
    }//清空玩家剩余卡牌，重开
    Player() {
        ioucount = 0;
    }
    int getiou() {
        return ioucount;
    }
    void addiou() {
        ioucount++;
    }
};