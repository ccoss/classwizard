#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAXPARTITION 32
#include <string>
#include <iostream>
#include <QtGui>
#include <QTest>
#include "architecture.h"

extern Architecture Platform;
using namespace std;

#define SYSTEMSIZE 3481

typedef struct mountinfo
{
public:
    QString qsMainDevice;
    QString qsDeviceName;
    QString qsSystem;
    QString qsMountPoint;
    QString qsSize;
    bool isPrimary;
}mountinfo;


const QString cdromDir   = "/home/bin";
const QString installDir = "/opt/bin";

const  QString  deleteSH  = "/etc/gaea/scripts/delete.sh";
const  QString  createSH  = "/etc/gaea/scripts/create.sh";
const  QString  formatSH  = "/etc/gaea/scripts/format.sh";
const  QString  mountSH   = "/etc/gaea/scripts/mount.sh";

//**************************** 创建一个拷贝文件的线程 ******************************//
class FileCopyThread  : public QThread
{
    Q_OBJECT

public:
    FileCopyThread(const QString &destDir, const QString &resDir,
                   const quint32 &filesTotal,    QWidget *parent = 0);
    ~FileCopyThread();

Q_SIGNALS:
    void setProgressInfo(int, const QString &);
    void finishStatus(bool);

protected:
    virtual void run();

private:
    QString m_resDir;
    QString m_destDir;
    quint32 m_filesTotal;

    void init();

    bool copyDir(QDir& destDir, const QDir& resDir);
};

//*****************************解压文件线程****************************************//
class ExtractFile : public QThread
{
     Q_OBJECT
public:
    ExtractFile(const QString &srcDir, const QString &destDir, const QString &srcFile,
                const int fileTotal, const bool language, QWidget* parent = 0);
    ~ExtractFile();
    bool getTarFileSize();
    quint32 filesTotal(const QDir &dir);

protected:
    virtual void run();

private:
    QString m_qsSrcDir;
    QString m_qsSrcFile;
    QString m_qsDestDir;
    QTimer* m_timer;
    int     m_nFileTotal;
    bool    m_language;

    bool extractFile();
    int getSize(const QString path);

signals:
    void explains();
    void setProgressInfo(int nPro, const QString info);
    void finishStatus(bool isOk);
public slots:
    void sendPro();
};

//******************************* 安装窗口 ****************************************//
class OsInstallWindow : public QMainWindow
{
    Q_OBJECT

public:
            OsInstallWindow(const QString &usrName, const QString &userPassword,
                    const QString &langegha,const QString &keyboard,
                    const QString &loginMode, const QString &m_timeMod, const QString &hostName, const bool languate, const QString qsRevertPart = "NULL", QWidget *parent = 0);

            ~OsInstallWindow();
    bool    getPartInfo(mountinfo partedTread);          //获得分区信息
    void    init(bool isAutoPartition);                  //初始化并且开始安装系统

    //多分区情况
    bool    reNamePathHand();
    bool    reNamePathAuto();
    bool    mkdirPath(const QString path);
    bool    copyPath(QString srcDir, QString dstDir);
    bool    rmPath(QString path);
    bool    mountPathHand();
    bool     mountPathAuto();
    bool    m_language;
    //

protected:
   virtual  void paintEvent( QPaintEvent * pEvent );

private Q_SLOTS:
    void    installAuto();
    void    installHand();
    void    finishStatus(bool status);
    void    setProgressInfo(int value, const QString &info);

private:
    QLabel          *m_infoLabel;
    QProgressBar    *m_progressBar;
    FileCopyThread  *m_thread;
    ExtractFile     *m_extactThread;
    int             m_nFileSize;

    QString         m_userName;
    QString         m_userPassword;
    QString         m_langegha;
    QString         m_keyboard;
    QString         m_loginMode;
    QString         m_timeMode;
    QString         m_hostName;
    int             m_nMount;
    QString         m_swapPartition;
    QStringList     m_qsMountList[5];

    //for grub
    QString         m_qsMainDevice;
    QString         m_qsPartition;
    QString         m_qsSystemType;
    QString         m_qsBootDevice;
    QString         m_qsSwapDevice;
    QString         m_qsRevertPartition;
    QString         m_qsFilesytem;
    bool            m_isMountBoot;
    bool            m_isAuto;

    //is the pmon is sata0 for boot
    bool            m_isSata;
    QString         m_qsIsSata;

    //hand parted
    int         m_nPartitionNum;
    mountinfo   m_partedTreat[MAXPARTITION];

private:

    void        copySysFile();
    void        exatSysFile();
    quint32     filesTotal(const QDir &dir);
    void        errorMessage(const QString &message);
    bool        mountDev();
    bool        setGrubHand();
    bool        setGrubAuto();
    void        writeFstabHand();
    void        writeFstabAuto();
    bool        writeAddSudo();
    bool        writeAutoLogin();
    bool        getUuid(QString &qsUuid, QString qsPart);
    bool        issata();
    bool        setRevertGrub();

    //addrevert
    bool iscreaterevertpart();
    bool createrevertpart();
    bool revertsystem();
    bool updategrub();

//*********************************** 搭建系统环境 ******************************************//
    bool autoParted();                 //自动分区
    bool deleteParted();                //删除分区
    bool createParted();                //创建磁盘分区
    bool formatParted();                //格式化磁盘分区
    bool mountParted();                 //挂载分区并安装系统
    bool handformatParted();            //格式化分区
    bool handmountParted();             //挂载安装分区
//************************************* 系统设置 *******************************************//

    void addUser(const QString &username, const QString &password );
    void setLanguage(const QString &language);
    void setTimeZone(const QString &timeZone);
    void setHostName(const QString &hostName);
    void setKeyboard(const QString &keyboard);
    void setLoginMode(int mode);
//**************************************还原系统********************************************//
    void revertSystem();
};

#endif // MAINWINDOW_H
