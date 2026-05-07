#include "mainwindow.h"

MainWindow::MainWindow() {
    resize(1280, 760);
    setMinimumSize(1100, 680);
    setWindowTitle("骗子酒馆 liarliar");

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    player = PlayerData("旅人");
    playerWonLastGame = false;
    turnState = PlayerTurn;

    battleMessageTimer = new QTimer(this);
    battleMessageTimer->setSingleShot(true);

    // 按顺序创建所有场景
    createIntroScene();
    createStartScene();
    createTavernScene();
    createBattleScene();
    createRoundResultScene(); // 创建单局结算场景
    createEndScene();

    // 初始显示进入酒馆前的全屏图片界面
    stack->setCurrentWidget(introScene);
}

// 真实牌库系统
void MainWindow::initCardPool() {
    cardPool.clear();
    QStringList suits = {"红桃", "黑桃", "方片", "梅花"};
    QList<int> nums = {11, 12, 13}; // 只有 J, Q, K

    // 两副牌，共 2 * 4 * 3 = 24 张牌
    for (int deck = 0; deck < 2; ++deck) {
        for (const QString& suit : suits) {
            for (int num : nums) {
                cardPool.push_back(Card(suit, num));
            }
        }
    }

    // 洗牌
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cardPool.begin(), cardPool.end(), g);
}

void MainWindow::draw5Cards(PlayerData& p) {
    p.clearCards();
    for (int i = 0; i < 5; ++i) {
        if (!cardPool.isEmpty()) {
            p.addCard(cardPool.takeLast());
        }
    }
}

void MainWindow::draw5Cards(NPCData& npc) {
    npc.clearCards();
    for (int i = 0; i < 5; ++i) {
        if (!cardPool.isEmpty()) {
            npc.addCard(cardPool.takeLast());
        }
    }
}

// NPC 台词
QString MainWindow::npcSpeak(const QString& name) {
    if (name == "牧场主") return "哼，想从我这里拿走欠条？先赢了我再说吧！";
    if (name == "老警长") return "年轻人，卡牌比枪更能解决问题，出手吧！";
    if (name == "华工老板") return "我们讲诚信，也讲义气。想拿欠条，就凭本事。";
    return "来吧，开始这场较量。";
}

QString MainWindow::getNpcImagePath(const QString& name) {
    QString basePath = QCoreApplication::applicationDirPath() + "/";
    if (name == "牧场主") return basePath + "image-muchangzhu.png";
    if (name == "老警长") return basePath + "image-laojingzhang.png";
    if (name == "华工老板") return basePath + "image-huagong.png";
    return "";
}

QString MainWindow::getNpcResultImagePath(const QString& name) {
    QString basePath = QCoreApplication::applicationDirPath() + "/";
    if (name == "牧场主") return basePath + "image-mcz.png";
    if (name == "老警长") return basePath + "image-ljz.png";
    if (name == "华工老板") return basePath + "image-hglb.png";
    return "";
}

QString MainWindow::getNpcWinDialogue(const QString& name) {
    if (name == "牧场主") return "算你狠！拿去，这是你的欠条，别让我在我的牛圈旁再看到你！";
    if (name == "老警长") return "（老警长沉默片刻，递给你一张皱巴巴的欠条）\n你的眼神比你的牌更稳，年轻人，拿走吧。";
    if (name == "华工老板") return "我们华工从不食言，年轻人，欠条给你！江湖路远，好自为之。";
    return "你赢了，欠条给你。";
}

QString MainWindow::getNpcLoseDialogue(const QString& name) {
    if (name == "牧场主") return "连牛群都骗不过的手法，也敢在我桌上耍花样？滚回老家去吧！";
    if (name == "老警长") return "小子，你的表情比牌更先露馅。在这片地界，犹豫的人总是先输。";
    if (name == "华工老板") return "做买卖讲眼力，打牌讲城府。今天这局，你火候还不够，回去练练吧。";
    return "看来你还得多练练。";
}

// 开始战斗
void MainWindow::startBattle(const QString& npcName) {
    currentNpc = NPCData(npcName);

    // 初始化牌库并分别发5张牌
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

// 胜利与失败处理逻辑
void MainWindow::handleRoundWin(const QString& message) {
    player.iouCount++;
    playerWonLastGame = true;
    turnState = BattleEnded;
    updateActionButtons();

    // 1. 先在牌桌上显示翻牌结果
    roundInfoLabel->setText(message);

    // 2. 延迟 2 秒后，跳转到全屏结算画面
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
            // 集齐3张，显示史诗级文案
            roundResultTextLabel->setText("恭喜你，幸运又狡猾的年轻人，你拿回了全部的欠条！");
            stack->setCurrentWidget(roundResultScene);

            // 停顿 3 秒后触发最终胜利结局
            battleMessageTimer->singleShot(3000, this, [this]() {
                goEndScene(true);
            });
        } else {
            // 没集齐，显示NPC交出欠条的台词
            roundResultTextLabel->setText(getNpcWinDialogue(currentNpc.name));
            stack->setCurrentWidget(roundResultScene);

            // 停顿 3 秒后返回大厅
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

    // 1. 先在牌桌上显示翻牌结果
    roundInfoLabel->setText(message);

    // 2. 延迟 2 秒后，跳转到全屏结算画面
    QTimer::singleShot(2000, this, [this]() {
        QString imgPath = getNpcResultImagePath(currentNpc.name);
        QPixmap pix(imgPath);
        if (!pix.isNull()) {
            roundResultImageLabel->setPixmap(pix);
            roundResultImageLabel->setText("");
        } else {
            roundResultImageLabel->setText("【图片缺失: " + imgPath + "】");
        }

        // 显示NPC嘲讽台词
        roundResultTextLabel->setText(getNpcLoseDialogue(currentNpc.name));
        stack->setCurrentWidget(roundResultScene);

        // 停顿 3 秒后返回大厅
        battleMessageTimer->singleShot(3000, this, [this]() {
            restartToTavern();
        });
    });
}

// 玩家出牌
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
    roundInfoLabel->setText(QString("你打出了 %1 张牌，并声称它们都是 Q。\n%2 正在观察你的表情……")
                                .arg(selected.size()).arg(currentNpc.name));

    bool npcChallenge = (QRandomGenerator::global()->bounded(100) < 35);

    if (npcChallenge) {
        battleMessageTimer->singleShot(1400, this, &MainWindow::resolveNpcChallenge);
    } else {
        battleMessageTimer->singleShot(1400, this, [this]() {
            if (player.cardCount() == 0) {
                handleRoundWin(QString("%1没有质疑，而你已经出完全部手牌。\n你赢下了这局！").arg(currentNpc.name));
            } else {
                roundInfoLabel->setText(QString("%1：我不质疑。\n现在轮到我出牌了。").arg(currentNpc.name));
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

// NPC出牌
void MainWindow::doNpcPlayCard() {
    if (turnState != NpcTurn) return;

    if (currentNpc.cardCount() <= 0) {
        handleRoundLose(QString("%1已经出完手牌。\n你错过了上一次质疑机会，这局判你失败。").arg(currentNpc.name));
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

    roundInfoLabel->setText(QString("%1打出了 %2 张牌，并声称它们都是 Q。\n你要质疑吗？")
                                .arg(currentNpc.name).arg(playCount));

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
        handleRoundLose(QString("你没有质疑，而 %1 已经出完了全部手牌。\n这局你输了。").arg(currentNpc.name));
        return;
    }

    roundInfoLabel->setText(QString("你：我不质疑。\n轮到你继续出牌。"));
    turnState = PlayerTurn;
    updateActionButtons();
    updateBattleUI();
}
// 场景切换与更新
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

    battleTitleLabel->setText(QString("骗子酒馆 · 正在与 %1 对局").arg(currentNpc.name));

    // 更新 NPC 头像
    if (npcAvatarLabel) {
        QString npcImgPath = getNpcImagePath(currentNpc.name);
        QPixmap npcPix(npcImgPath);
        if (!npcPix.isNull()) {
            npcAvatarLabel->setPixmap(npcPix.scaled(280, 280, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            npcAvatarLabel->setText("");
        } else {
            npcAvatarLabel->setText("图片缺失:\n" + currentNpc.name);
        }
    }

    // 更新 玩家 头像
    if (playerAvatarLabel) {
        QString playerImgPath = QCoreApplication::applicationDirPath() + "/image-wanjia.png";
        QPixmap playerPix(playerImgPath);
        if (!playerPix.isNull()) {
            playerAvatarLabel->setPixmap(playerPix.scaled(280, 280, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            playerAvatarLabel->setText("");
        } else {
            playerAvatarLabel->setText("图片缺失:\nimage-wanjia.png");
        }
    }

    // NPC 手牌：只显示背面数量
    for (int i = 0; i < npcCardLabels.size(); ++i) {
        if (i < currentNpc.cardCount()) {
            npcCardLabels[i]->show();
            npcCardLabels[i]->setText("🂠");
        } else {
            npcCardLabels[i]->hide();
        }
    }

    // 玩家手牌：直接显示正面，可点击选择
    for (int i = 0; i < playerCardBtns.size(); ++i) {
        if (i < player.cardCount()) {
            playerCardBtns[i]->show();

            const Card& c = player.handcards[i];
            QString color = c.isRed() ? "#b40000" : "#111111";
            QString normalStyle =
                QString(
                    "QPushButton{"
                    "background:qlineargradient(x1:0,y1:0,x2:0,y2:1,"
                    "stop:0 #fffdf7, stop:1 #ece7dc);"
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
                    "stop:0 #fff6d8, stop:1 #f0dfaa);"
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

    // 纯净结局：根据输赢全屏加载图片
    QString endImgPath = QCoreApplication::applicationDirPath() + (playerWonLastGame ? "/image-win.png" : "/image-lose.png");
    QPixmap endPix(endImgPath);
    if (!endPix.isNull()) {
        endImageLabel->setPixmap(endPix);
        endImageLabel->setText("");
    } else {
        endImageLabel->setText(playerWonLastGame ? "【图片缺失: image-win.png】" : "【图片缺失: image-lose.png】");
        endImageLabel->setStyleSheet("color:#ff5555; font-size:30px; font-weight:bold; background:black;");
    }
}

// 创建初始全屏图片场景
void MainWindow::createIntroScene() {
    introScene = new QWidget();
    introScene->setStyleSheet("background:#0a0a0a;");

    QGridLayout* grid = new QGridLayout(introScene);
    grid->setContentsMargins(0, 0, 0, 0);

    QLabel* beginImgLabel = new QLabel(introScene);
    beginImgLabel->setScaledContents(true);
    QString imgPath = QCoreApplication::applicationDirPath() + "/image-begin.png";
    QPixmap pix(imgPath);
    if (!pix.isNull()) {
        beginImgLabel->setPixmap(pix);
    } else {
        beginImgLabel->setText("【图片缺失: image-begin.png】\n请将图片放在exe同级目录");
        beginImgLabel->setAlignment(Qt::AlignCenter);
        beginImgLabel->setStyleSheet("color:#ff5555; font-size:24px; font-weight:bold;");
    }
    grid->addWidget(beginImgLabel, 0, 0);

    QVBoxLayout* btnLayout = new QVBoxLayout();
    btnLayout->addStretch();

    QPushButton* enterBtn = new QPushButton("进入酒馆", introScene);
    enterBtn->setFixedSize(240, 70);
    enterBtn->setStyleSheet(
        "QPushButton{font-size:26px;font-weight:bold;background:rgba(139,58,0,220);color:#ffecd2;border-radius:15px;border:3px solid #d67a2d;}"
        "QPushButton:hover{background:rgba(168,74,0,255);}"
        );
    btnLayout->addWidget(enterBtn, 0, Qt::AlignHCenter);
    btnLayout->addSpacing(60);

    grid->addLayout(btnLayout, 0, 0);
    stack->addWidget(introScene);

    connect(enterBtn, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(startScene);
    });
}

// 创建开始场景
void MainWindow::createStartScene() {
    startScene = new QWidget();
    startScene->setStyleSheet(
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #1c120d, stop:1 #3a2418);"
        );

    QVBoxLayout* layout = new QVBoxLayout(startScene);
    layout->setContentsMargins(80, 60, 80, 60);
    layout->addStretch();

    QLabel* title = new QLabel("骗子酒馆", startScene);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "QLabel{"
        "color:#efc06a; font-size:52px; font-weight:bold;"
        "background:rgba(0,0,0,120); border:3px solid #8b5a2b;"
        "border-radius:16px; padding:24px; letter-spacing:2px;"
        "}"
        );
    layout->addWidget(title, 0, Qt::AlignHCenter);

    QLabel* sub = new QLabel("西部边境的酒馆里，真话和假话一样值钱。", startScene);
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("QLabel{color:#f5e7c8; font-size:21px; background:rgba(0,0,0,100); padding:12px; border-radius:10px;}");
    layout->addSpacing(18);
    layout->addWidget(sub, 0, Qt::AlignHCenter);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* startBtn = new QPushButton("开始游戏", startScene);
    QPushButton* quitBtn = new QPushButton("退出", startScene);

    startBtn->setFixedSize(190, 62);
    quitBtn->setFixedSize(190, 62);

    startBtn->setStyleSheet(
        "QPushButton{font-size:22px;font-weight:bold;background:#9b5d26;color:white;border-radius:10px;border:2px solid #d6a25d;}"
        "QPushButton:hover{background:#b76f30;}"
        );
    quitBtn->setStyleSheet(
        "QPushButton{font-size:22px;font-weight:bold;background:#5e3523;color:white;border-radius:10px;border:2px solid #9d6a4c;}"
        "QPushButton:hover{background:#774533;}"
        );

    btnLayout->addStretch();
    btnLayout->addWidget(startBtn);
    btnLayout->addSpacing(30);
    btnLayout->addWidget(quitBtn);
    btnLayout->addStretch();

    layout->addSpacing(35);
    layout->addLayout(btnLayout);
    layout->addStretch();

    connect(startBtn, &QPushButton::clicked, this, [this]() { stack->setCurrentWidget(tavernScene); });
    connect(quitBtn, &QPushButton::clicked, this, [this]() { close(); });

    stack->addWidget(startScene);
}

// 创建酒馆大厅
void MainWindow::createTavernScene() {
    tavernScene = new QWidget();
    tavernScene->setStyleSheet(
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #24160f, stop:1 #4a2d1e);"
        );

    QVBoxLayout* mainLayout = new QVBoxLayout(tavernScene);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    QHBoxLayout* topLayout = new QHBoxLayout();
    QLabel* title = new QLabel("酒馆大厅 · 选择你的对手", tavernScene);
    title->setStyleSheet(
        "QLabel{color:#f5deb3;font-size:30px;font-weight:bold;"
        "background:rgba(0,0,0,110);padding:12px;border-radius:12px;border:2px solid #80552f;}"
        );

    iouLabel = new QLabel("欠条：0 / 3", tavernScene);
    iouLabel->setStyleSheet(
        "QLabel{color:#ffd36b;font-size:22px;font-weight:bold;"
        "background:rgba(0,0,0,120);padding:10px;border-radius:10px;border:2px solid #8b5a2b;}"
        );

    topLayout->addWidget(title);
    topLayout->addStretch();
    topLayout->addWidget(iouLabel);
    mainLayout->addLayout(topLayout);

    mainLayout->addSpacing(30);

    QLabel* tip = new QLabel("酒馆的橡木桌边已经坐着三个人。每个人都在笑，但没人会先说真话。", tavernScene);
    tip->setWordWrap(true);
    tip->setAlignment(Qt::AlignCenter);
    tip->setStyleSheet("QLabel{color:#f0e0c2;font-size:18px;background:rgba(0,0,0,80);padding:14px;border-radius:10px;}");
    mainLayout->addWidget(tip);

    mainLayout->addStretch();

    QHBoxLayout* npcBtnLayout = new QHBoxLayout();

    QPushButton* rancherBtn = new QPushButton("挑战牧场主", tavernScene);
    QPushButton* sheriffBtn = new QPushButton("挑战老警长", tavernScene);
    QPushButton* bossBtn = new QPushButton("挑战华工老板", tavernScene);

    QList<QPushButton*> btns = {rancherBtn, sheriffBtn, bossBtn};
    for (auto b : btns) {
        b->setFixedSize(220, 80);
        b->setStyleSheet(
            "QPushButton{font-size:24px;font-weight:bold;background:rgba(72,42,24,220);color:#fff5e1;"
            "border:3px solid #c28b4b;border-radius:14px;}"
            "QPushButton:hover{background:rgba(105,58,31,235);}"
            );
        npcBtnLayout->addWidget(b);
        npcBtnLayout->addSpacing(24);
    }

    mainLayout->addLayout(npcBtnLayout);
    mainLayout->addStretch();

    // 退出酒馆按钮（主动退出直接判负）
    QPushButton* tavernBackBtn = new QPushButton("退出酒馆 (认输)", tavernScene);
    tavernBackBtn->setFixedSize(200, 54);
    tavernBackBtn->setStyleSheet(
        "QPushButton{font-size:18px;background:#8b2a2a;color:white;border-radius:8px;border:2px solid #cc4a4a;}"
        "QPushButton:hover{background:#a63838;}"
        );
    mainLayout->addWidget(tavernBackBtn, 0, Qt::AlignHCenter);

    stack->addWidget(tavernScene);

    connect(rancherBtn, &QPushButton::clicked, this, [this]() { startBattle("牧场主"); });
    connect(sheriffBtn, &QPushButton::clicked, this, [this]() { startBattle("老警长"); });
    connect(bossBtn, &QPushButton::clicked, this, [this]() { startBattle("华工老板"); });
    connect(tavernBackBtn, &QPushButton::clicked, this, [this]() {
        goEndScene(false); // 主动退出，触发失败结局
    });

    updateTavernInfo();
}

// 创建战斗场景
void MainWindow::createBattleScene() {
    battleScene = new QWidget();
    battleScene->setStyleSheet("background:#1c120d;");

    QVBoxLayout* mainLayout = new QVBoxLayout(battleScene);
    mainLayout->setContentsMargins(20, 10, 20, 20);
    mainLayout->setSpacing(10);

    battleTitleLabel = new QLabel("骗子酒馆 · 对局", battleScene);
    battleTitleLabel->setAlignment(Qt::AlignCenter);
    battleTitleLabel->setStyleSheet(
        "QLabel{color:#f3c56e;font-size:24px;font-weight:bold;"
        "background:rgba(0,0,0,80);padding:8px;border-radius:8px;border:2px solid #7b512f;}"
        );
    mainLayout->addWidget(battleTitleLabel);

    // NPC
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setAlignment(Qt::AlignCenter);

    npcAvatarLabel = new QLabel(battleScene);
    npcAvatarLabel->setFixedSize(280, 280);
    npcAvatarLabel->setAlignment(Qt::AlignCenter);
    npcAvatarLabel->setStyleSheet("QLabel{background:rgba(0,0,0,60);border:3px solid #7a5430;border-radius:15px;}");

    npcSpeechLabel = new QLabel("……", battleScene);
    npcSpeechLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    npcSpeechLabel->setWordWrap(true);
    npcSpeechLabel->setFixedSize(500, 120);
    npcSpeechLabel->setStyleSheet(
        "QLabel{color:#fff0d2;font-size:18px;background:rgba(0,0,0,150);"
        "border:2px solid #7f5a36;border-radius:15px;padding:15px; margin-left: 20px;}"
        );

    topLayout->addWidget(npcAvatarLabel);
    topLayout->addWidget(npcSpeechLabel);
    mainLayout->addLayout(topLayout);

    //牌桌
    QFrame* tableFrame = new QFrame(battleScene);
    tableFrame->setFixedHeight(180);
    tableFrame->setStyleSheet(
        "QFrame{"
        "background:qradialgradient(cx:0.5, cy:0.5, radius:0.8, fx:0.5, fy:0.5,"
        "stop:0 #5a341f, stop:0.6 #472918, stop:1 #2c180e);"
        "border:8px solid #1b0f09; border-radius:20px;"
        "}"
        );
    QVBoxLayout* tableLayout = new QVBoxLayout(tableFrame);

    QHBoxLayout* npcCardsLayout = new QHBoxLayout();
    npcCardsLayout->setAlignment(Qt::AlignCenter);
    npcCardLabels.clear();
    for (int i = 0; i < 5; ++i) {
        QLabel* card = new QLabel("🂠", tableFrame);
        card->setFixedSize(60, 85);
        card->setAlignment(Qt::AlignCenter);
        card->setStyleSheet("QLabel{background:#6f3e23;border:2px solid #23140a;border-radius:6px;font-size:30px;color:#f8e4c0;}");
        npcCardLabels.append(card);
        npcCardsLayout->addWidget(card);
    }
    tableLayout->addLayout(npcCardsLayout);

    roundInfoLabel = new QLabel("请选择操作", tableFrame);
    roundInfoLabel->setAlignment(Qt::AlignCenter);
    roundInfoLabel->setStyleSheet("QLabel{color:#ffd77c;font-size:20px;font-weight:bold; background:transparent; border:none;}");
    tableLayout->addWidget(roundInfoLabel);

    mainLayout->addWidget(tableFrame);
//以下是玩家
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->setAlignment(Qt::AlignBottom);

    playerAvatarLabel = new QLabel(battleScene);
    playerAvatarLabel->setFixedSize(280, 280);
    playerAvatarLabel->setAlignment(Qt::AlignCenter);
    playerAvatarLabel->setStyleSheet("QLabel{background:rgba(0,0,0,60);border:3px solid #5f4635;border-radius:15px;}");
    bottomLayout->addWidget(playerAvatarLabel);

    bottomLayout->addStretch();

    QHBoxLayout* playerCardsLayout = new QHBoxLayout();
    playerCardsLayout->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    playerCardsLayout->setSpacing(15);
    playerCardBtns.clear();
    for (int i = 0; i < 5; ++i) {
        QPushButton* btn = new QPushButton(battleScene);
        btn->setFixedSize(100, 145);
        btn->setCheckable(true);
        btn->setCursor(Qt::PointingHandCursor);
        playerCardBtns.append(btn);
        playerCardsLayout->addWidget(btn);
    }
    bottomLayout->addLayout(playerCardsLayout);

    bottomLayout->addStretch();

    QVBoxLayout* actionBtnLayout = new QVBoxLayout();
    actionBtnLayout->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    actionBtnLayout->setSpacing(15);

    QString btnStyle =
        "QPushButton{"
        "font-size:20px; font-weight:bold; background:#a3612d; color:white;"
        "border-radius:12px; border:2px solid #e0ab67; padding:12px 20px; min-width: 140px;"
        "}"
        "QPushButton:hover{background:#bf7638;}"
        "QPushButton:disabled{background:#5b4638;color:#c9b8aa;border:2px solid #7a6658;}";

    playCardBtn = new QPushButton("打出选中牌", battleScene);
    challengeNpcBtn = new QPushButton("质疑 NPC", battleScene);
    passNpcBtn = new QPushButton("不质疑", battleScene);
    backBtn = new QPushButton("放弃对局", battleScene);

    QList<QPushButton*> actionBtns = {playCardBtn, challengeNpcBtn, passNpcBtn, backBtn};
    for (QPushButton* b : actionBtns) {
        b->setStyleSheet(btnStyle);
        actionBtnLayout->addWidget(b);
    }
    bottomLayout->addLayout(actionBtnLayout);

    mainLayout->addLayout(bottomLayout);
    stack->addWidget(battleScene);

    connect(playCardBtn, &QPushButton::clicked, this, &MainWindow::doPlayerPlayCard);
    connect(challengeNpcBtn, &QPushButton::clicked, this, &MainWindow::resolvePlayerChallenge);
    connect(passNpcBtn, &QPushButton::clicked, this, &MainWindow::doPlayerPass);
    connect(backBtn, &QPushButton::clicked, this, [this]() {
        goEndScene(false); // 战斗中途退出，直接判负
    });
}

// 创建单局结算场景 (新增)
void MainWindow::createRoundResultScene() {
    roundResultScene = new QWidget();
    roundResultScene->setStyleSheet("background:#000000;");

    QVBoxLayout* layout = new QVBoxLayout(roundResultScene);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // 上半部分：NPC专属大图
    roundResultImageLabel = new QLabel(roundResultScene);
    roundResultImageLabel->setAlignment(Qt::AlignCenter);
    roundResultImageLabel->setScaledContents(true); // 铺满区域
    roundResultImageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(roundResultImageLabel, 1); // 占据主要空间

    // 下半部分：对话框
    QFrame* dialogFrame = new QFrame(roundResultScene);
    dialogFrame->setFixedHeight(160);
    dialogFrame->setStyleSheet(
        "QFrame{"
        "background:rgba(20, 15, 10, 240);"
        "border-top: 4px solid #d8a24a;"
        "}"
        );

    QVBoxLayout* dialogLayout = new QVBoxLayout(dialogFrame);
    dialogLayout->setContentsMargins(40, 20, 40, 20);

    roundResultTextLabel = new QLabel("...", dialogFrame);
    roundResultTextLabel->setWordWrap(true);
    roundResultTextLabel->setAlignment(Qt::AlignCenter);
    roundResultTextLabel->setStyleSheet(
        "QLabel{"
        "color:#fff0d2; font-size:26px; font-weight:bold;"
        "background:transparent; border:none; line-height: 1.5;"
        "}"
        );
    dialogLayout->addWidget(roundResultTextLabel);

    layout->addWidget(dialogFrame, 0); // 固定高度在底部

    stack->addWidget(roundResultScene);
}

// 创建结局场景
void MainWindow::createEndScene() {
    endScene = new QWidget();
    endScene->setStyleSheet("background:#000000;");

    QGridLayout* grid = new QGridLayout(endScene);
    grid->setContentsMargins(0, 0, 0, 0);

    endImageLabel = new QLabel(endScene);
    endImageLabel->setScaledContents(true);
    grid->addWidget(endImageLabel, 0, 0);

    QVBoxLayout* btnLayout = new QVBoxLayout();
    btnLayout->addStretch();

    QHBoxLayout* bottomBtns = new QHBoxLayout();
    bottomBtns->addStretch();

    QPushButton* againBtn = new QPushButton("重新开始", endScene);
    QPushButton* exitBtn = new QPushButton("结束游戏", endScene);

    againBtn->setFixedSize(200, 65);
    exitBtn->setFixedSize(200, 65);

    againBtn->setStyleSheet(
        "QPushButton{font-size:22px;font-weight:bold;background:rgba(155,93,38,220);color:white;border-radius:12px;border:2px solid #d7a15b;}"
        "QPushButton:hover{background:rgba(182,110,47,255);}"
        );
    exitBtn->setStyleSheet(
        "QPushButton{font-size:22px;font-weight:bold;background:rgba(139,42,42,220);color:white;border-radius:12px;border:2px solid #cc4a4a;}"
        "QPushButton:hover{background:rgba(166,56,56,255);}"
        );

    bottomBtns->addWidget(againBtn);
    bottomBtns->addSpacing(30);
    bottomBtns->addWidget(exitBtn);
    bottomBtns->addStretch();

    btnLayout->addLayout(bottomBtns);
    btnLayout->addSpacing(50);

    grid->addLayout(btnLayout, 0, 0);
    stack->addWidget(endScene);

    connect(againBtn, &QPushButton::clicked, this, [this]() {
        player.iouCount = 0; // 重新开始时清空欠条
        restartToTavern();
    });
    connect(exitBtn, &QPushButton::clicked, this, [this]() { close(); });
}