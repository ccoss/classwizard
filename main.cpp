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
**任凯号 version 1.1
**date  2011.1.4
****************************************************************************/

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QTextStream>
#include <QFile>

#include<stdlib.h>
#include <iostream>

#include "classwizard.h"
//#include "architecture.h"
QTranslator *tor;
Architecture Platform;

Architecture getarchitecture()
{
    QString qsGetArchCmd = "/etc/gaea/scripts/revert/getarch.sh";
    QProcess::execute(qsGetArchCmd);
    QFile file("/etc/gaea/scripts/revert/architecture");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return FALEREAD;
    if(file.size())
    {
        file.close();
        return LOONGSON;
    }
    else
    {
        file.close();
        return X86;
    }
}

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(classwizard);
    Platform = getarchitecture();
    //Q_INIT_RESOURCE(translate);
    QApplication app(argc, argv);
    
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    tor = new QTranslator();
    tor->load("/etc/gaea/trans/en.qm");
    app.installTranslator(tor);
    ClassWizard wizard;
    wizard.show();
    return app.exec();
}
