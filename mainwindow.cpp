 extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
}
#include "mainwindow.h"

FileCopyThread::FileCopyThread(const QString &destDir, const QString &resDir,
                               const quint32 &filesTotal, QWidget *parent)
    : QThread(parent)
    ,m_resDir(resDir)
    ,m_destDir(destDir)
    ,m_filesTotal(filesTotal)
{
    init();
}

FileCopyThread::~FileCopyThread()
{
        qDebug() << "delete thread";
}

void FileCopyThread::init()
{

}

//===============================================================================//
//==【函 数 名 】： void run()
//==【功    能 】：线程的run方法...当调用线程的start方法时就会执行run方法
//==【参    数 】：
//==【返 回 值 】：
//===============================================================================//
void FileCopyThread::run()
{
    QDir resDir(m_resDir);
    QDir destDir(m_destDir);

    bool status = copyDir(destDir, resDir);

    emit finishStatus(status);
}

//===============================================================================//
//==【函 数 名 】： bool copyDir(QDir& destDir, const QDir& resDir)
//==【功    能 】： 拷贝目录
//==【参    数 】： destDir : 目标目录, resDir : 源目录
//==【返 回 值 】： ture拷贝成功 false:拷贝未完成
//===============================================================================//
bool FileCopyThread::copyDir(QDir& destDir, const QDir& resDir)
{
    static quint32 value = 0;
    if(!destDir.exists())
    {
        //当目标目录不存在时...新建
        if(!destDir.mkdir(destDir.absolutePath()))      return false;
    }

    QFileInfoList fileInfoList = resDir.entryInfoList();

    foreach(QFileInfo fileInfo, fileInfoList)
    {
        //过滤"."和".."文件夹
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")   continue;

        //拷贝子目录
        if(fileInfo.isDir())
        {

            //递归调用拷贝
              QDir res(fileInfo.filePath());
              QDir dest(destDir.filePath(fileInfo.fileName()));

              if(!copyDir(dest, res))   return false;
        }
        //拷贝子文件
        else
        {
            if(destDir.exists(fileInfo.fileName()))
            {
                destDir.remove(fileInfo.fileName());
            }
            if(!QFile::copy(fileInfo.filePath(), destDir.filePath(fileInfo.fileName())))
            {
                return false;
            }

            ++value;
            emit setProgressInfo((int)(10  +  value / (m_filesTotal / 80)), destDir.filePath(fileInfo.fileName()));
        }
    }
    return true;
}

ExtractFile::ExtractFile(const QString &srcDir, const QString &destDir, const QString &srcFile,
            const int fileTotal, const bool language, QWidget* parent)
                :QThread(parent)
                ,m_qsSrcDir(srcDir)
                ,m_qsSrcFile(srcFile)
                ,m_qsDestDir(destDir)
                ,m_nFileTotal(fileTotal)
                ,m_language(language)
{
    m_nFileTotal = getSize(srcDir);
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(sendPro()));
    m_timer->start(1000);
}

bool ExtractFile::getTarFileSize()
{
}

ExtractFile::~ExtractFile()
{
        qDebug() << "delete thread";
}

void ExtractFile::run()
{
     bool status;
     status = extractFile();
     //setProgressInfo(int nPro, const QString info);
     emit finishStatus(status);
}

bool ExtractFile::extractFile()
{
    QString qsExtractCmd = "/bin/tar";
    QString extractArgu = "zxf";
    QString extractPartArgu = "-C";
    QStringList arguments;
    QString qsPathName = m_qsSrcDir;
    qsPathName.append(m_qsSrcFile);

    qDebug() << qsExtractCmd << ", " << extractArgu << ", " << qsPathName << ", " << extractPartArgu << ", " << m_qsDestDir;

    arguments.append(extractArgu);
    arguments.append(qsPathName);
    arguments.append(extractPartArgu);
    arguments.append(m_qsDestDir);

    if(QProcess::execute(qsExtractCmd, arguments) == 0)
    {
        m_timer->stop();
        return true;
    }
    else
    {
        m_timer->stop();
        return false;
    }
}

int ExtractFile::getSize(const QString path)
{
    QString duCmd = "/etc/gaea/scripts/getsize.sh";
    QString file1 = "/etc/gaea/scripts/tarSize";
    QStringList arguments;

    arguments.append(path);

    if(QProcess::execute(duCmd, arguments) != 0)
        return 0;

    FILE* fp;
    string file = file1.toStdString();
    fp = fopen(file.c_str(), "r");
    if(fp == NULL)
        return 0;
    char buf[256];
    memset(buf, '\0', 256);

    fgets(buf, 256, fp);
    for(int i = 0; i < strlen(buf); i ++)
    {
        if(buf[i] == '\t')
        {
            buf[i] = '\0';
            break;
        }
    }
    string size = buf;
    string strSize;
    int nSize = 0;
    cout << "size = " << size << endl;
    int nSizeLen = size.size();
    string strUtils = size.substr(nSizeLen - 1, 1);
    int nPoint = size.find('.');
    string strPointBack;
    if(nPoint > 0)
    {
        strPointBack = size.substr(nPoint + 1, 1);
    }

     char buf1[256];
     char buf2[256];
     int nPointNum;
     strcpy(buf2, strPointBack.c_str());
     sscanf(buf2, "%d", &nPointNum);

    strSize = size.substr(0, nSizeLen - 1);
    strcpy(buf1, strSize.c_str());
    sscanf(buf1, "%d", &nSize);

    if(strUtils == "G")
    {
        nSize *= 1024;
        nPointNum *= 1024;
        nPointNum /= 10;
        nSize += nPointNum;
    }
    else if(strUtils == "K")
    {
        nSize /= 1024;
    }
    return nSize;
}

void ExtractFile::sendPro()
{
    int nSize;
     QString qsCopy;
    qDebug() << "send pro exec.....";
    if(m_language)
    {
        qsCopy = "拷贝文件...";
    }
    else
    {
        qsCopy = "copying file...";
    }
    QString qsSizePath = "/mnt/";
    nSize = getSize(qsSizePath);
    m_nFileTotal = SYSTEMSIZE;
    int sendPro = 10 + 70 * (nSize/(float)m_nFileTotal);
    emit setProgressInfo((int)(sendPro), qsCopy);
}

OsInstallWindow::OsInstallWindow(const QString &usrName, const QString &userPassword,
                                 const QString &langegha, const QString &keyboard,
                                 const QString &loginMode, const QString &timeMode, const QString &hostName, const bool languate, const QString qsRevertPart, QWidget *parent)
    : QMainWindow(parent)
    , m_userName(usrName)
    , m_userPassword(userPassword)
    , m_langegha(langegha)
    , m_keyboard(keyboard)
    , m_loginMode(loginMode)
    , m_timeMode(timeMode)
    , m_qsRevertPartition(qsRevertPart)
    , m_language(languate)
    , m_hostName(hostName)
{
    m_nPartitionNum = 0;
    m_nMount = 0;
    m_isMountBoot = true;

    if(m_langegha == "中国")
    {
        m_langegha = "China";
    }
}

OsInstallWindow::~OsInstallWindow()
{
    delete m_thread;
    m_thread = NULL;
}

//===============================================================================//
//==【函 数 名 】： void OsInstallWindow::init()
//==【功    能 】： 初始化
//==【参    数 】：
//==【返 回 值 】：
//===============================================================================//
void OsInstallWindow::init(bool isAutoPartition)
{
    const quint16 ScreenWidth  = QApplication::desktop()->width();
    const quint16 ScreenHeight = QApplication::desktop()->height();

    m_infoLabel = new QLabel(this);
    //显示在窗口屏幕的X ＝ 20 Y ＝ 屏幕的高减掉100
    m_infoLabel->setGeometry(20, ScreenHeight - 100, ScreenWidth - 40, 30);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 100);
    //显示在窗口屏幕的X ＝ 20 Y ＝ 屏幕的高减掉50    进度条的宽度是屏幕的宽度减左右的间隔距离
    m_progressBar->setGeometry(20, ScreenHeight - 50, ScreenWidth - 40, 30);

    m_progressBar->setStyleSheet("QProgressBar{border: 0px;\
                             border-radius: 0px;\
                             background-color:Qt::red;\
                             text-align: none;} \
                             QProgressBar::chunk {\
                             background-color:Qt::green; \
                             width: 10px; \
                             margin: 0.5px;}");

    if(isAutoPartition)
    {
        QTimer::singleShot(1000, this, SLOT(installAuto()));
    }
    else
    {
        QTimer::singleShot(1000, this, SLOT(installHand()));
    }
}

void OsInstallWindow::paintEvent ( QPaintEvent * pEvent )
{
    QPainter painter(this);
    //this->setAutoFillBackground( true );
    //QPalette palette;
    //QImage img( "/etc/gaea/images/background.png" );
    //palette.setBrush( QPalette::Background, img);
    //this->setPalette(palette);

    painter.drawPixmap(0, 0, QPixmap("/etc/gaea/images/background.png").scaled(width(), height(),Qt::IgnoreAspectRatio));
}

void OsInstallWindow::installAuto()
{
     m_isAuto = true;
     if(!autoParted())       return;

     if(m_language)
     {
         setProgressInfo( 10, tr("安装系统.........."));
     }
     else
     {
        setProgressInfo( 10, tr("install system.........."));
    }
     exatSysFile();
}

void OsInstallWindow::installHand()
{
    m_isAuto = false;

    if(m_language)
    {
        setProgressInfo(2, tr("格式化系统.........."));
    }
    {
        setProgressInfo(2, tr("format system.........."));
    }
    sleep( 2 );
    if(!handformatParted())
        return;
    sleep( 2 );
    if(m_language)
    {
        setProgressInfo( 5, tr("挂载系统.........."));
    }
    else
    {
        setProgressInfo( 5, tr("mount system.........."));
    }
    if(!handmountParted())
        return;
    if(m_language)
    {
        setProgressInfo( 10, tr("安装系统.........."));
    }
    else
    {
        setProgressInfo( 10, tr("install system.........."));
    }
    //copySysFile();
    std::cout << "exatSysFile before\n";
    exatSysFile();
    std::cout << "exatSysFile after\n";
}

void OsInstallWindow::finishStatus(bool status)
{
    if(Platform == LOONGSON)
        m_isAuto = false;
    if(status)
    {
         if(m_language)
         {
            setProgressInfo( 90, tr("设置系统.........."));
         }
         else
         {
            setProgressInfo( 90, tr("settings System.........."));
         }
    }
    else
    {
         if(m_language)
         {
             setProgressInfo( 90, tr("安装失败.........."));
         }
         else
         {
            setProgressInfo( 90, tr("install Failure.........."));
         }
         return;
    }

    //多分区情况
    if(m_nMount != 0)
    {
        sleep( 20 );
        qDebug() << "renamePathHand*****************************************88";
        reNamePathHand();
        mountPathHand();
         for(int i = 0; i < m_nMount; i ++)
         {
             QString srcPath = m_qsMountList[i].at(2);
             QString dstPath = m_qsMountList[i].at(2);
             srcPath.append(tr(".back"));

             copyPath(srcPath, dstPath);
             rmPath(srcPath);
         }
    }
    if(m_isAuto)
    {
         reNamePathAuto();
         mountPathAuto();
         copyPath(tr("/mnt/home.back"), tr("/mnt/home"));
         copyPath(tr("/mnt/boot.back"), tr("/mnt/boot"));
    }
    //
     if(m_language)
     {
         setProgressInfo( 92, tr("添加用户.........."));
     }
     else
     {
          setProgressInfo( 92, tr("add user.........."));
     }
    addUser(m_userName, m_userPassword);
    if(m_language)
    {
        setProgressInfo( 92, tr("设置语言.........."));
    }
    else
    {
        setProgressInfo( 92, tr("set language.........."));
    }
    setLanguage(m_langegha);
    if(m_language)
    {
        setProgressInfo( 94, tr("设置时区.........."));
    }
    else
    {
        setProgressInfo( 94, tr("set timeZone.........."));
    }
    setTimeZone(m_timeMode);
    if(m_language)
    {
        setProgressInfo( 96, tr("设置主机名称.........."));
    }
    else
    {
        setProgressInfo( 96, tr("set host Name.........."));
    }
    setHostName(m_hostName);

    mountDev();



    if(m_language)
    {
        setProgressInfo( 96, tr("制作备份系统.........."));
    }
    else
    {
        setProgressInfo( 96, tr("set backup system........."));
    }
    if(!m_isAuto)
    {
       setGrubHand();
       if(Platform == X86)
            writeFstabHand();
    }
    else
    {
        qDebug() << "setGrubauto";
        setGrubAuto();
        writeFstabAuto();
    }
    qDebug() << "setRevertGrub";
    setRevertGrub();
    qDebug() << "setRevertGrub end";
    if(m_loginMode == tr("Auto login"))
    {
        writeAutoLogin();
    }
    setProgressInfo( 96, tr("制作备份系统.........."));
    sleep(1);
    writeAddSudo();
    sleep(1);
    revertSystem();

    if(m_language)
    {
        setProgressInfo(100, tr("系统安装完成，即将重新启动.........."));
    }
    else
    {
        setProgressInfo( 100, tr("the system will restart.........."));
    }
    sleep(5);

    QString qsRobootCmd("/sbin/reboot");
    if(QProcess::execute(qsRobootCmd) == 0);
}

bool OsInstallWindow::writeAddSudo()
{
    QString addUserCmd = "/etc/gaea/scripts/setSudoUser.sh";
    QStringList arguments;
    arguments.append(m_userName);

    if(QProcess::execute(addUserCmd, arguments) == 0)
        return true;
    else
        return false;
}

//===============================================================================//
//==【函 数 名 】： void setProgressInfo(const int &value, const QString &info)
//==【功    能 】： 更新进度条并显示信息
//==【参    数 】： value : 进度值 info : 显示信息
//==【返 回 值 】：
//===============================================================================//
void OsInstallWindow::setProgressInfo(int value, const QString &info)
{
    cout << "value = " << value << endl;
    m_progressBar->setValue(value);
    m_infoLabel->setText(info);

    qApp->processEvents();
}

//===============================================================================//
//==【函 数 名 】： quint64 filesTotal(const QDir &dir)
//==【功    能 】： 计算给定目录下的文件总数
//==【参    数 】： dir
//==【返 回 值 】： quint64 返回文件计算的结果
//===============================================================================//
quint32 OsInstallWindow::filesTotal(const QDir &dir)
{
    static quint32 total = 0;

    QFileInfoList fileInfoList = dir.entryInfoList();

    foreach(QFileInfo fileInfo, fileInfoList)
    {
        //过滤"."和".."文件夹
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")   continue;

        if(fileInfo.isDir())
        {
            //递归调用
            filesTotal(dir.filePath(fileInfo.fileName()));
        }
        else
        {
            ++total;
        }
    }
    return total;
}

void OsInstallWindow::copySysFile()
{
    quint32 total = filesTotal(QDir(cdromDir));

    m_thread = new FileCopyThread(installDir , cdromDir, total);

    connect(m_thread, SIGNAL(finishStatus(bool)), this,  SLOT(finishStatus(bool)));
    connect(m_thread, SIGNAL(setProgressInfo(int, const QString &)),
            this,     SLOT(setProgressInfo(int, const QString &)));

    m_thread->start();
}

void OsInstallWindow::exatSysFile()
{
    QString qsSrcPath = "/home/system/";
    QString qsTemp = "/mnt/";
    std::cout << "1\n";
    QString qsName;
    QDir dir(qsSrcPath);
    std::cout << "2\n";
    QStringList tarFile;
    tarFile << "*.tar.gz";
    dir.setNameFilters(tarFile);
    std::cout << "3\n";
    tarFile = dir.entryList();
    std::cout << "4\n";
    qsName = static_cast<QString>(tarFile.at(0));
    std::cout << "5\n";
    QString pathName;
    std::cout << "6\n";
    pathName = qsSrcPath;
    pathName.append(qsName);
    QFile file(pathName);
    m_nFileSize = file.size();
    m_nFileSize = SYSTEMSIZE;
    cout << "m_nFileSize = " << m_nFileSize << endl;
    m_extactThread = new ExtractFile(qsSrcPath, qsTemp, qsName, m_nFileSize, m_language);

    connect(m_extactThread, SIGNAL(finishStatus(bool)), this,  SLOT(finishStatus(bool)));
    connect(m_extactThread, SIGNAL(setProgressInfo(int, const QString &)),
            this,     SLOT(setProgressInfo(int, const QString &)));

    m_extactThread->start();
}

//===============================================================================//
//==【函 数 名 】： bool autoParted()
//==【功    能 】： 自动分区
//==【参    数 】：
//==【返 回 值 】：
//===============================================================================//
bool OsInstallWindow::autoParted()
{
    if(!deleteParted())
    {
        errorMessage(tr("delete Parted Failure..."));
        return false;
    }

    if(!createParted())
    {
        errorMessage(tr("create Parted Failure..."));
        return false;
    }

    if(!formatParted())
    {
        errorMessage(tr("format Parted Failure..."));
        return false;
    }

    if(!mountParted())
    {
        errorMessage(tr("mount  Parted Failure..."));
        return false;
    }

    return true;
}

bool OsInstallWindow::reNamePathHand()
{
   QString  qsPath;
   QString  qsNewPath;
   for(int i = 0; i < m_nMount; i ++)
   {
       qsPath = m_qsMountList[i].at(2);
       qsNewPath = qsPath;
       qsNewPath.append(tr(".back"));
       QDir     qDirPath(qsPath);
       if(qDirPath.rename(qsPath, qsNewPath))
       {
           mkdirPath(qsPath);
           qsPath.clear();
           qsNewPath.clear();
           continue;
       }
       else
       {
           return false;
       }
   }
   return true;
}

bool OsInstallWindow::reNamePathAuto()
{
   QString  qsPath;
   QString  qsNewPath;
   qsPath = "/mnt/boot";
   qsNewPath = qsPath;
   qsNewPath.append(tr(".back"));
   QDir     qDirPath(qsPath);
   if(qDirPath.rename(qsPath, qsNewPath))
   {
       mkdirPath(qsPath);
       qsPath.clear();
       qsNewPath.clear();
   }

   qsPath = "/mnt/home";
   qsNewPath = qsPath;
   qsNewPath.append(tr(".back"));
   QDir     qDirPath1(qsPath);
   if(qDirPath1.rename(qsPath, qsNewPath))
   {
       mkdirPath(qsPath);
       qsPath.clear();
       qsNewPath.clear();
   }
   return true;
}

bool OsInstallWindow::mkdirPath(const QString path)
 {
     QDir qdDir;
     if(qdDir.mkdir(path))
     {
         return true;
     }
     else
     {
          return false;
     }
 }

bool OsInstallWindow::copyPath(QString srcDir, QString dstDir)
{
    QString cpCmd = "/etc/gaea/scripts/cpCmd.sh";
    QStringList arguments;
    arguments.append(srcDir);
    arguments.append(dstDir);

    if(QProcess::execute(cpCmd, arguments) == 0)
        return true;
    else
        return false;
}

bool OsInstallWindow::rmPath(QString path)
{
    QString     rmCmd = "/bin/rm";
    QStringList arguments;
    QString     argu = "-rf";
    arguments.append(argu);
    arguments.append(path);

    if(QProcess::execute(rmCmd, arguments) == 0)
        return true;
    else
        return false;
}

bool OsInstallWindow::mountPathHand()
{
    qDebug() << "*******************mount system****************************************";
    int nRootPart;
    QString hardMntCmd = "/etc/gaea/scripts/handmnt.sh";
    QStringList arguments;
    QString qsDevice;
    QString qsMount = "/mnt";

    for(int i = 0; i < m_nMount; i ++)
    {
         if(!QProcess::execute(hardMntCmd, m_qsMountList[i]))
         {
             continue;
         }
         else
            return false;
    }
    return true;
}

bool OsInstallWindow::mountPathAuto()
{
    qDebug() << "*******************mount system****************************************";
    QString autoMntCmd = "/etc/gaea/scripts/mountAuto.sh";
    if(QProcess::execute(autoMntCmd))
    {
        return false;
    }
    return true;
}

//===============================================================================//
//==【函 数 名 】： void deleteParted()
//==【功    能 】： 删除分区
//==【参    数 】：
//==【返 回 值 】：
//===============================================================================//
bool OsInstallWindow::deleteParted()
{
    if(m_language)
    {
        setProgressInfo(1, tr("删除分区"));
    }
    else
    {
        setProgressInfo(1, tr("delete Parted"));
    }

    if(QProcess::execute(deleteSH) == 0)
    {
        return true;
    }
    else
        return false;

}

//===============================================================================//
//==【函 数 名 】： void OsInstallWindow::createParted()
//==【功    能 】：
//==【参    数 】：
//==【返 回 值 】：
//===============================================================================//
bool OsInstallWindow::createParted()
{
    if(m_language)
    {
        setProgressInfo(4, tr("创建分区"));
    }
    else
    {
        setProgressInfo(4, tr("create Parted"));
    }

    if(QProcess::execute(createSH) == 0)
        return true;
    else
        return false;

}

//===============================================================================//
//==【函 数 名 】： void formatParted()
//==【功    能 】：
//==【参    数 】：
//==【返 回 值 】：
//===============================================================================//
bool OsInstallWindow::formatParted()
{
    if(m_language)
    {
        setProgressInfo(7, tr("格式化分区"));
    }
    else
    {
        setProgressInfo(7, tr("format Parted"));
    }

    if(QProcess::execute(formatSH) == 0)
        return true;
    else
        return false;
}

//===============================================================================//
//==【函 数 名 】： void mountParted()
//==【功    能 】：
//==【参    数 】：
//==【返 回 值 】：
//===============================================================================//
bool OsInstallWindow::mountParted()
{
    if(m_language)
    {
        setProgressInfo(10, tr("挂载分区"));
    }
    else
    {
        setProgressInfo(10, tr("mount Parted"));
    }

    if(QProcess::execute(mountSH) == 0)
        return true;
    else
        return false;
}

//===============================================================================//
//==【函 数 名 】： void addUser(const QString &username, const QString &password )
//==【功    能 】： 添加用户
//==【参    数 】： username : 用户名   password : 密码
//==【返 回 值 】： 无
//===============================================================================//
void OsInstallWindow::addUser(const QString &username, const QString &password )
{
    QString addUserSH = "/etc/gaea/scripts/addAdmin.sh";
    QStringList arguments;
    arguments.append(username);
    arguments.append(password);

    if(QProcess::execute(addUserSH, arguments) == 0)
        return;
}

//===============================================================================//
//==【函 数 名 】： void setLanguage(const QString &language)
//==【功    能 】： 设置系统语言
//==【参    数 】： language ： 语言标识
//==【返 回 值 】：
//===============================================================================//
void OsInstallWindow::setLanguage(const QString &language)
{
    QString laguageSH = "/etc/gaea/scripts/setLanguage.sh";
    QStringList arguments;
    arguments.append(language);

    if(QProcess::execute(laguageSH, arguments) == 0)
        return;
    else
        return;
}

void OsInstallWindow::setTimeZone(const QString &timeZone)
{
    QString timeZoneSH = "/etc/gaea/scripts/setTime.sh";
    QStringList arguments;
    arguments.append(timeZone);
    if(QProcess::execute(timeZoneSH, arguments) == 0)
        return;
}

void OsInstallWindow::setHostName(const QString &hostName)
{
    QString setHostSH = "/etc/gaea/scripts/setHostname.sh";
    QStringList arguments;
    arguments.append(hostName);
    if(QProcess::execute(setHostSH, arguments) == 0)
        return;
}

//===============================================================================//
//==【函 数 名 】： void setKeyboard(const QString &keyboard)
//==【功    能 】： 设置键盘
//==【参    数 】： keyboard ： 键盘标识
//==【返 回 值 】：
//===============================================================================//
void OsInstallWindow::setKeyboard(const QString &keyboard)
{
}

//===============================================================================//
//==【函 数 名 】： void setLoginMode(int mode)
//==【功    能 】： 设置用户登入方式
//==【参    数 】： mode ： 登入模式
//==【返 回 值 】：
//===============================================================================//
void OsInstallWindow::setLoginMode(int mode)
{
}

//===============================================================================//
//==【函 数 名 】： void errorMessage(const QString &message)
//==【功    能 】： 显示错误信息
//==【参    数 】： message ： 显示的信息
//==【返 回 值 】：
//===============================================================================//
void OsInstallWindow::errorMessage(const QString &message)
{
     QMessageBox::critical(this, tr(""), message);
}

//===============================================================================//
//==【函 数 名 】： void getPartInfo(mountinfo partedTread)
//==【功    能 】： 获得安装硬盘信息
//==【参    数 】： partedTread ： 安装信息
//==【返 回 值 】：
//===============================================================================//
bool OsInstallWindow::getPartInfo(mountinfo partedTread)
{
    m_partedTreat[m_nPartitionNum ++] = partedTread;
}


//===============================================================================//
//==【函 数 名 】： bool handformatParted()
//==【功    能 】： 格式化安装分区
//==【参    数 】
//==【返 回 值 】：
//===============================================================================//
bool OsInstallWindow::handformatParted()
{
    qDebug() << "*******************format system****************************************";
    QString hardformatCmd = "/etc/gaea/scripts/handfmt.sh";
    QString qsDevice;
    QStringList arguments;
    qDebug() << "format disk info: ";
    for(int i = 0; i < m_nPartitionNum; i ++)
    {
        arguments.clear();
        if(!m_partedTreat[i].qsMountPoint.isNull())
        {
            qsDevice = tr("/dev/") + m_partedTreat[i].qsDeviceName;

            arguments.append(qsDevice);
            arguments.append(m_partedTreat[i].qsSystem);

            if(QProcess::execute(hardformatCmd, arguments) == 0)
                continue;
            else
                return false;
        }
    }
    if(m_qsRevertPartition != "NULL")
    {
        arguments.append(qsDevice);
        arguments.append(m_qsRevertPartition);

        if(QProcess::execute(hardformatCmd, arguments) == 0)
        {
            ;
        }
        else
        {
            return false;
        }
    }
    return true;
}

//===============================================================================//
//==【函 数 名 】： bool OsInstallWindow::handmountParted()
//==【功    能 】： 挂载安装分区
//==【参    数 】：
//==【返 回 值 】：
//===============================================================================//
//bool OsInstallWindow::handmountParted()
//{
//    qDebug() << "*******************mount system****************************************";
//    QString hardMntCmd = "/etc/gaea/scripts/handmnt.sh";
//    QStringList arguments;
//    QString qsDevice;
//    QString qsMount = "/mnt";
//    for(int i = 0; i < m_nPartitionNum; i ++)
//    {
//        if(!m_partedTreat[i].qsMountPoint.isNull())
//        {
//            qsDevice = tr("/dev/") + m_partedTreat[i].qsDeviceName;
//            qsMount.append(m_partedTreat[i].qsMountPoint);
//            arguments.append(qsDevice);
//            arguments.append(m_partedTreat[i].qsSystem);
//            arguments.append(qsMount);
//            qDebug() << m_partedTreat[i].qsDeviceName << ": "
//                    <<m_partedTreat[i].qsSystem << ": "
//                    << qsMount;
//             if(QProcess::execute(hardMntCmd, arguments) == 0)
//                 continue;
//             else
//                 return false;
//        }
//    }
//    return true;
//}

bool OsInstallWindow::mountDev()
{
    QString qsMountDevCmd = "/etc/gaea/scripts/mountDev.sh";
    if(QProcess::execute(qsMountDevCmd) == 0)
        return true;
    else
        return false;
}

bool OsInstallWindow::setGrubHand()
{
    qDebug() << "set Grub Hand in!";
    QString qsSetBootCmd;
    if(Platform == X86)
        qsSetBootCmd = "/etc/gaea/scripts/setGrubX86.sh";
    else
        qsSetBootCmd = "/etc/gaea/scripts/setPmonMips.sh";
    QStringList arguments;
    if(Platform == X86)
    {
        arguments.append(m_qsMainDevice);
    }
    else
    {
        qDebug() << "ren debug arug1 = " << m_qsFilesytem;
        qDebug() << "ren debug arug2 = " << m_qsPartition;
        arguments.append(m_qsFilesytem);
        arguments.append(m_qsPartition);
        //modify for loongson

    }
    if(!QProcess::execute(qsSetBootCmd, arguments))
    {
            std::cout << "set grub ok" << std::endl;
    }
    else
    {
        if(this->m_language)
        {
            QMessageBox::critical(this, tr("安装失败"),tr("安装失败，请重新安装！"), tr("确定") );
        }
        else
        {
            QMessageBox::critical(this, tr("backupfailed"),tr("backup filed!"), tr("Yes") );
            this->close();
        }
        std::cout << "set grub not ok" << std::endl;
    }
    qDebug() << "set Grub Hand out!";
}

bool OsInstallWindow::setGrubAuto()
{
    //判断是否存在/ect/grub.d/10_linux文件
    QString file("/mnt/etc/grub.d/10_linux");
    QString mvCmd = "/bin/mv";

    QString file1 = file;
    file1.append(".back");
    QFile a(file);
    QStringList arguments;
    arguments.append(file);
    arguments.append(file1);

    //创建10_linux
    QFile linuxSet("06_linuxgaea");
    linuxSet.open(QIODevice::WriteOnly);
    QTextStream out(&linuxSet);

    QString qsInsmod = "insmod ";
    qsInsmod.append("ext2");
    qsInsmod.append("\n");

    QString qsSetRoot = "set root=(hd0,";
    qsSetRoot.append("3");
    qsSetRoot.append(")");
    qsSetRoot.append("\n");

    QString qsLinux = "linux /";
    QString qsInitrd = "initrd /";

    QDir qdBoot("/mnt/boot");
    QFileInfoList fileInfoList;

    fileInfoList = qdBoot.entryInfoList();

    foreach(QFileInfo fileInfo, fileInfoList)
    {
        QString fileName = fileInfo.fileName();
        if(fileName.startsWith("vmlinuz", Qt::CaseSensitive))
        {
            qsLinux.append(fileName);
        }
        if(fileName.startsWith("initrd", Qt::CaseSensitive))
        {
            qsInitrd.append(fileName);
        }
    }
    qsLinux.append(" root=/dev/sda1");
    qsLinux.append(" ro\n");

    qsInitrd.append("\n");

    out << QString("#!/bin/sh -e\n");
    out << QString("\n");
  //  out << QString("echo \"gaea\"\n");
    out << QString("cat << EOF\n");
    out << QString("\n");
    out << QString("menuentry \"共创gaea\"{\n");

    out << qsInsmod;
    out << qsSetRoot;
    out << qsLinux;
    out << qsInitrd;

    out << QString("}\n");
    out << QString("EOF\n");

    linuxSet.close();

    arguments.clear();

    arguments.append(tr("06_linuxgaea"));
    arguments.append(tr("/mnt/etc/grub.d/"));

    QString qsSetGrubCmd = "/etc/gaea/scripts/setGrubX86.sh";
    arguments.clear();
    arguments.append("sda");
    if(!QProcess::execute(qsSetGrubCmd, arguments))
    {
            std::cout << "set grub ok" << std::endl;
    }
    else
    {
            std::cout << "set grub not ok" << std::endl;
    }
}

bool OsInstallWindow::issata()
{
    QFile qfCpuInfo("/proc/cpuinfo");
    m_qsIsSata = "false";
    if(qfCpuInfo.open(QFile::ReadOnly | QFile::Truncate))
    {
        QTextStream stream(&qfCpuInfo);
        QString qsLine;

        qsLine = stream.readLine();
        do {
            if(qsLine.startsWith(tr("system type")))
            {
                if(!QString::compare(qsLine, tr("system type            : lemote-yeeloong"), Qt::CaseSensitive))
                {
                    m_isSata = true;
                    m_qsIsSata = "true";
                }
                else
                {
                    m_qsIsSata = "false";
                    m_isSata = false;
                }
                break;
            }
        }while(!qsLine.isNull());
    }
}


void OsInstallWindow::writeFstabHand()
{
    QString qsFsTab("/etc/gaea/scripts/fstab");
    QFile qfFsTab(qsFsTab);
    qfFsTab.open(QIODevice::WriteOnly);
    QTextStream in(&qfFsTab);

    int nLen = m_qsPartition.size();
    QString qsPartNum = QString("%1").arg(m_qsPartition[nLen - 1]);

    QString qsPart = "/dev/sda";
    qsPart.append(qsPartNum);
    qsPart.append("\t");

    QString qsMount("/\t");

    string strSystemType = m_qsSystemType.toStdString();
    int len = strSystemType.size();
    string strLast = strSystemType.substr(len - 1, 1);
    if(strLast == "\n" || strLast =="'\a" || strLast == "\t")
    {
        strSystemType = strSystemType.substr(0, len - 1);
        m_qsSystemType.clear();
        m_qsSystemType = QString::fromStdString(strSystemType);
    }
    QString qsSystem(m_qsSystemType);

    qsSystem.append("\t");

    QString qsOption = "defaults\t";

    QString qsNum = "0\t0";

    in << qsPart;
    in << qsMount;
    in << qsSystem;
    in << qsOption;
    in << qsNum;

    if(!m_qsSwapDevice.isNull())
    {
        in << "\n";
        int nLen = m_qsPartition.size();
        QString qsPartNum = QString("%1").arg(m_qsSwapDevice[nLen - 1]);

        QString qsPart = "/dev/sda";
        qsPart.append(qsPartNum);
        qsPart.append("\t");

        QString qsMount("none\t");

        QString qsSystem("swap");
        qsSystem.append("\t");

        QString qsOption = "sw\t";

        QString qsNum = "0\t0";

        in << qsPart;
        in << qsMount;
        in << qsSystem;
        in << qsOption;
        in << qsNum;
    }

    qfFsTab.close();

    QString mvCmd = "/bin/mv";
    QStringList arguments;
    arguments.append("/etc/gaea/scripts/fstab");
    arguments.append("/mnt/etc/");

    if(!QProcess::execute(mvCmd, arguments))
    {
            std::cout << "mv is ok" << std::endl;
    }
    else
    {
            std::cout << "mv is not ok" << std::endl;
    }
}

void OsInstallWindow::writeFstabAuto()
{
    QString qsFsTab("/etc/gaea/scripts/fstab");
    QFile qfFsTab(qsFsTab);
    qfFsTab.open(QIODevice::WriteOnly);
    QTextStream in(&qfFsTab);

    QString qsPart = "/dev/sda1";
    qsPart.append("\t");

    QString qsMount("/\t");

    QString qsSystem("ext3");
    qsSystem.append("\t");

    QString qsOption = "defaults\t";

    QString qsNum = "0\t0";

    in << qsPart;
    in << qsMount;
    in << qsSystem;
    in << qsOption;
    in << qsNum;

    in << "\n";

    in <<"/dev/sda2\t";
    in <<"/home\t";
    in <<"ext3\t";
    in <<"defaults\t";
    in <<"0\t0";

    qfFsTab.close();

    QString mvCmd = "/bin/mv";
    QStringList arguments;
    arguments.append("/etc/gaea/scripts/fstab");
    arguments.append("/mnt/etc/");

    if(!QProcess::execute(mvCmd, arguments))
    {
            std::cout << "mv is ok" << std::endl;
    }
    else
    {
            std::cout << "mv is not ok" << std::endl;
    }

}

bool OsInstallWindow::writeAutoLogin()
{
    QString qsAutoLoginCmd = "/etc/gaea/scripts/setAutoLogin.sh";
    QStringList arguments;
    arguments.append(m_userName);
    arguments.append(m_userPassword);

    if(QProcess::execute(qsAutoLoginCmd, arguments) == 0)
    {
        return true;
    }
    return false;
}

bool OsInstallWindow::getUuid(QString &qsUuid, QString qsPart)
{
    QString qsGetUuidCmd = "/etc/gaea/scripts/getUuid.sh";
    QStringList arguments;
    arguments.append(qsPart);

    if(QProcess::execute(qsGetUuidCmd, arguments) == 0)
    {
        return true;
    }
    QFile file("/etc/gaea/scripts/uuid");
    if(file.open(QIODevice::ReadOnly))
    {
        char buf[256];
        qint64 lineLen = file.readLine(buf, sizeof(buf));
        if(lineLen != -1)
        {
            string strUuid(buf);
            qsUuid = QString::fromStdString(strUuid);
            int nPointOne = qsUuid.indexOf(tr("UUID"));
            int nPointTwo = qsUuid.indexOf(tr("TYPE"));
            //返回该字符串的小写串
            qsUuid = qsUuid.mid(nPointOne + 6, nPointTwo - nPointOne - 8);
            qDebug() << qsUuid;
            return true;
        }
    }
    return false;
}

bool OsInstallWindow::handmountParted()
{
    qDebug() << "*******************mount system****************************************";
    int nRootPart;
    QString hardMntCmd = "/etc/gaea/scripts/handmnt.sh";
    QStringList arguments;
    QString qsDevice;
    QString qsMount = "/mnt";

    for(int i = 0; i < m_nPartitionNum; i ++)
    {
        if(m_partedTreat[i].qsMountPoint == tr("/"))
        {

            qsDevice = tr("/dev/") + m_partedTreat[i].qsDeviceName;
            m_qsMainDevice = m_partedTreat[i].qsMainDevice;
            m_qsPartition = m_partedTreat[i].qsDeviceName;
            m_qsFilesytem = m_partedTreat[i].qsSystem;
            qsMount.append(m_partedTreat[i].qsMountPoint);
            qDebug() << "ren debug root: m_qsPartiton = " << m_qsPartition;
            arguments.append(qsDevice);
            arguments.append(m_partedTreat[i].qsSystem);
            m_qsSystemType = m_partedTreat[i].qsSystem;
            arguments.append(qsMount);
            qDebug() << m_partedTreat[i].qsDeviceName << ": "
                    <<m_partedTreat[i].qsSystem << ": "
                    << qsMount;
             if(QProcess::execute(hardMntCmd, arguments) == 0)
             {
                 nRootPart = i;
                 break;
             }
             else
                 return false;
        }
    }
    arguments.clear();

    for(int i = 0; i < m_nPartitionNum; i ++)
    {
        if(i == nRootPart)
        {
            continue;
        }
        if(!m_partedTreat[i].qsMountPoint.isNull())
        {
            if(m_partedTreat[i].qsMountPoint == "boot")
            {
                m_isMountBoot = false;
                m_qsBootDevice = m_partedTreat[i].qsDeviceName;
            }
            if(m_partedTreat[i].qsMountPoint == "none")
            {
                m_qsSwapDevice = m_partedTreat[i].qsDeviceName;
                continue;
            }
            qsDevice = tr("/dev/") + m_partedTreat[i].qsDeviceName;
            qsMount.append(m_partedTreat[i].qsMountPoint);
            arguments.append(qsDevice);
            arguments.append(m_partedTreat[i].qsSystem);
            arguments.append(qsMount);

            //记录其它挂载信息
            m_qsMountList[m_nMount ++] = arguments;
            //
            arguments.clear();
        }
    }
    return true;
}

void OsInstallWindow::revertSystem()
{
    //mount revert partition
    QString qsRevertCMD="/etc/gaea/scripts/setRevertPartition.sh";
    QStringList arguments;
    arguments.append(m_qsRevertPartition);
    if(QProcess::execute(qsRevertCMD, arguments) == 0)
    {
        return;
    }
    return;
}

bool OsInstallWindow::setRevertGrub()
{
    QString qsRevertCMD;
    QStringList arguments;
    int nSize = m_qsRevertPartition.size();
    QString qsNum;

    if(Platform == X86)
    {
        qsRevertCMD="/etc/gaea/scripts/revert/setRevertGrub.sh";
        arguments.append(tr("hd0"));
        if(nSize == 4)
        {
            qsNum = m_qsRevertPartition.mid(3, 1);
        }
        else if(nSize == 5)
        {
            qsNum = m_qsRevertPartition.mid(3, 2);
        }
        arguments.append(qsNum);
    }
    else if(Platform == LOONGSON)
    {
        qsRevertCMD = "/etc/gaea/scripts/revert/setRevertPmon.sh";
        //arguments.append("sda1");
        arguments.append(m_qsPartition);
        arguments.append(m_qsRevertPartition);
        arguments.append("false");
    }

    qDebug() << "ren test: m_qsRevPart= " << m_qsRevertPartition;
    if(QProcess::execute(qsRevertCMD, arguments) == 0)
    {
        return true;
    }
    return false;
}
