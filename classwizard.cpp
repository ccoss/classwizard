/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

//#include <QtGui>
//#include <QtDebug>

#include "classwizard.h"
#include "qtextstream.h"

extern QTranslator *tor;
//! [0] //! [1]
ClassWizard::ClassWizard(QWidget *parent)
    : QWizard(parent)
{
    m_language = false;
    m_infoPage = new IntroPage();
    partitionShow = new PartitionTool();
    userInfoPage = new UserInfoPage();
    codestyle = new CodeStylePage();

    this->addPage(m_infoPage);
    this->addPage(partitionShow);
    this->addPage( userInfoPage );
    this->addPage(codestyle);

    setWizardStyle(QWizard::ClassicStyle);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(240,248,255));
    this->setPalette(palette);

    setFixedSize(800, 550);
    //setFixedSize(800, 600);
    QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
    int width = desktop->width();
    int height = desktop->height();

    int dilwidht = 800;
    int dilheight = 550;
    int x = (width - dilwidht)/2;
    int y = (height - dilheight)/2;

    this->move(x, y);

    setWindowTitle(tr("Install system"));

    this->connect(this->button( QWizard::NextButton ), SIGNAL(clicked() ), userInfoPage, SLOT(Userinfo() ) );
    this->connect(this->button(QWizard::FinishButton), SIGNAL(clicked()), this, SLOT(partitionSaveData()));
    this->connect(this->button(QWizard::NextButton), SIGNAL(clicked()), SLOT(nextButtonCheck()));
    this->connect(this->button(QWizard::BackButton), SIGNAL(clicked()), SLOT(backButtonCheck()));
    this->connect(this->partitionShow, SIGNAL(partitedOk()), this, SLOT(partitionOk()));
    this->connect(this->partitionShow, SIGNAL(partitedNotOk()), this, SLOT(partitionNotOk()));
    this->connect(this->m_infoPage, SIGNAL(setLanguage(bool)), this, SLOT(setLanguage(bool)));
}

void ClassWizard::setLanguage(bool language)
{
    m_language = language;
    if(language)
    {
        userInfoPage->setLanguage(language);
        partitionShow->setLanguage(language);
        codestyle->setLanguage(language);
        this->button(QWizard::NextButton)->setText(tr("Next"));
        this->button(QWizard::BackButton)->setText(tr("Back"));
        this->button(QWizard::CancelButton)->setText(tr("Cancel"));
        this->button(QWizard::FinishButton)->setText(tr("Install"));
        setWindowTitle(tr("安装系统"));
    }
    else
    {
        userInfoPage->setLanguage(language);
        partitionShow->setLanguage(language);
        codestyle->setLanguage(language);
        this->button(QWizard::NextButton)->setText(tr("Next"));
        this->button(QWizard::BackButton)->setText(tr("Back"));
        this->button(QWizard::CancelButton)->setText(tr("Cancel"));
        this->button(QWizard::FinishButton)->setText(tr("Install"));
        setWindowTitle(tr("Install system"));
    }
}

void ClassWizard::partitionOk()
{
    this->button(QWizard::NextButton)->setEnabled(true);
}

void ClassWizard::partitionNotOk()
{
    this->button(QWizard::NextButton)->setEnabled(false);
}

void ClassWizard::partitionSaveData()
{
    mountinfo           inTempPartitionInfo;
    DiskPartitionInfo   getTempPartitionInfo;
    bool                isMount = false;
    int                 nPartitionNum;
    nPartitionNum = partitionShow->getPartitionNum();
    m_nCurrentPartition = 0;
    m_isAutoParted = partitionShow->getisAutoPartition();
    QString qsRevertPartition = partitionShow->getRevertPartition();
    if(qsRevertPartition.size() == 0)
    {
        qsRevertPartition = tr("NULL");
    }
    hide();
    osInstallWindow = new OsInstallWindow(userInfoPage->getUserName(), userInfoPage->getPassWord(),
                                         userInfoPage->getLanguage(), userInfoPage->getKeyBoard(),
                                         userInfoPage->getLoginModule(), userInfoPage->getTimeZone(), userInfoPage->getHostName(), m_language, qsRevertPartition);

    for(int i = 0; i < nPartitionNum; i ++)
    {
        isMount = partitionShow->getPartitioninfo(getTempPartitionInfo, i);
        if(!isMount)
            continue;
        inTempPartitionInfo.qsMainDevice = QString::fromStdString(getTempPartitionInfo.strPartitionDevice);
        inTempPartitionInfo.qsDeviceName = QString::fromStdString(getTempPartitionInfo.strPartitionName);
        inTempPartitionInfo.qsMountPoint = QString::fromStdString(getTempPartitionInfo.strMountPoint);
        inTempPartitionInfo.qsSystem = QString::fromStdString(getTempPartitionInfo.strSystemType);
        inTempPartitionInfo.isPrimary = getTempPartitionInfo.isFlag;

        osInstallWindow->getPartInfo(inTempPartitionInfo);
    }
    if(Platform == X86)
        osInstallWindow->init(m_isAutoParted);
    else if(Platform == LOONGSON)
        osInstallWindow->init(false);
        osInstallWindow->showFullScreen();
}

void ClassWizard::backButtonCheck()
{
    setLanguage(m_language);
}

void ClassWizard::nextButtonCheck()
{
    setLanguage(m_language);
    int len = partitionShow->getPartitionNum();
    if(this->currentPage()->title() == partitionShow->title())
    {
        partitionShow->clearMountPoint();
        if(Platform == X86)
        {
            this->button(QWizard::NextButton)->setEnabled(true);
        }
        else if(Platform == LOONGSON)
        {
            this->button(QWizard::NextButton)->setEnabled(false);
        }
        partitionShow->checkCanNext();
    }
    if(this->currentPage()->title() == userInfoPage->title())
    {
        qDebug() << "write page";
    }
    if(this->currentPage()->title() == codestyle->title())
    {
        QString userName = userInfoPage->getUserName();
        QString passWard = userInfoPage->getKeyBoard();
        QString keyBoard = userInfoPage->getKeyBoard();
        QString language = userInfoPage->getLanguage();
        QString loginMode = userInfoPage->getLoginModule();
        QString timeZone= userInfoPage->getTimeZone();
        QString hostName = userInfoPage->getHostName();

       codestyle->getUserName(userName);
       codestyle->getPassWard(passWard);
       codestyle->getKeyBoard(keyBoard);
       codestyle->getLanguage(language);
       codestyle->getLoginMode(loginMode);
       codestyle->getTimeMode(timeZone);
       codestyle->getHostName(hostName);

       codestyle->initializePage();
    }
}

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    this->setAutoFillBackground( true );

    QPalette palette;
    QImage img( "/etc/gaea/images/background.png" );
    palette.setBrush( QPalette::Background, img);
    this->setPalette(palette);

    selectLanguageLabel = new QLabel("<font face=宋体 size=4>Select install language</font>");
    englishPushButton = new QPushButton(tr("English"));
    chinesePushButton = new QPushButton(tr("简体中文"));
    englishPushButton->setFixedSize(200,30);
    chinesePushButton->setFixedSize(200,30);

    QObject::connect(this->chinesePushButton,SIGNAL(clicked()),this,SLOT(transCH()));
    QObject::connect(this->englishPushButton,SIGNAL(clicked()),this,SLOT(transEN()));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(26);
    layout->addWidget(selectLanguageLabel);
    layout->setSpacing(26);
    layout->addWidget(englishPushButton);
    layout->setSpacing(13);
    layout->addWidget(chinesePushButton);
    layout->addStretch();

    label = new QLabel(tr("Ready to install? Once you answer a few questions,"
                          "the contents of the liveCD can be installed on "
                          "this computer so you can run System at full speed"
                          "without the liveCD. Answering the questions should"
                          "only take a few minutes. Please choose the language"
                          "used for the configuration process. This language"
                          "will be the default language for this computer."));
    label->setWordWrap(true);
    //label1 = new QLabel();
    QVBoxLayout *layout1 = new QVBoxLayout;
    layout1->addWidget(label);
    layout1->addStretch();

    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->setContentsMargins(30,10,30,10);
    layout2->setSpacing(30);
    layout2->addLayout(layout);
    layout2->setSpacing(30);
    layout2->addLayout(layout1);
    layout2->setSpacing(30);
    transEN();
    setLayout(layout2);

    setTitle(tr("Introduction"));
    //setPixmap(QWizard::BackgroundPixmap, QPixmap("/etc/gaea/images/background.png"));
    //setPixmap(QWizard::BannerPixmap, QPixmap("/etc/gaea/images/background.png"));
}
//! [7]

void IntroPage::transCH()
{
    if (tor != NULL)
    {
        qApp->removeTranslator(tor);
        delete tor;
        tor = NULL;
        printf("dlete tor\n");
    }
    tor = new QTranslator();
    tor->load("/etc/gaea/trans/zh.qm");
    qApp->installTranslator(tor);
    setTitle(tr("Introduction"));

    label->setText(tr("Ready to install? Once you answer a few questions,"
                              "the contents of the liveCD can be installed on "
                              "this computer so you can run System at full speed"
                              "without the liveCD. Answering the questions should"
                              "only take a few minutes. Please choose the language"
                              "used for the configuration process. This language"
                              "will be the default language for this computer."));
    selectLanguageLabel->setText(tr("Select install language."));

    emit setLanguage(true);
}

void IntroPage::transEN()
{
//    QFile file("language.cfg");
//    file.open(QIODevice::WriteOnly);
//    QTextStream stream3(&file);
//    stream3 <<QString("Language: ")+QString(englishPushButton->text())<<"\n";
    if (tor != NULL)
    {
        qApp->removeTranslator(tor);
        delete tor;
        tor = NULL;
        printf("dlete tor\n");
    }
    tor = new QTranslator();
    tor->load("/etc/gaea/trans/en.qm");
    qApp->installTranslator(tor);
    setTitle(tr("Introduction"));
    //englishPushButton->setText(tr("English"));
    //chinesePushButton->setText(tr("Chinese"));

    label->setText(tr("Ready to install? Once you answer a few questions,"
                             "the contents of the liveCD can be installed on "
                             "this computer so you can run System at full speed"
                             "without the liveCD. Answering the questions should"
                             "only take a few minutes. Please choose the language"
                             "used for the configuration process. This language"
                             "will be the default language for this computer."));
    selectLanguageLabel->setText(tr("Select install language."));

     emit setLanguage(false);
}



void UserInfoPage::Userinfo()
{
    QFile file("userinfo.cfg");
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream <<QString("Username: ")+QString(userNameLineEdit->text()) <<"\n";
    stream <<QString("Hostname: ")+QString(baseClassLineEdit->text()) <<"\n";
    stream <<QString("Password: ")+QString(passwdSetLineEdit->text()) <<"\n";
    stream <<QString("Language: ")<<"\n";
    stream <<QString("Timezone: ")+QString(timezoneSelComboBox->currentText()) <<"\n";
    stream <<QString("Keyboard: ")+QString(keyboardSelComboBox->currentText()) <<"\n";

    userName = userNameLineEdit->text();
    passWord = passwdSetLineEdit->text();
    timeZone = timezoneSelComboBox->currentText();
    keyBorad = keyboardSelComboBox->currentText();
    language = languageSelComboBox->currentText();
    hostName = baseClassLineEdit->text();

    if(qobjectCtorRadioButton->isChecked())
    {
        loginModule = "Auto login";
    }
    else if(qwidgetCtorRadioButton->isChecked())
    {
        loginModule = "Login with password";
    }else
    {
        loginModule = "Login no password";
    }
    file.close();
}

//密码错误提示
bool UserInfoPage::validatePage()
{
    
    QString qsUserName = userNameLineEdit->text();
    QString qsUserOne = qsUserName.mid(0,1);
    string strUserOne = qsUserOne.toStdString();
    QMessageBox msgBox(this);
    //若为空，则弹出菜单

    if( (strUserOne.c_str()[0] >= '0') && (strUserOne.c_str()[0] <= '9'))
    {
        if(this->m_language)
        {
            msgBox.setText(tr("用户名首字母不能为数字！"));
            msgBox.addButton(tr("确定"), QMessageBox::ActionRole);
            msgBox.setWindowTitle("警告");
            msgBox.exec();
        }
        else
        {
            msgBox.setText(tr("UserName first char Can not be a num!"));
            msgBox.addButton(tr("Yes"), QMessageBox::ActionRole);
            msgBox.setWindowTitle("Warning");
            msgBox.exec();
        }
      //  QMessageBox::warning(this,
      //           tr("Warning"),
      //           tr("UserName first char Can not be a num!"),
      //           QMessageBox::Yes);
        userNameLineEdit->setFocus();
        return false;
    }
        
    if ( passwdSetLineEdit->text() != passwdCheckLineEdit->text() )
    {
        if(this->m_language)
        {
            msgBox.setText(tr("密码错误！"));
            msgBox.addButton(tr("确定"), QMessageBox::ActionRole);
            msgBox.setWindowTitle("警告");
            msgBox.exec();
        }
        else
        {
            msgBox.setText(tr("Password Error!"));
            msgBox.addButton(tr("Yes"), QMessageBox::ActionRole);
            msgBox.setWindowTitle("Warning");
            msgBox.exec();
        }
        //QMessageBox::warning(this,
        //         tr("Warning"),
        //         tr("Password Error"),
        //         QMessageBox::Yes);
        passwdSetLineEdit->setFocus();
        return false;
     }
     else
     {
         //this->nativeParentWidget();
         return true;
     }
}

//! [8] //! [9]
UserInfoPage::UserInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
//! [8]
    this->setAutoFillBackground( true );
    QPalette palette;
    QImage img( "/etc/gaea/images/background.png" );
    palette.setBrush( QPalette::Background, img);
    this->setPalette(palette);

//! [10]
    userNameLabel = new QLabel;
    userNameLineEdit = new QLineEdit;
    userNameLabel->setBuddy(userNameLineEdit);

    baseClassLabel = new QLabel;
    baseClassLineEdit = new QLineEdit;
    baseClassLabel->setBuddy(baseClassLineEdit);

    passwdSetLabel = new QLabel;
    passwdSetLineEdit = new QLineEdit;
    passwdSetLineEdit->setEchoMode(QLineEdit::Password);

    passwdCheckLabel = new QLabel;
    passwdCheckLineEdit = new QLineEdit;
    passwdCheckLineEdit->setEchoMode(QLineEdit::Password);

    timezoneSelLabel = new QLabel;
    timezoneSelComboBox = new QComboBox;
    timezoneSelComboBox->insertItem(0, "China(Shanghai)");

    keyboardSelLabel = new QLabel;
    keyboardSelComboBox = new QComboBox;
    keyboardSelComboBox->insertItem(0, "USA");
    groupBox = new QGroupBox(tr("User Setting"));

    languageSelLabel = new QLabel;
    languageSelComboBox = new QComboBox;
    languageSelComboBox->insertItem(0, "China");
    languageSelComboBox->insertItem(1, "USA");

    qobjectCtorRadioButton = new QRadioButton(tr("Auto login"));
    qwidgetCtorRadioButton = new QRadioButton(tr("Login with password"));
    //defaultCtorRadioButton = new QRadioButton(tr("Login no password"));
    qobjectCtorRadioButton->setChecked(true);

    registerField("userName*", userNameLineEdit);
    registerField("baseClass*", baseClassLineEdit);
    registerField("passwdSet*", passwdSetLineEdit);
    registerField("passwdCheck*", passwdCheckLineEdit);
//    registerField("qobjectCtor", qobjectCtorRadioButton);
//    registerField("qwidgetCtor", qwidgetCtorRadioButton);
    //registerField("defaultCtor", defaultCtorRadioButton);

    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->addWidget(qobjectCtorRadioButton);
    groupBoxLayout->addWidget(qwidgetCtorRadioButton);
    //groupBoxLayout->addWidget(defaultCtorRadioButton);
    groupBox->setLayout(groupBoxLayout);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(userNameLabel, 0, 0);
    layout->addWidget(userNameLineEdit, 0, 1);
    layout->addWidget(baseClassLabel, 1, 0);
    layout->addWidget(baseClassLineEdit, 1, 1);
    layout->addWidget(languageSelLabel, 2, 0);
    layout->addWidget(languageSelComboBox, 2, 1);
    layout->addWidget(passwdSetLabel, 3, 0);
    layout->addWidget(passwdSetLineEdit, 3, 1);
    layout->addWidget(passwdCheckLabel, 3, 2);
    layout->addWidget(passwdCheckLineEdit, 3, 3);
    layout->addWidget(timezoneSelLabel, 4, 0);
    layout->addWidget(timezoneSelComboBox, 4, 1);
    layout->addWidget(keyboardSelLabel, 4, 2);
    layout->addWidget(keyboardSelComboBox, 4, 3);
    
    QVBoxLayout *layout1 = new QVBoxLayout;
    layout1->setContentsMargins(30,10,30,10);
    layout1->addLayout(layout);
    layout1->addSpacing(26);
    layout1->addWidget(groupBox);
    layout1->addStretch();
    setLayout(layout1);
    setTitle(tr("User Information"));
}

void UserInfoPage::setLanguage(bool language)
{
    m_language = language;
    if(language)
    {
//        if (tor != NULL)
//        {
//            qApp->removeTranslator(tor);
//            delete tor;
//            tor = NULL;
//        }
//        tor = new QTranslator();
//        tor->load("/etc/gaea/trans/zh.qm");
//        qApp->installTranslator(tor);

        userNameLabel->setText(tr("User name:"));
        baseClassLabel->setText(tr("Host name:"));
        passwdSetLabel->setText(tr("Password:"));
        passwdCheckLabel->setText(tr("Password again:"));
        timezoneSelLabel->setText(tr("Time zone:"));
        timezoneSelComboBox->setItemText(0, tr("China(Shanghai)"));
        keyboardSelLabel->setText(tr("Key board:"));
        keyboardSelComboBox->setItemText(0, tr("USA"));
        groupBox->setTitle(tr("User Setting"));
        languageSelLabel->setText(tr("Language:"));
        languageSelComboBox->setItemText(0, tr("China"));
        languageSelComboBox->setItemText(1, tr("USA"));
        qobjectCtorRadioButton->setText(tr("Auto login"));
        qwidgetCtorRadioButton->setText(tr("Login with password"));
        //defaultCtorRadioButton->setText(tr("Login no password"));
        setTitle(tr("用户信息"));
    }
    else
    {
//        if (tor != NULL)
//        {
//            qApp->removeTranslator(tor);
//            delete tor;
//            tor = NULL;
//        }
//        tor = new QTranslator();
//        tor->load("/etc/gaea/trans/en.qm");
//        qApp->installTranslator(tor);

        userNameLabel->setText(tr("User name:"));
        baseClassLabel->setText(tr("Host name:"));
        passwdSetLabel->setText(tr("Password:"));
        passwdCheckLabel->setText(tr("Password again:"));
        timezoneSelLabel->setText(tr("Time zone:"));
        timezoneSelComboBox->setItemText(0, tr("China(Shanghai)"));
        keyboardSelLabel->setText(tr("Key board:"));
        keyboardSelComboBox->setItemText(0, tr("USA"));
        groupBox->setTitle(tr("User Setting"));
        languageSelLabel->setText(tr("Language:"));
        languageSelComboBox->setItemText(0, tr("China"));
        languageSelComboBox->setItemText(1, tr("USA"));
        qobjectCtorRadioButton->setText(tr("Auto login"));
        qwidgetCtorRadioButton->setText(tr("Login with password"));
        //defaultCtorRadioButton->setText(tr("Login no password"));
        setTitle(tr("User Information"));
    }
}

CodeStylePage::CodeStylePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Information Confirm"));

    this->setAutoFillBackground( true );
    QPalette palette;
    QImage img( "/etc/gaea/images/background.png" );
    palette.setBrush( QPalette::Background, img);
    this->setPalette(palette);

    subTitalLabel = new QLabel;

    infoconfirmTextBrowser = new QTextBrowser;

    registerField("infoconfirm", infoconfirmTextBrowser);

    QVBoxLayout *layout = new QVBoxLayout;

    layout->setMargin(30);
    layout->addWidget(subTitalLabel);
    layout->addSpacing(20);
    layout->addWidget(infoconfirmTextBrowser);
    layout->addSpacing(260);
    setLayout(layout);
}

void CodeStylePage::setLanguage(bool language)
{
    m_languageBool = language;
    if(language)
    {
//        if (tor != NULL)
//        {
//            qApp->removeTranslator(tor);
//            delete tor;
//            tor = NULL;
//        }
//        tor = new QTranslator();
//        tor->load("/etc/gaea/trans/zh.qm");
//        qApp->installTranslator(tor);
        subTitalLabel->setText(tr("Please confirm user information:"));
        setTitle(tr("信息确认"));
    }
    else
    {
//        if (tor != NULL)
//        {
//            qApp->removeTranslator(tor);
//            delete tor;
//            tor = NULL;
//        }
//        tor = new QTranslator();
//        tor->load("/etc/gaea/trans/en.qm");
//        qApp->installTranslator(tor);
        subTitalLabel->setText(tr("Please confirm user information:"));
        setTitle(tr("Information Confirm"));
    }
}

void CodeStylePage::initializePage()
{
    QString information;
    if(m_languageBool)
    {
       information = QString(tr("用户名: %1\n主机名: %2\n语  言: %3\n时  区: %4\n键  盘: %5")).arg(m_userName).arg(m_hostName).arg(m_language).arg(m_timeMode).arg(m_keyboard);
    }
    else
    {
        information = QString(tr("User name: %1\nHost name: %2\nLanguage: %3\nTime zone: %4\nKey board: %5")).arg(m_userName).arg(m_hostName).arg(m_language).arg(m_timeMode).arg(m_keyboard);
    }
    infoconfirmTextBrowser->setText(information);
}
//! [16]

OutputFilesPage::OutputFilesPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Install Lists"));
    setSubTitle(tr("Specify where you want the wizard to put the generated "
                   "skeleton code."));
    this->setAutoFillBackground( true );
    QPalette palette;
    QImage img( "/etc/gaea/images/background.png" );
    palette.setBrush( QPalette::Background, img);
    this->setPalette(palette);

    desktopLabel = new QLabel(tr("Basic desktop (required)"));
    extrasoftwareLabel = new QLabel(tr("Extra software (optional)"));
    databaseLabel = new QLabel(tr("Database"));
    developenvLabel = new QLabel(tr("Develop environment"));
    apache2Label = new QLabel(tr("Apache2"));
    othersLabel = new QLabel(tr("Other software"));
    databaseCheckBox = new QCheckBox;
    developenvCheckBox = new QCheckBox;
    apache2CheckBox = new QCheckBox;
    othersCheckBox = new QCheckBox;

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(desktopLabel, 0, 0);
    layout->addWidget(extrasoftwareLabel, 1, 0);
    layout->addWidget(databaseCheckBox, 2, 0);
    layout->addWidget(databaseLabel, 2, 1);
    layout->addWidget(developenvCheckBox, 3, 0);
    layout->addWidget(developenvLabel, 3, 1);
    layout->addWidget(apache2CheckBox, 4, 0);
    layout->addWidget(apache2Label, 4, 1);
    layout->addWidget(othersCheckBox, 5, 0);
    layout->addWidget(othersLabel, 5, 1);
    setLayout(layout);
}

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Conclusion"));
    this->setAutoFillBackground( true );
    QPalette palette;
    QImage img( "/etc/gaea/images/background.png" );
    palette.setBrush( QPalette::Background, img);
    this->setPalette(palette);
    //QBitmap img2(":/images/conclusion.png");
    //this->setMask(img2);


    progressBar = new QProgressBar;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(progressBar);
    layout->setMargin(30);
    layout->addStretch();
    setLayout(layout);
}
