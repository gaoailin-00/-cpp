#include "mainwindow.h"
// 创建开场场景图
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
        beginImgLabel->setText("【图片缺失: image-begin.png】");
        beginImgLabel->setAlignment(Qt::AlignCenter);
        beginImgLabel->setStyleSheet("color:#ff5555; font-size:24px; font-weight:bold;");
    }
    grid->addWidget(beginImgLabel, 0, 0);

    // 右下角"进入酒馆"按钮
    QVBoxLayout* overlayLayout = new QVBoxLayout();
    overlayLayout->addStretch();

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();

    QPushButton* enterBtn = new QPushButton("进入酒馆", introScene);
    enterBtn->setFixedSize(220, 65);
    enterBtn->setStyleSheet(
        "QPushButton{"
        "font-size:24px;font-weight:bold;"
        "background:rgba(139,58,0,220);"
        "color:#ffecd2;"
        "border-radius:14px;"
        "border:3px solid #d67a2d;"
        "}"
        "QPushButton:hover{background:rgba(168,74,0,255);}"
        );

    bottomLayout->addWidget(enterBtn);
    bottomLayout->addSpacing(40);

    overlayLayout->addLayout(bottomLayout);
    overlayLayout->addSpacing(35);

    grid->addLayout(overlayLayout, 0, 0);
    stack->addWidget(introScene);

    connect(enterBtn, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(startScene);
    });
}


// 创建 startScene（标题界面）
void MainWindow::createStartScene() {
    startScene = new QWidget();
    startScene->setStyleSheet(
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #1c120d,stop:1 #3a2418);"
        );

    QVBoxLayout* layout = new QVBoxLayout(startScene);
    layout->setContentsMargins(80, 60, 80, 60);
    layout->addStretch();

    QLabel* title = new QLabel("骗子酒馆", startScene);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "QLabel{"
        "color:#efc06a;font-size:52px;font-weight:bold;"
        "background:rgba(0,0,0,120);"
        "border:3px solid #8b5a2b;"
        "border-radius:16px;padding:24px;"
        "letter-spacing:2px;"
        "}"
        );
    layout->addWidget(title, 0, Qt::AlignHCenter);

    QLabel* sub = new QLabel("西部边境的酒馆里，真话和假话一样值钱。", startScene);
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet(
        "QLabel{"
        "color:#f5e7c8;font-size:21px;"
        "background:rgba(0,0,0,100);"
        "padding:12px;border-radius:10px;"
        "}"
        );
    layout->addSpacing(18);
    layout->addWidget(sub, 0, Qt::AlignHCenter);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* startBtn = new QPushButton("开始游戏", startScene);
    QPushButton* quitBtn  = new QPushButton("退出", startScene);

    startBtn->setFixedSize(190, 62);
    quitBtn->setFixedSize(190, 62);

    startBtn->setStyleSheet(
        "QPushButton{font-size:22px;font-weight:bold;"
        "background:#9b5d26;color:white;"
        "border-radius:10px;border:2px solid #d6a25d;}"
        "QPushButton:hover{background:#b76f30;}"
        );
    quitBtn->setStyleSheet(
        "QPushButton{font-size:22px;font-weight:bold;"
        "background:#5e3523;color:white;"
        "border-radius:10px;border:2px solid #9d6a4c;}"
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

    stack->addWidget(startScene);

    connect(startBtn, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(historyScene);
    });
    connect(quitBtn, &QPushButton::clicked, this, [this]() {
        close();
    });
}

// 创建 historyScene（剧情过场）

void MainWindow::createHistoryScene() {
    historyScene = new QWidget();
    historyScene->setStyleSheet("background:#000000;");

    QGridLayout* grid = new QGridLayout(historyScene);
    grid->setContentsMargins(0, 0, 0, 0);

    QLabel* historyImageLabel = new QLabel(historyScene);
    historyImageLabel->setScaledContents(true);

    QString imgPath = QCoreApplication::applicationDirPath() + "/image-history.png";
    QPixmap pix(imgPath);
    if (!pix.isNull()) {
        historyImageLabel->setPixmap(pix);
    } else {
        historyImageLabel->setText("【图片缺失: image-history.png】");
        historyImageLabel->setAlignment(Qt::AlignCenter);
        historyImageLabel->setStyleSheet("color:#ff5555; font-size:28px; font-weight:bold;");
    }
    grid->addWidget(historyImageLabel, 0, 0);

    // 右下角"继续"按钮
    QVBoxLayout* overlayLayout = new QVBoxLayout();
    overlayLayout->addStretch();

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();

    QPushButton* continueBtn = new QPushButton("继续", historyScene);
    continueBtn->setFixedSize(180, 60);
    continueBtn->setStyleSheet(
        "QPushButton{"
        "font-size:22px;font-weight:bold;"
        "background:rgba(139,58,0,220);"
        "color:#ffecd2;"
        "border-radius:12px;"
        "border:3px solid #d67a2d;"
        "}"
        "QPushButton:hover{background:rgba(168,74,0,255);}"
        );

    bottomLayout->addWidget(continueBtn);
    bottomLayout->addSpacing(40);

    overlayLayout->addLayout(bottomLayout);
    overlayLayout->addSpacing(35);

    grid->addLayout(overlayLayout, 0, 0);
    stack->addWidget(historyScene);

    connect(continueBtn, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(ruleScene);
    });
}

// 创建 ruleScene（规则说明）

void MainWindow::createRuleScene() {
    ruleScene = new QWidget();
    ruleScene->setStyleSheet("background:#000000;");

    QGridLayout* grid = new QGridLayout(ruleScene);
    grid->setContentsMargins(0, 0, 0, 0);

    QLabel* ruleImageLabel = new QLabel(ruleScene);
    ruleImageLabel->setScaledContents(true);

    QString imgPath = QCoreApplication::applicationDirPath() + "/image-rule.png";
    QPixmap pix(imgPath);
    if (!pix.isNull()) {
        ruleImageLabel->setPixmap(pix);
    } else {
        ruleImageLabel->setText("【图片缺失: image-rule.png】");
        ruleImageLabel->setAlignment(Qt::AlignCenter);
        ruleImageLabel->setStyleSheet("color:#ff5555; font-size:28px; font-weight:bold;");
    }
    grid->addWidget(ruleImageLabel, 0, 0);

    // 右下角"开始对弈"按钮
    QVBoxLayout* overlayLayout = new QVBoxLayout();
    overlayLayout->addStretch();

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();

    QPushButton* startBattleBtn = new QPushButton("开始对弈", ruleScene);
    startBattleBtn->setFixedSize(220, 60);
    startBattleBtn->setStyleSheet(
        "QPushButton{"
        "font-size:22px;font-weight:bold;"
        "background:rgba(139,58,0,220);"
        "color:#ffecd2;"
        "border-radius:12px;"
        "border:3px solid #d67a2d;"
        "}"
        "QPushButton:hover{background:rgba(168,74,0,255);}"
        );

    bottomLayout->addWidget(startBattleBtn);
    bottomLayout->addSpacing(40);

    overlayLayout->addLayout(bottomLayout);
    overlayLayout->addSpacing(35);

    grid->addLayout(overlayLayout, 0, 0);
    stack->addWidget(ruleScene);

    connect(startBattleBtn, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(tavernScene);
    });
}


// 创建 tavernScene（酒馆大厅）

void MainWindow::createTavernScene() {
    tavernScene = new QWidget();
    tavernScene->setStyleSheet("background:#000000;");

    QGridLayout* rootGrid = new QGridLayout(tavernScene);
    rootGrid->setContentsMargins(0, 0, 0, 0);

    // 背景图
    QLabel* bgLabel = new QLabel(tavernScene);
    bgLabel->setScaledContents(true);
    QString hallImgPath = QCoreApplication::applicationDirPath() + "/image-hall.png";
    QPixmap hallPix(hallImgPath);
    if (!hallPix.isNull()) {
        bgLabel->setPixmap(hallPix);
    } else {
        bgLabel->setText("【图片缺失: image-hall.png】");
        bgLabel->setAlignment(Qt::AlignCenter);
        bgLabel->setStyleSheet("color:#ff5555; font-size:28px; font-weight:bold; background:#111;");
    }
    rootGrid->addWidget(bgLabel, 0, 0);

    // 覆盖层布局
    QVBoxLayout* overlayLayout = new QVBoxLayout();
    overlayLayout->setContentsMargins(28, 22, 28, 22);
    overlayLayout->setSpacing(16);

   
    QHBoxLayout* topLayout = new QHBoxLayout();

    QVBoxLayout* topLeftLayout = new QVBoxLayout();
    topLeftLayout->setSpacing(10);

    QLabel* title = new QLabel("酒馆大厅", tavernScene);
    title->setStyleSheet(
        "QLabel{"
        "color:#f5deb3;font-size:30px;font-weight:bold;"
        "background:rgba(0,0,0,130);"
        "padding:10px 18px;border-radius:12px;"
        "border:2px solid #80552f;"
        "}"
        );

    QLabel* tip = new QLabel("挑一个对手，把欠条一张张赢回来。", tavernScene);
    tip->setStyleSheet(
        "QLabel{"
        "color:#f0e0c2;font-size:17px;"
        "background:rgba(0,0,0,110);"
        "padding:10px 16px;border-radius:10px;"
        "}"
        );

    iouLabel = new QLabel("欠条：0 / 3", tavernScene);
    iouLabel->setStyleSheet(
        "QLabel{"
        "color:#ffd36b;font-size:22px;font-weight:bold;"
        "background:rgba(0,0,0,130);"
        "padding:10px 16px;border-radius:10px;"
        "border:2px solid #8b5a2b;"
        "}"
        );

    topLeftLayout->addWidget(title, 0, Qt::AlignLeft);
    topLeftLayout->addWidget(tip, 0, Qt::AlignLeft);

    topLayout->addLayout(topLeftLayout);
    topLayout->addStretch();
    topLayout->addWidget(iouLabel, 0, Qt::AlignTop);

    overlayLayout->addLayout(topLayout);
    overlayLayout->addStretch();

    // ===== 左下角 NPC 挑战按钮 =====
    QHBoxLayout* middleLayout = new QHBoxLayout();

    QVBoxLayout* npcBtnLayout = new QVBoxLayout();
    npcBtnLayout->setSpacing(16);

    QPushButton* rancherBtn = new QPushButton("挑战牧场主", tavernScene);
    QPushButton* sheriffBtn = new QPushButton("挑战老警长", tavernScene);
    QPushButton* bossBtn    = new QPushButton("挑战华工老板", tavernScene);

    QString npcBtnStyle =
        "QPushButton{"
        "font-size:24px;font-weight:bold;"
        "background:rgba(45,28,18,205);"
        "color:#fff5e1;"
        "border:3px solid #c28b4b;"
        "border-radius:14px;"
        "padding:10px 18px;"
        "min-width:260px;"
        "min-height:64px;"
        "}"
        "QPushButton:hover{"
        "background:rgba(88,50,28,225);"
        "}";

    rancherBtn->setStyleSheet(npcBtnStyle);
    sheriffBtn->setStyleSheet(npcBtnStyle);
    bossBtn->setStyleSheet(npcBtnStyle);

    npcBtnLayout->addWidget(rancherBtn, 0, Qt::AlignLeft);
    npcBtnLayout->addWidget(sheriffBtn, 0, Qt::AlignLeft);
    npcBtnLayout->addWidget(bossBtn, 0, Qt::AlignLeft);

    middleLayout->addSpacing(40);
    middleLayout->addLayout(npcBtnLayout);
    middleLayout->addStretch();

    overlayLayout->addLayout(middleLayout);
    overlayLayout->addSpacing(14);

    //右下角退出按钮
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();

    QPushButton* tavernBackBtn = new QPushButton("退出酒馆(认输)", tavernScene);
    tavernBackBtn->setFixedSize(180, 50);
    tavernBackBtn->setStyleSheet(
        "QPushButton{"
        "font-size:17px;font-weight:bold;"
        "background:rgba(120,35,35,220);"
        "color:white;"
        "border-radius:10px;"
        "border:2px solid #cc4a4a;"
        "}"
        "QPushButton:hover{background:rgba(155,45,45,235);}"
        );

    bottomLayout->addWidget(tavernBackBtn);
    overlayLayout->addLayout(bottomLayout);

    rootGrid->addLayout(overlayLayout, 0, 0);
    stack->addWidget(tavernScene);

    connect(rancherBtn, &QPushButton::clicked, this, [this]() { startBattle("牧场主"); });
    connect(sheriffBtn, &QPushButton::clicked, this, [this]() { startBattle("老警长"); });
    connect(bossBtn, &QPushButton::clicked, this, [this]() { startBattle("华工老板"); });
    connect(tavernBackBtn, &QPushButton::clicked, this, [this]() { goEndScene(false); });

    updateTavernInfo();
}
// 创建对弈牌桌界面

void MainWindow::createBattleScene() {
    battleScene = new QWidget();
    battleScene->setStyleSheet("background:#1c120d;");

    QVBoxLayout* mainLayout = new QVBoxLayout(battleScene);
    mainLayout->setContentsMargins(20, 10, 20, 20);
    mainLayout->setSpacing(10);

    // 标题
    battleTitleLabel = new QLabel("骗子酒馆 · 对局", battleScene);
    battleTitleLabel->setAlignment(Qt::AlignCenter);
    battleTitleLabel->setStyleSheet(
        "QLabel{color:#f3c56e;font-size:24px;font-weight:bold;"
        "background:rgba(0,0,0,80);padding:8px;border-radius:8px;"
        "border:2px solid #7b512f;}"
        );
    mainLayout->addWidget(battleTitleLabel);

    //npc
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setAlignment(Qt::AlignCenter);

    npcAvatarLabel = new QLabel(battleScene);
    npcAvatarLabel->setFixedSize(280, 280);
    npcAvatarLabel->setAlignment(Qt::AlignCenter);
    npcAvatarLabel->setStyleSheet(
        "QLabel{background:rgba(0,0,0,60);"
        "border:3px solid #7a5430;border-radius:15px;}"
        );

    npcSpeechLabel = new QLabel("……", battleScene);
    npcSpeechLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    npcSpeechLabel->setWordWrap(true);
    npcSpeechLabel->setFixedSize(500, 120);
    npcSpeechLabel->setStyleSheet(
        "QLabel{color:#fff0d2;font-size:18px;"
        "background:rgba(0,0,0,150);"
        "border:2px solid #7f5a36;border-radius:15px;"
        "padding:15px;margin-left:20px;}"
        );

    topLayout->addWidget(npcAvatarLabel);
    topLayout->addWidget(npcSpeechLabel);
    mainLayout->addLayout(topLayout);

    //桌
    QFrame* tableFrame = new QFrame(battleScene);
    tableFrame->setFixedHeight(180);
    tableFrame->setStyleSheet(
        "QFrame{"
        "background:qradialgradient(cx:0.5,cy:0.5,radius:0.8,fx:0.5,fy:0.5,"
        "stop:0 #5a341f,stop:0.6 #472918,stop:1 #2c180e);"
        "border:8px solid #1b0f09;border-radius:20px;"
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
        card->setStyleSheet(
            "QLabel{background:#6f3e23;border:2px solid #23140a;"
            "border-radius:6px;font-size:30px;color:#f8e4c0;}"
            );
        npcCardLabels.append(card);
        npcCardsLayout->addWidget(card);
    }
    tableLayout->addLayout(npcCardsLayout);

    roundInfoLabel = new QLabel("请选择操作", tableFrame);
    roundInfoLabel->setAlignment(Qt::AlignCenter);
    roundInfoLabel->setStyleSheet(
        "QLabel{color:#ffd77c;font-size:20px;font-weight:bold;"
        "background:transparent;border:none;}"
        );
    tableLayout->addWidget(roundInfoLabel);

    mainLayout->addWidget(tableFrame);

    //player
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->setAlignment(Qt::AlignBottom);

    playerAvatarLabel = new QLabel(battleScene);
    playerAvatarLabel->setFixedSize(280, 280);
    playerAvatarLabel->setAlignment(Qt::AlignCenter);
    playerAvatarLabel->setStyleSheet(
        "QLabel{background:rgba(0,0,0,60);"
        "border:3px solid #5f4635;border-radius:15px;}"
        );
    bottomLayout->addWidget(playerAvatarLabel);

    bottomLayout->addStretch();

    // 玩家手牌按钮
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

    // 操作按钮
    QVBoxLayout* actionBtnLayout = new QVBoxLayout();
    actionBtnLayout->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    actionBtnLayout->setSpacing(15);

    QString btnStyle =
        "QPushButton{"
        "font-size:20px;font-weight:bold;"
        "background:#a3612d;color:white;"
        "border-radius:12px;border:2px solid #e0ab67;"
        "padding:12px 20px;min-width:140px;"
        "}"
        "QPushButton:hover{background:#bf7638;}"
        "QPushButton:disabled{"
        "background:#5b4638;color:#c9b8aa;"
        "border:2px solid #7a6658;}";

    playCardBtn    = new QPushButton("打出选中牌", battleScene);
    challengeNpcBtn = new QPushButton("质疑 NPC",  battleScene);
    passNpcBtn     = new QPushButton("不质疑",     battleScene);
    backBtn        = new QPushButton("放弃对局",   battleScene);

    QList<QPushButton*> actionBtns =
        {playCardBtn, challengeNpcBtn, passNpcBtn, backBtn};
    for (QPushButton* b : actionBtns) {
        b->setStyleSheet(btnStyle);
        actionBtnLayout->addWidget(b);
    }
    bottomLayout->addLayout(actionBtnLayout);

    mainLayout->addLayout(bottomLayout);
    stack->addWidget(battleScene);

    connect(playCardBtn,     &QPushButton::clicked, this, &MainWindow::doPlayerPlayCard);
    connect(challengeNpcBtn, &QPushButton::clicked, this, &MainWindow::resolvePlayerChallenge);
    connect(passNpcBtn,      &QPushButton::clicked, this, &MainWindow::doPlayerPass);
    connect(backBtn,         &QPushButton::clicked, this, [this]() { goEndScene(false); });
}


// 创建 roundResultScene（单局结算）

void MainWindow::createRoundResultScene() {
    roundResultScene = new QWidget();
    roundResultScene->setStyleSheet("background:#000000;");

    QVBoxLayout* layout = new QVBoxLayout(roundResultScene);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // 上方 NPC 大图
    roundResultImageLabel = new QLabel(roundResultScene);
    roundResultImageLabel->setAlignment(Qt::AlignCenter);
    roundResultImageLabel->setScaledContents(true);
    roundResultImageLabel->setSizePolicy(
        QSizePolicy::Expanding, QSizePolicy::Expanding
        );
    layout->addWidget(roundResultImageLabel, 1);

    // 下方对话框
    QFrame* dialogFrame = new QFrame(roundResultScene);
    dialogFrame->setFixedHeight(160);
    dialogFrame->setStyleSheet(
        "QFrame{"
        "background:rgba(20,15,10,240);"
        "border-top:4px solid #d8a24a;"
        "}"
        );

    QVBoxLayout* dialogLayout = new QVBoxLayout(dialogFrame);
    dialogLayout->setContentsMargins(40, 20, 40, 20);

    roundResultTextLabel = new QLabel("...", dialogFrame);
    roundResultTextLabel->setWordWrap(true);
    roundResultTextLabel->setAlignment(Qt::AlignCenter);
    roundResultTextLabel->setStyleSheet(
        "QLabel{"
        "color:#fff0d2;font-size:26px;font-weight:bold;"
        "background:transparent;border:none;"
        "}"
        );
    dialogLayout->addWidget(roundResultTextLabel);

    layout->addWidget(dialogFrame, 0);
    stack->addWidget(roundResultScene);
}


// 创建 endScene（最终结局）

void MainWindow::createEndScene() {
    endScene = new QWidget();
    endScene->setStyleSheet("background:#000000;");

    QGridLayout* grid = new QGridLayout(endScene);
    grid->setContentsMargins(0, 0, 0, 0);

    endImageLabel = new QLabel(endScene);
    endImageLabel->setScaledContents(true);
    grid->addWidget(endImageLabel, 0, 0);

    // 按钮覆盖层
    QVBoxLayout* btnLayout = new QVBoxLayout();
    btnLayout->addStretch();

    QHBoxLayout* bottomBtns = new QHBoxLayout();
    bottomBtns->addStretch();

    QPushButton* againBtn = new QPushButton("回到大厅", endScene);
    QPushButton* exitBtn  = new QPushButton("退出游戏", endScene);

    againBtn->setFixedSize(200, 65);
    exitBtn->setFixedSize(200, 65);

    againBtn->setStyleSheet(
        "QPushButton{"
        "font-size:22px;font-weight:bold;"
        "background:rgba(155,93,38,220);color:white;"
        "border-radius:12px;border:2px solid #d7a15b;"
        "}"
        "QPushButton:hover{background:rgba(182,110,47,255);}"
        );
    exitBtn->setStyleSheet(
        "QPushButton{"
        "font-size:22px;font-weight:bold;"
        "background:rgba(139,42,42,220);color:white;"
        "border-radius:12px;border:2px solid #cc4a4a;"
        "}"
        "QPushButton:hover{background:rgba(166,56,56,255);}"
        );

    bottomBtns->addWidget(againBtn);
    bottomBtns->addSpacing(30);
    bottomBtns->addWidget(exitBtn);
    bottomBtns->addStretch();

    btnLayout->addLayout(bottomBtns);
    btnLayout->addSpacing(110);

    grid->addLayout(btnLayout, 0, 0);
    stack->addWidget(endScene);

    connect(againBtn, &QPushButton::clicked, this, [this]() {
        player.iouCount = 0;
        restartToTavern();
    });
    connect(exitBtn, &QPushButton::clicked, this, [this]() {
        close();
    });
}
