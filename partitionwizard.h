#ifndef PARTITIONWIZARD_H
#define PARTITIONWIZARD_H

#include <QtGui/QRadioButton>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QTableWidget>
#include <QtGui/QPushButton>
#include <QtCore/QString>

#include "diskinfo.h"
#include "mainwindow.h"
#include "addpartdialog.h"
#include "addrevert.h"
//#include "architecture.h"

#define MAXPARTITION 32
class PartitionInfo;

extern QTranslator *tor;
extern Architecture Platform;
//extern Architecture Platform;
class PartitionTool : public QWizardPage
{
    Q_OBJECT

public:
    PartitionTool(QWidget *parent = 0);

    //void setLan
    bool getPartitioninfo(DiskPartitionInfo &partinfo, int num);
    bool getPartition();

    QString getMainDevice() const
    {
       return m_qsCurrentDevice;
    }

    bool getisAutoPartition() const
    {
        return isAutoPartition;
    }
    int getPartitionNum() const
    {
        return m_nPartitionNum;
    }

    bool setLanguage(bool language);
    void checkCanNext();
    void clearMountPoint();
    void updateDiskTableWidget(bool isGetPartInfo);
    QString getRevertPartition() const
    {
        return m_qsRevertPartition;
    }

private:
    bool                    isAutoPartition; //true partAuto, false partbyHan
    QLabel*                 partAutoLabel;
    QLabel*                 diskSelLabel;
    QComboBox*              diskSelComboBox;
    QTableWidget*           diskInfoTableWidget;

    QRadioButton*           partAutoRadioButton;
    QRadioButton*           partManRadioButton;
    //QPushButton*            expertPushButton;
    QPushButton*            m_addPartitionButton;
    QPushButton*            m_delPartitionButton;

    DiskPartitionInfo       m_diskPartitionInfo[MAXPARTITION];
    int                     m_nPartitionNum;
    DiskInfo*               m_diskInfo;
    PartitionInfo*          m_partitionInfoClass;
    AddPartitionDialog*     m_addPartition;

    QString                 m_qsCurrentDevice;         //当前活动硬盘
    QString                 m_qsCurrentDiskSpace;      //当前硬盘容量
    QString                 m_qsCurrentDiskFreeSpace;  //当前硬盘剩余空间
    QString                 m_qsFreeSize;
    int                     m_nFreeSpaceRow;
    bool                    m_language;
    QStringList             m_labels;

    //add revert
    AddRevert*              m_addrevert;
    QPushButton*            m_qpAddRevert;
    QString                 m_qsRevertPartition;

    //update tablewidget data
    bool addPartitionExec(QStringList arguments);
    bool delPartitionExec(QStringList arguments);
    void showSize(string &showSize);
    void clearPartTable();
    bool compareSize(string Size, string compareSize);   

private slots:

    void Change(int row, int column);
    void diskinfoUnshown();
    void diskinfoShown();
    void diskinfoShown(int);
    void addPartition();
    void checkRootSys(QString mountPoint, QString type);
    void setPartition(QString qsSize, QString qsMountPoint, QString qsSystemType, QString qsPartedType);
    void setPartition1(QString qsSize, QString qsMountPoint, QString qsSystemType, QString qsPartedType);
    void delPartition();
    void setPartitionButtionOn(int row, int column);
    void bootGpartedExec();
     void addrevert();
    void addRvertPart(QString qsRevertpart);

signals:

    void explains();
    void partitedOk();                                    //能够进行下一步
    void partitedNotOk();                                 //不能进入下一步
};

class PartitionInfo : public QDialog
{
    Q_OBJECT

public:
    PartitionInfo(QWidget *parent = 0);
    ~PartitionInfo(){}
    void setLanguage(bool language);

signals:
    void explains();
    void clickSure(QString mountPoint, QString type);

private:
    QLabel*             systemLabel;
    QLabel*             mountLabel;
    QLabel*             formattingLabel;
    QComboBox*          systemSelComboBox;
    QComboBox*          mountSelComboBox;
    QPushButton*        pbSure;
    QPushButton*        pbCancel;
    QCheckBox*          formattingCheckBox;

private slots:

    void clickedPbCancel();
    void clickedPbSure();
    void isSwap(const QString &text);
};
#endif // PARTITIONWIZARD_H
