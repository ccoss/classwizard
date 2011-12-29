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

#ifndef CLASSWIZARD_H
#define CLASSWIZARD_H

//#include <QWizard>
#include <qtextstream.h>
#include <qfile.h>

#include "partitionwizard.h"
#define MAXPARTNUM 32

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QRadioButton;
class QTextBrowser;
class QProgressBar;
class QTableWidget;
class QTextStream;
class QFile;
class QPalette;
class PartitionInfo;
QT_END_NAMESPACE

class PartitionTool;
class CodeStylePage;
class UserInfoPage;
class IntroPage;

class ClassWizard : public QWizard
{
    Q_OBJECT
public:
    ClassWizard(QWidget *parent = 0);
private:
   PartitionTool*   partitionShow;
   CodeStylePage*   codestyle;
   OsInstallWindow* osInstallWindow;
   UserInfoPage*    userInfoPage;
   IntroPage*        m_infoPage;
   mountinfo        wizardMountInfo[MAXPARTNUM];
   int              m_nCurrentPartition;
   bool             m_isAutoParted;
   bool             m_language;

public slots:
   void partitionSaveData();
   void nextButtonCheck();
   void backButtonCheck();
   void partitionOk();
   void partitionNotOk();
   void setLanguage(bool language);
};
//! [0]

//! [1]
class IntroPage : public QWizardPage
{
    Q_OBJECT
public:
    IntroPage(QWidget *parent = 0);

private slots:
    void transCH();
    void transEN();

private:
    QLabel *selectLanguageLabel;
    QPushButton *englishPushButton;
    QPushButton *chinesePushButton;
    QLabel *label;
    QLabel *label1;
    //QPalette pal;

    //const QPixmap a;
signals:
    void explains();
    void setLanguage(bool language);
};


//! [2]
class UserInfoPage : public QWizardPage
{
    Q_OBJECT
public:
    UserInfoPage(QWidget *parent = 0);
    void setLanguage(bool language);
    virtual bool validatePage();
    QString getUserName() const
    {
        return userName;
    }
    QString getPassWord() const
    {
        return passWord;
    }
    QString getTimeZone() const
    {
        return timeZone;
    }
    QString getKeyBoard() const
    {
        return keyBorad;
    }
    QString getLoginModule() const
    {
        return loginModule;
    }
    QString getLanguage() const
    {
        return language;
    }
    QString getHostName() const
    {
        return hostName;
    }

private:
    QLabel          *userNameLabel;
    QLabel          *baseClassLabel;
    QLabel          *passwdSetLabel;
    QLabel          *passwdCheckLabel;
    QLabel          *timezoneSelLabel;
    QLabel          *keyboardSelLabel;
    QLabel          *languageSelLabel;
    QLineEdit       *userNameLineEdit;
    QLineEdit       *baseClassLineEdit;
    QLineEdit       *passwdSetLineEdit;
    QLineEdit       *passwdCheckLineEdit;
    QComboBox       *timezoneSelComboBox;
    QComboBox       *keyboardSelComboBox;
    QComboBox       *languageSelComboBox;
    QCheckBox       *qobjectMacroCheckBox;
    QGroupBox       *groupBox;
    QRadioButton    *qobjectCtorRadioButton;
    QRadioButton    *qwidgetCtorRadioButton;
    //QRadioButton    *defaultCtorRadioButton;
    QCheckBox       *copyCtorCheckBox;
    bool            m_language;

    QString         userName;
    QString         passWord;
    QString         timeZone;
    QString         keyBorad;
    QString         loginModule;
    QString         language;
    QString         hostName;

private slots:
   void Userinfo();

};
//! [2]

//! [3]
class CodeStylePage : public QWizardPage
{
    Q_OBJECT

public:
    CodeStylePage(QWidget *parent = 0);

    void setLanguage(bool language);

    void getUserName(QString &userName)
    {
        m_userName = userName;
    }

    void getPassWard(QString &userPassWard)
    {
        m_userPassword = userPassWard;
    }

    void getLanguage(QString &language)
    {
        m_language = language;
    }

    void getKeyBoard(QString &keyBoard)
    {
        m_keyboard = keyBoard;
    }

    void getLoginMode(QString &loginMode)
    {
        m_loginMode = loginMode;
    }

    void getTimeMode(QString &timeMode)
    {
        m_timeMode = timeMode;
    }

    void getHostName(QString &hostName)
    {
        m_hostName = hostName;
    }

    void initializePage();
private:
    QLabel          *subTitalLabel;
    QString         m_userName;
    QString         m_hostName;
    QString         m_userPassword;
    QString         m_language;
    QString         m_keyboard;
    QString         m_loginMode;
    QString         m_timeMode;
    QTextBrowser *infoconfirmTextBrowser;
    bool            m_languageBool;
};

class OutputFilesPage : public QWizardPage
{
    Q_OBJECT

public:    

    OutputFilesPage(QWidget *parent = 0);

//protected:
//    void initializePage();

private:

    QLabel      *desktopLabel;
    QLabel      *extrasoftwareLabel;
    QLabel      *databaseLabel;
    QLabel      *developenvLabel;
    QLabel      *apache2Label;
    QLabel      *othersLabel;

    QCheckBox   *databaseCheckBox;
    QCheckBox   *developenvCheckBox;
    QCheckBox   *apache2CheckBox;
    QCheckBox   *othersCheckBox;
};

class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:

    ConclusionPage(QWidget *parent = 0);



private:

    QProgressBar *progressBar;


};

#endif
