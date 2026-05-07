#include "mainwindow.h"

void MainWindow::goEndScene(bool win) {
    playerWonLastGame = win;
    updateEndScene();
    stack->setCurrentWidget(endScene);
}

void MainWindow::restartToTavern() {
    updateTavernInfo();
    stack->setCurrentWidget(tavernScene);
}

void MainWindow::updateTavernInfo() {
    if (!iouLabel) return;
    iouLabel->setText(QString("欠条：%1 / 3").arg(player.iouCount));
}

void MainWindow::updateActionButtons() {
    if (!playCardBtn || !challengeNpcBtn || !passNpcBtn || !backBtn) return;

    playCardBtn->hide();
    challengeNpcBtn->hide();
    passNpcBtn->hide();
    backBtn->show();

    switch (turnState) {
    case PlayerTurn:
        playCardBtn->show();
        playCardBtn->setEnabled(true);
        break;
    case WaitingNpcChallenge:
        break;
    case NpcTurn:
        break;
    case WaitingPlayerDecision:
        challengeNpcBtn->show();
        passNpcBtn->show();
        break;
    case BattleEnded:
        break;
    }
}

void MainWindow::updateBattleUI() {
    if (!battleTitleLabel) return;

    battleTitleLabel->setText(
        QString("骗子酒馆 · 正在与 %1 对局").arg(currentNpc.name)
        );

    if (npcAvatarLabel) {
        QString npcImgPath = getNpcImagePath(currentNpc.name);
        QPixmap npcPix(npcImgPath);
        if (!npcPix.isNull()) {
            npcAvatarLabel->setPixmap(
                npcPix.scaled(280, 280, Qt::KeepAspectRatio, Qt::SmoothTransformation)
                );
            npcAvatarLabel->setText("");
        } else {
            npcAvatarLabel->setText("图片缺失:\n" + currentNpc.name);
        }
    }

    if (playerAvatarLabel) {
        QString playerImgPath =
            QCoreApplication::applicationDirPath() + "/image-wanjia.png";
        QPixmap playerPix(playerImgPath);
        if (!playerPix.isNull()) {
            playerAvatarLabel->setPixmap(
                playerPix.scaled(280, 280, Qt::KeepAspectRatio, Qt::SmoothTransformation)
                );
            playerAvatarLabel->setText("");
        } else {
            playerAvatarLabel->setText("图片缺失:\nimage-wanjia.png");
        }
    }

    for (int i = 0; i < npcCardLabels.size(); ++i) {
        if (i < currentNpc.cardCount()) {
            npcCardLabels[i]->show();
            npcCardLabels[i]->setText("🂠");
        } else {
            npcCardLabels[i]->hide();
        }
    }

    for (int i = 0; i < playerCardBtns.size(); ++i) {
        if (i < player.cardCount()) {
            playerCardBtns[i]->show();

            const Card& c = player.handcards[i];
            QString color = c.isRed() ? "#b40000" : "#111111";
            QString normalStyle = QString(
                                      "QPushButton{"
                                      "background:qlineargradient(x1:0,y1:0,x2:0,y2:1,"
                                      "stop:0 #fffdf7,stop:1 #ece7dc);"
                                      "border:2px solid #3c2a1d;"
                                      "border-radius:10px;"
                                      "color:%1;"
                                      "font-size:24px;"
                                      "font-weight:bold;"
                                      "padding:6px;"
                                      "}"
                                      "QPushButton:hover{"
                                      "border:2px solid #d8a24a;"
                                      "}"
                                      "QPushButton:checked{"
                                      "background:qlineargradient(x1:0,y1:0,x2:0,y2:1,"
                                      "stop:0 #fff6d8,stop:1 #f0dfaa);"
                                      "border:3px solid #e0a12b;"
                                      "margin-top:-15px;"
                                      "}"
                                      ).arg(color);

            playerCardBtns[i]->setStyleSheet(normalStyle);
            playerCardBtns[i]->setText(QString("%1\n%2").arg(c.getSuitSymbol(), c.getNumText()));
            playerCardBtns[i]->setChecked(false);
        } else {
            playerCardBtns[i]->hide();
            playerCardBtns[i]->setChecked(false);
        }
    }
}

void MainWindow::updateEndScene() {
    if (!endImageLabel) return;

    QString endImgPath = QCoreApplication::applicationDirPath()
                         + (playerWonLastGame ? "/image-win.png" : "/image-lose.png");
    QPixmap endPix(endImgPath);
    if (!endPix.isNull()) {
        endImageLabel->setPixmap(endPix);
        endImageLabel->setText("");
    } else {
        endImageLabel->setText(
            playerWonLastGame
                ? "【图片缺失: image-win.png】"
                : "【图片缺失: image-lose.png】"
            );
        endImageLabel->setStyleSheet(
            "color:#ff5555; font-size:30px; font-weight:bold; background:black;"
            );
    }
}