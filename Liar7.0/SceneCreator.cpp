#include "mainwindow.h"

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

    connect(enterBtn, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(startScene);
    });
}

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

    connect(startBtn, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(historyScene);
    });
    connect(quitBtn, &QPushButton::clicked, this, [this]() {
        close();
    });
}

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

    connect(continueBtn, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(ruleScene);
    });
}

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

    connect(startBattleBtn, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(tavernScene);
    });
}

void MainWindow::createTavernScene() {
    tavernScene = new QWidget();
    tavernScene->setStyleSheet("background:#000000;");

    QGridLayout* rootGrid = new QGridLayout(tavernScene);
    rootGrid->setContentsMargins(0, 0, 0, 0);

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

    // 新增：Drinks and Food 按钮 (右上角)
    QPushButton* menuBtn = new QPushButton("Drinks and Food", tavernScene);
    menuBtn->setCursor(Qt::PointingHandCursor);
    menuBtn->setStyleSheet(
        "QPushButton{"
        "font-size:20px;font-weight:bold;"
        "background:rgba(60,20,20,220);"
        "color:#f0e0c2;"
        "border:3px solid #d8a24a;"
        "border-radius:12px;"
        "padding:10px 20px;"
        "}"
        "QPushButton:hover{background:rgba(90,30,30,240); border-color:#ffc870;}"
        );
    connect(menuBtn, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(menuScene);
    });

    topLayout->addWidget(iouLabel, 0, Qt::AlignTop);
    topLayout->addSpacing(20);
    topLayout->addWidget(menuBtn, 0, Qt::AlignTop);

    overlayLayout->addLayout(topLayout);
    overlayLayout->addStretch();

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

    connect(rancherBtn, &QPushButton::clicked, this, [this]() { startBattle("牧场主"); });
    connect(sheriffBtn, &QPushButton::clicked, this, [this]() { startBattle("老警长"); });
    connect(bossBtn, &QPushButton::clicked, this, [this]() { startBattle("华工老板"); });
    connect(tavernBackBtn, &QPushButton::clicked, this, [this]() { goEndScene(false); });

    updateTavernInfo();
}
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
        "background:rgba(0,0,0,80);padding:8px;border-radius:8px;"
        "border:2px solid #7b512f;}"
        );
    mainLayout->addWidget(battleTitleLabel);

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

    connect(playCardBtn,     &QPushButton::clicked, this, &MainWindow::doPlayerPlayCard);
    connect(challengeNpcBtn, &QPushButton::clicked, this, &MainWindow::resolvePlayerChallenge);
    connect(passNpcBtn,      &QPushButton::clicked, this, &MainWindow::doPlayerPass);
    connect(backBtn,         &QPushButton::clicked, this, [this]() { goEndScene(false); });
}

void MainWindow::createRoundResultScene() {
    roundResultScene = new QWidget();
    roundResultScene->setStyleSheet("background:#000000;");

    QVBoxLayout* layout = new QVBoxLayout(roundResultScene);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    roundResultImageLabel = new QLabel(roundResultScene);
    roundResultImageLabel->setAlignment(Qt::AlignCenter);
    roundResultImageLabel->setScaledContents(true);
    roundResultImageLabel->setSizePolicy(
        QSizePolicy::Expanding, QSizePolicy::Expanding
        );
    layout->addWidget(roundResultImageLabel, 1);

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
}

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

    connect(againBtn, &QPushButton::clicked, this, [this]() {
        player.iouCount = 0;
        restartToTavern();
    });
    connect(exitBtn, &QPushButton::clicked, this, [this]() {
        close();
    });
}

// 新增：创建点餐菜单场景
void MainWindow::createMenuScene() {
    menuScene = new QWidget();
    menuScene->setStyleSheet("background:#1a110c;");

    QVBoxLayout* mainLayout = new QVBoxLayout(menuScene);
    mainLayout->setContentsMargins(40, 20, 40, 20);

    QLabel* title = new QLabel("酒馆菜单 Drinks and Food", menuScene);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color:#f3c56e;font-size:36px;font-weight:bold;margin-bottom:10px;");
    mainLayout->addWidget(title);

    QGridLayout* grid = new QGridLayout();
    grid->setSpacing(30);

    // 循环生成 6 个菜品格子
    for (int i = 1; i <= 6; ++i) {
        QVBoxLayout* itemLayout = new QVBoxLayout();
        itemLayout->setAlignment(Qt::AlignCenter);

        QLabel* imgLabel = new QLabel(menuScene);
        imgLabel->setFixedSize(220, 220);
        imgLabel->setScaledContents(true);
        imgLabel->setStyleSheet("border:3px solid #5f4635; border-radius:10px; background:#000;");

        QString imgPath = QCoreApplication::applicationDirPath() + QString("/image-%1.png").arg(i);
        QPixmap pix(imgPath);
        if (!pix.isNull()) {
            imgLabel->setPixmap(pix);
        } else {
            imgLabel->setText(QString("缺失 image-%1.png").arg(i));
            imgLabel->setAlignment(Qt::AlignCenter);
            imgLabel->setStyleSheet("color:red; border:3px solid #5f4635; border-radius:10px; background:#000;");
        }

        // 约 8mm 高 * 2cm 宽的按钮 (在屏幕上大约 80x30 像素)
        QPushButton* orderBtn = new QPushButton("点餐", menuScene);
        orderBtn->setFixedSize(80, 30);
        orderBtn->setCursor(Qt::PointingHandCursor);
        orderBtn->setStyleSheet(
            "QPushButton{"
            "background:#8b5a2b;color:white;font-size:14px;font-weight:bold;"
            "border-radius:5px;border:1px solid #d6a25d;"
            "}"
            "QPushButton:hover{background:#a87038;}"
            );

        connect(orderBtn, &QPushButton::clicked, this, [this, i]() {
            showOrderResult(i);
        });

        itemLayout->addWidget(imgLabel, 0, Qt::AlignCenter);
        itemLayout->addSpacing(10);
        itemLayout->addWidget(orderBtn, 0, Qt::AlignCenter);

        int row = (i - 1) / 3;
        int col = (i - 1) % 3;
        grid->addLayout(itemLayout, row, col);
    }

    mainLayout->addLayout(grid);
    mainLayout->addStretch();

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();

    QPushButton* backBtn = new QPushButton("返回酒馆大厅", menuScene);
    backBtn->setFixedSize(180, 50);
    backBtn->setStyleSheet(
        "QPushButton{"
        "font-size:18px;font-weight:bold;"
        "background:rgba(120,35,35,220);color:white;"
        "border-radius:10px;border:2px solid #cc4a4a;"
        "}"
        "QPushButton:hover{background:rgba(155,45,45,235);}"
        );
    connect(backBtn, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(tavernScene);
    });

    bottomLayout->addWidget(backBtn);
    mainLayout->addLayout(bottomLayout);
}

// 新增：创建点餐结果场景
void MainWindow::createOrderResultScene() {
    orderResultScene = new QWidget();
    orderResultScene->setStyleSheet("background:#000000;");

    QVBoxLayout* layout = new QVBoxLayout(orderResultScene);
    layout->setContentsMargins(0, 0, 0, 0);

    orderResultImageLabel = new QLabel(orderResultScene);
    orderResultImageLabel->setAlignment(Qt::AlignCenter);
    orderResultImageLabel->setScaledContents(true);
    layout->addWidget(orderResultImageLabel, 1);

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();

    QPushButton* backBtn = new QPushButton("返回菜单", orderResultScene);
    backBtn->setFixedSize(180, 50);
    backBtn->setStyleSheet(
        "QPushButton{"
        "font-size:18px;font-weight:bold;"
        "background:#8b5a2b;color:white;"
        "border-radius:10px;border:2px solid #d6a25d;"
        "margin-bottom: 20px; margin-right: 20px;"
        "}"
        "QPushButton:hover{background:#a87038;}"
        );
    connect(backBtn, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(menuScene);
    });

    bottomLayout->addWidget(backBtn);
    layout->addLayout(bottomLayout);
}
