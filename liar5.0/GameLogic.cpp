#include "mainwindow.h"

void MainWindow::startBattle(const QString& npcName) {
    currentNpc = NPCData(npcName);

    initCardPool();
    draw5Cards(player);
    draw5Cards(currentNpc);
    currentOutCards.clear();

    playerWonLastGame = false;
    turnState = PlayerTurn;

    updateBattleUI();
    npcSpeechLabel->setText(npcSpeak(currentNpc.name));
    roundInfoLabel->setText(QString("对局开始！\n目标牌：Q\n现在轮到你出牌。"));
    updateActionButtons();

    stack->setCurrentWidget(battleScene);
}

void MainWindow::handleRoundWin(const QString& message) {
    player.iouCount++;
    playerWonLastGame = true;
    turnState = BattleEnded;
    updateActionButtons();

    roundInfoLabel->setText(message);

    QTimer::singleShot(2000, this, [this]() {
        QString imgPath = getNpcResultImagePath(currentNpc.name);
        QPixmap pix(imgPath);
        if (!pix.isNull()) {
            roundResultImageLabel->setPixmap(pix);
            roundResultImageLabel->setText("");
        } else {
            roundResultImageLabel->setText("【图片缺失: " + imgPath + "】");
        }

        if (player.iouCount >= 3) {
            roundResultTextLabel->setText("恭喜你，幸运又狡猾的年轻人，你拿回了全部的欠条！");
            stack->setCurrentWidget(roundResultScene);
            battleMessageTimer->singleShot(3000, this, [this]() {
                goEndScene(true);
            });
        } else {
            roundResultTextLabel->setText(getNpcWinDialogue(currentNpc.name));
            stack->setCurrentWidget(roundResultScene);
            battleMessageTimer->singleShot(3000, this, [this]() {
                restartToTavern();
            });
        }
    });
}

void MainWindow::handleRoundLose(const QString& message) {
    playerWonLastGame = false;
    turnState = BattleEnded;
    updateActionButtons();

    roundInfoLabel->setText(message);

    QTimer::singleShot(2000, this, [this]() {
        QString imgPath = getNpcResultImagePath(currentNpc.name);
        QPixmap pix(imgPath);
        if (!pix.isNull()) {
            roundResultImageLabel->setPixmap(pix);
            roundResultImageLabel->setText("");
        } else {
            roundResultImageLabel->setText("【图片缺失: " + imgPath + "】");
        }

        roundResultTextLabel->setText(getNpcLoseDialogue(currentNpc.name));
        stack->setCurrentWidget(roundResultScene);

        battleMessageTimer->singleShot(3000, this, [this]() {
            restartToTavern();
        });
    });
}

void MainWindow::doPlayerPlayCard() {
    if (turnState != PlayerTurn) return;

    QVector<int> selected;
    for (int i = 0; i < playerCardBtns.size() && i < player.handcards.size(); ++i) {
        if (playerCardBtns[i]->isChecked()) {
            selected.push_back(i);
        }
    }

    if (selected.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先点击选择至少一张手牌。");
        return;
    }

    currentOutCards.clear();
    for (int idx : selected) {
        if (idx >= 0 && idx < player.handcards.size()) {
            currentOutCards.push_back(player.handcards[idx]);
        }
    }

    player.removeCardsByIndexes(selected);
    updateBattleUI();

    turnState = WaitingNpcChallenge;
    updateActionButtons();
    roundInfoLabel->setText(
        QString("你打出了 %1 张牌，并声称它们都是 Q。\n%2 正在观察你的表情……")
            .arg(selected.size()).arg(currentNpc.name)
        );

    bool npcChallenge = (QRandomGenerator::global()->bounded(100) < 35);

    if (npcChallenge) {
        battleMessageTimer->singleShot(1400, this, &MainWindow::resolveNpcChallenge);
    } else {
        battleMessageTimer->singleShot(1400, this, [this]() {
            if (player.cardCount() == 0) {
                handleRoundWin(
                    QString("%1没有质疑，而你已经出完全部手牌。\n你赢下了这局！")
                        .arg(currentNpc.name)
                    );
            } else {
                roundInfoLabel->setText(
                    QString("%1：我不质疑。\n现在轮到我出牌了。").arg(currentNpc.name)
                    );
                turnState = NpcTurn;
                updateActionButtons();
                battleMessageTimer->singleShot(1200, this, &MainWindow::doNpcPlayCard);
            }
        });
    }
}

void MainWindow::resolveNpcChallenge() {
    QString result = QString("%1：我质疑你！翻牌！\n真实牌面：\n").arg(currentNpc.name);
    bool allQ = true;

    for (const Card& c : currentOutCards) {
        result += QString("%1%2 ").arg(c.getSuitSymbol(), c.getNumText());
        if (!c.isTargetQ()) allQ = false;
    }

    if (allQ) {
        handleRoundWin(result + "\n\n【全部都是真Q！】你赢了。");
    } else {
        handleRoundLose(result + "\n\n【你出了假牌，被识破了！】");
    }
}

void MainWindow::doNpcPlayCard() {
    if (turnState != NpcTurn) return;

    if (currentNpc.cardCount() <= 0) {
        handleRoundLose(
            QString("%1已经出完手牌。\n你错过了上一次质疑机会，这局判你失败。")
                .arg(currentNpc.name)
            );
        return;
    }

    int maxPlay = qMin(3, currentNpc.cardCount());
    int playCount = QRandomGenerator::global()->bounded(1, maxPlay + 1);

    QVector<int> selected;
    for (int i = 0; i < playCount; ++i) {
        selected.push_back(i);
    }

    currentOutCards.clear();
    for (int idx : selected) {
        currentOutCards.push_back(currentNpc.handcards[idx]);
    }

    currentNpc.removeCardsByIndexes(selected);
    updateBattleUI();

    roundInfoLabel->setText(
        QString("%1打出了 %2 张牌，并声称它们都是 Q。\n你要质疑吗？")
            .arg(currentNpc.name).arg(playCount)
        );

    turnState = WaitingPlayerDecision;
    updateActionButtons();
}

void MainWindow::resolvePlayerChallenge() {
    if (turnState != WaitingPlayerDecision) return;

    QString result = QString("你选择质疑 %1！\n真实牌面：\n").arg(currentNpc.name);
    bool allQ = true;

    for (const Card& c : currentOutCards) {
        result += QString("%1%2 ").arg(c.getSuitSymbol(), c.getNumText());
        if (!c.isTargetQ()) allQ = false;
    }

    if (allQ) {
        handleRoundLose(result + "\n\n【NPC出的都是真Q！】你质疑失败了。");
    } else {
        handleRoundWin(result + "\n\n【NPC撒谎，被你识破了！】你赢了。");
    }
}

void MainWindow::doPlayerPass() {
    if (turnState != WaitingPlayerDecision) return;

    if (currentNpc.cardCount() == 0) {
        handleRoundLose(
            QString("你没有质疑，而 %1 已经出完了全部手牌。\n这局你输了。")
                .arg(currentNpc.name)
            );
        return;
    }

    roundInfoLabel->setText(QString("你：我不质疑。\n轮到你继续出牌。"));
    turnState = PlayerTurn;
    updateActionButtons();
    updateBattleUI();
}