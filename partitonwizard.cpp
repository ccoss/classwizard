#include <QMessageBox>
//#include <string>
#include "partitionwizard.h"
//#include "architecture.h"

PartitionTool::PartitionTool(QWidget *parent)
    : QWizardPage(parent)
{
    bool isPartition;
    bool isAddPartitionComBox = false;
    isAutoPartition = true;
    m_nPartitionNum = 0;
    m_nFreeSpaceRow = -1;
    m_qsFreeSize = "freesize";
    this->setAutoFillBackground(true);

    QPalette palette;
    QImage img( "/etc/gaea/images/background.png" );
    palette.setBrush( QPalette::Background, img);
    this->setPalette(palette);

    if(Platform == X86)
    {
        partAutoRadioButton = new QRadioButton;
        partManRadioButton = new QRadioButton;
    }
    partAutoLabel = new QLabel;
    diskSelLabel = new QLabel;

    //add and del partition button init
    m_addPartitionButton = new QPushButton;
    m_delPartitionButton = new QPushButton;
    m_qpAddRevert = new QPushButton;
    m_addPartitionButton->setEnabled(false);
    m_delPartitionButton->setEnabled(false);
    
    //init partition get partition name
    isPartition = getPartition();

    //init combox
    diskSelComboBox = new QComboBox;
    if(Platform == X86)
        diskSelComboBox->setEnabled(false);
    else if(Platform == LOONGSON)
        diskSelComboBox->setEnabled(true);
    QString qsTempMainPartition;
    if(!isPartition)
    {
        qDebug() << "no partition";
    }
    else
    {
        for(int i = 0; i < m_nPartitionNum; i ++)
        {
            qsTempMainPartition.clear();
            qsTempMainPartition = QString::fromStdString(m_diskPartitionInfo[i].strPartitionDevice);
            if(i != 0)
            {
                for(int j = 0; j < i; j ++)
                {
                    if(qsTempMainPartition == diskSelComboBox->itemText(j))
                    {
                        isAddPartitionComBox = true;
                    }
                }
            }
            if(!isAddPartitionComBox)
                diskSelComboBox->insertItem(i, qsTempMainPartition);
            isAddPartitionComBox = false;
        }
    }

    //init tablewidget
    diskInfoTableWidget = new QTableWidget();
    diskInfoTableWidget->verticalHeader()->setVisible(false);
    //隔行显示颜色
    diskInfoTableWidget->setAlternatingRowColors(true);

    diskInfoTableWidget->setColumnCount(5);
    m_labels << tr("Device") <<tr("systemType") <<tr("Mount") <<tr("Size") << tr("partType");
    diskInfoTableWidget->setHorizontalHeaderLabels(m_labels);
    diskInfoTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    diskInfoTableWidget->setSelectionBehavior(diskInfoTableWidget->SelectRows);
    diskInfoTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    //add data to tablewidget
    clearPartTable();
    updateDiskTableWidget(true);

    if(Platform == X86)
        diskInfoTableWidget->setEnabled(false);
    else if(Platform == LOONGSON)
        diskInfoTableWidget->setEnabled(true);

    QHBoxLayout *disklayout = new QHBoxLayout;
    //disklayout->addWidget(partManRadioButton);
    disklayout->addSpacing(26);
    disklayout->addWidget(diskSelLabel);
    //disklayout->addStretch(2);
    disklayout->addWidget(diskSelComboBox);
    disklayout->addSpacing(400);

    QHBoxLayout *expertlayout = new QHBoxLayout;
    expertlayout->addSpacing(650);
    expertlayout->addWidget(m_addPartitionButton);
    expertlayout->addWidget(m_delPartitionButton);
    expertlayout->addWidget(m_qpAddRevert);

    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->setContentsMargins(30,10,30,10);
    if(Platform == X86)
    {
        groupBoxLayout->addWidget(partAutoRadioButton);
   // groupBoxLayout->addWidget(partAutoLabel);
        groupBoxLayout->addWidget(partManRadioButton);
    }
    groupBoxLayout->addLayout(disklayout);
    groupBoxLayout->addWidget(diskInfoTableWidget);
    groupBoxLayout->addSpacing(20);
    groupBoxLayout->addLayout(expertlayout);
    groupBoxLayout->addSpacing(150);
    if(Platform == X86)
        partAutoRadioButton->setChecked(true);

    if(Platform == X86)
    {
        QObject::connect(this->partAutoRadioButton,SIGNAL(clicked()),this,SLOT(diskinfoUnshown()));
        QObject::connect(this->partManRadioButton,SIGNAL(clicked()),this,SLOT(diskinfoShown()));
    }

    QObject::connect(this->m_addPartitionButton, SIGNAL(clicked()), this, SLOT(addPartition()));
    QObject::connect(this->m_delPartitionButton, SIGNAL(clicked()), this, SLOT(delPartition()));

    QObject::connect(this->diskInfoTableWidget, SIGNAL(cellDoubleClicked(int,int)),this,SLOT(Change(int, int)));
    QObject::connect(this->diskSelComboBox,     SIGNAL(activated(int)),this,            SLOT(diskinfoShown(int)));
    QObject::connect(this->diskInfoTableWidget, SIGNAL(cellClicked(int,int)), this,     SLOT(setPartitionButtionOn(int, int)));
    QObject::connect(this->m_qpAddRevert, SIGNAL(clicked()), this, SLOT(addrevert()));
    //QObject::connect(this->expertPushButton, SIGNAL(clicked()), this, SLOT(bootGpartedExec()));

    setLayout(groupBoxLayout);

    diskInfoTableWidget->setColumnWidth(0, 718/5);
    diskInfoTableWidget->setColumnWidth(1, 718/5);
    diskInfoTableWidget->setColumnWidth(2, 718/5);
    diskInfoTableWidget->setColumnWidth(3, 718/5);
    diskInfoTableWidget->setColumnWidth(4, 718/5);

    setTitle(tr("Partition"));
}

void PartitionTool::addrevert()
{
    QStringList argu;
    for(int i = 0; i < this->m_nPartitionNum; i ++)
    {
        if((m_diskPartitionInfo[i].strPartitionDevice == this->diskSelComboBox->currentText().toStdString())
            && (m_diskPartitionInfo[i].strMountPoint.size() == 0))
        {
            QString ar;
            ar = QString::fromStdString(m_diskPartitionInfo[i].strPartitionName);
            argu.append(ar);
            std::cout <<ar.toStdString() << std::endl;
            ar.clear();
        }
    }
    bool isCanNext = false;
    for(int i = 0; i < this->m_nPartitionNum; i ++)
    {
        if(m_diskPartitionInfo[i].strMountPoint == "/")
        {
            isCanNext = true;
        }
    }

    if(!isCanNext)
    {
        argu.clear();
    }

    if(argu.size() == 0)
    {
        QMessageBox::information(this, tr("warning"), tr("there in no restore partiton!"),tr("Yes") );
        //QMessageBox* mess = new QMessageBox();
        //mess->setText(tr("没有提供备份分区！"));
        //mess->exec();
        return;
    }
    m_addrevert = new AddRevert(argu, this->m_language, this); //modify by 20110714
    connect(m_addrevert, SIGNAL(revertpart(QString)), this, SLOT(addRvertPart(QString)));
    return;
}

void PartitionTool::addRvertPart(QString qsRevertpart)
{
    //check the partition size
    bool isCanBeRevert;
    for(int i = 0; i < m_nPartitionNum; i ++)
    {
        if(m_diskPartitionInfo[i].strPartitionName == qsRevertpart.toStdString())
        {
            isCanBeRevert = compareSize(m_diskPartitionInfo[i].strSize, "4999");
            if(!isCanBeRevert)
            {
                QMessageBox msgBox(this);
                if(this->m_language)
                {
                    msgBox.setText(tr("分区大小不符合根分区需要,请重新设置！"));
                    msgBox.addButton(tr("确定"), QMessageBox::ActionRole);
                    msgBox.setWindowTitle("警告");
                }
                else
                {
                    msgBox.setText(tr("the partition is small for revert filesystem！"));
                    msgBox.addButton(tr("Ok"), QMessageBox::ActionRole);
                    msgBox.setWindowTitle("Warnning");
                }
                msgBox.exec();
                return;
            }
        }
    }
    //
    m_qsRevertPartition = qsRevertpart;
    m_addrevert->close();
}

bool PartitionTool::setLanguage(bool language)
{
    m_language = language;
   if(language)
   {
       m_qsFreeSize = tr("freesize");
       partAutoLabel->setText(tr("Automatic partitioning"));
       
       diskSelLabel->setText(tr("select disk:"));
       if(Platform == X86)
       {
            partAutoRadioButton->setText(tr("Auto Partition"));
            partManRadioButton->setText(tr("Partition by Hand"));
       }
       m_addPartitionButton->setText(tr("Add"));
       m_delPartitionButton->setText(tr("Delete"));
       m_qpAddRevert->setText(tr("setRevert"));
       //expertPushButton->setText(tr("Expert Partition"));
       setTitle(tr("分区"));
       m_labels.clear();
       m_labels << tr("设备") <<tr("文件系统") <<tr("挂载点") <<tr("大小") << tr("分区格式");
   }
   else
   {
       m_qsFreeSize = tr("freesize");
       partAutoLabel->setText(tr("Automatic partitioning"));
       diskSelLabel->setText(tr("select disk:"));
       if(Platform == X86)
       {
            partAutoRadioButton->setText(tr("Auto Partition"));
            partManRadioButton->setText(tr("Partition by Hand"));
       }
       m_addPartitionButton->setText(tr("Add"));
       m_delPartitionButton->setText(tr("Delete"));
       m_qpAddRevert->setText(tr("setRevert"));
       //expertPushButton->setText(tr("Expert Partition"));
       setTitle(tr("Partition"));
       m_labels.clear();
       m_labels << tr("Device") <<tr("systemType") <<tr("Mount") <<tr("Size") << tr("partType");
   }
   diskInfoTableWidget->setHorizontalHeaderLabels(m_labels);
   for(int i=diskInfoTableWidget->rowCount()-1; i>=0; i--)
   {
       diskInfoTableWidget->removeRow(i);
   }
   updateDiskTableWidget(false);
}

void PartitionTool::bootGpartedExec()
{
    QString qsBootGparted = tr("/usr/sbin/gparted");
    if(QProcess::execute(qsBootGparted) != 0)
    {
        QMessageBox msgBox;
        if(m_language)
        {
            msgBox.setText("启动高级分区功能未成功!");
           // msgBox.defaultButton()->setText("确定");
        }
        else
        {
            msgBox.setText("gparted is not OK!");
        }
        msgBox.exec();
        return;
    }
}


void PartitionTool::clearPartTable()
{
    for(int i = 0; i < m_nPartitionNum; i ++)
    {
        m_diskPartitionInfo[i].strDeviceSize.clear();
        m_diskPartitionInfo[i].strEnd.clear();
        m_diskPartitionInfo[i].strMountPoint.clear();
        m_diskPartitionInfo[i].strPartitionDevice.clear();
        m_diskPartitionInfo[i].strPartitionName.clear();
        m_diskPartitionInfo[i].strPartitionType.clear();
        m_diskPartitionInfo[i].strSize.clear();
        m_diskPartitionInfo[i].strStart.clear();
        m_diskPartitionInfo[i].strSystemType.clear();
    }
}
//重新向tablewidget中添加数据
void PartitionTool::updateDiskTableWidget(bool isGetPartInfo)
{
    m_qsCurrentDevice = diskSelComboBox->currentText();
    string strCurrentDevice = m_qsCurrentDevice.toStdString();

    //清空tablewidget
    int nRow = diskInfoTableWidget->rowCount();
    for(int i = 0; i < nRow; i ++)
    {
        diskInfoTableWidget->removeRow(i);
    }
    
    //获得硬盘信息
    if(isGetPartInfo)
    {
        QString getPartTxtCmd = tr("/etc/gaea/scripts/getinfo.sh");
        QStringList arguments;
        QString qsDevice = tr("/dev/");
        qsDevice.append(m_qsCurrentDevice);
        arguments.append(qsDevice);
        qDebug() << qsDevice;
        if(QProcess::execute(getPartTxtCmd, arguments) != 0)
        {
          qDebug() << "create partition table failed";
             return;
        }

        m_diskInfo->getpartition();
        m_diskInfo->getPartitionInfo(strCurrentDevice);

        m_nPartitionNum = m_diskInfo->getPartitionNum();
        for(int i = 0; i < m_nPartitionNum;i ++)
        {
           m_diskInfo->partitioninfo(m_diskPartitionInfo[i], i);
        }
    }
    int nInSert = 0;
    string strFreeSize;
    QString qsFreeSize;
    int nLastInsert = 0;
    int nPrintOrder = 0;
    for(int i = 0; i < m_nPartitionNum; i ++)
    {
            for(int j = 0; j < m_nPartitionNum; j ++)
            {
                if((nPrintOrder == m_diskPartitionInfo[j].nPrintfOrder) && (strCurrentDevice == m_diskPartitionInfo[j].strPartitionDevice)
                        && m_diskPartitionInfo[j].strPartitionName.size() != 3)
                {
                    if(i > 0)
                    {
                       strFreeSize.clear();
                       qsFreeSize.clear();
                       strFreeSize =m_diskInfo->getRedues(m_diskPartitionInfo[j].strStart, m_diskPartitionInfo[nLastInsert].strEnd);
                       if(strFreeSize != "0")
                       {
                          if(diskInfoTableWidget->item(nInSert - 1, 4) != 0)
                          {
                            QString qsPartType = diskInfoTableWidget->item(nInSert - 1, 4)->text();
                            string strPartType = qsPartType.toStdString();
                            if(strncmp(strPartType.c_str(), "logical", 6))
                            {
                                 diskInfoTableWidget->insertRow(nInSert);
                                 diskInfoTableWidget->setItem(nInSert, 0, new QTableWidgetItem(m_qsFreeSize));

                                 string strShowSize = strFreeSize;
                                 showSize(strShowSize);
                                 qsFreeSize = QString::fromStdString(strShowSize);
                                 diskInfoTableWidget->setItem(nInSert ++, 3, new QTableWidgetItem(qsFreeSize));
                            }
                          }
                       }
                    }else if(i == 0)
                    {
                        strFreeSize.clear();
                        qsFreeSize.clear();
                        strFreeSize = m_diskPartitionInfo[j].strStart;
                        qsFreeSize = QString::fromStdString(strFreeSize);
                        cout << "strFreeSize = " << strFreeSize;
                        int nLen = strFreeSize.size();
                        string strUits = strFreeSize.substr(nLen - 2, 2);
                        string strSize = strFreeSize.substr(0, nLen - 2);
                        int nSize = m_diskInfo->stringtoint(strSize, false);
                        if(nSize > 20)
                        {
                           diskInfoTableWidget->insertRow(nInSert);
                           diskInfoTableWidget->setItem(nInSert, 0, new QTableWidgetItem(m_qsFreeSize));

                           string strShowSize = strFreeSize;
                           showSize(strShowSize);
                           qsFreeSize = QString::fromStdString(strShowSize);
                           diskInfoTableWidget->setItem(nInSert ++, 3, new QTableWidgetItem(qsFreeSize));
                        }
                    }
                    if(m_diskPartitionInfo[j].strPartitionName.size() != 3)
                    {
                         diskInfoTableWidget->insertRow(nInSert);
                         if(m_diskPartitionInfo[j].strPartitionName.size() != 0)
                            diskInfoTableWidget->setItem(nInSert, 0, new QTableWidgetItem(QString::fromStdString(m_diskPartitionInfo[j].strPartitionName)));
                         if(m_diskPartitionInfo[j].strSystemType.size() != 0)
                            diskInfoTableWidget->setItem(nInSert, 1, new QTableWidgetItem(QString::fromStdString(m_diskPartitionInfo[j].strSystemType)));
                         if(m_diskPartitionInfo[j].strMountPoint.size() != 0)
                            diskInfoTableWidget->setItem(nInSert, 3, new QTableWidgetItem(QString::fromStdString(m_diskPartitionInfo[j].strMountPoint)));
                         if(m_diskPartitionInfo[j].strSize.size() != 0)
                         {
                            string strShowSize = m_diskPartitionInfo[j].strSize;
                            showSize(strShowSize);
                            diskInfoTableWidget->setItem(nInSert, 3, new QTableWidgetItem(QString::fromStdString(strShowSize)));
                         }
                         if(m_diskPartitionInfo[j].strPartitionType.size() != 0)
                         {
                            diskInfoTableWidget->setItem(nInSert ++, 4, new QTableWidgetItem(QString::fromStdString(m_diskPartitionInfo[j].strPartitionType)));
                         }
                         if(strncmp(m_diskPartitionInfo[j].strPartitionType.c_str(), "logical", 7))
                            nLastInsert = j;
                         nPrintOrder ++;
                         break;
                    }
                }
            }
    }
    m_diskInfo->getLastFreeSize(nLastInsert, strFreeSize);
    if(strFreeSize != "0")
    {
       diskInfoTableWidget->insertRow(nInSert);
       diskInfoTableWidget->setItem(nInSert, 0, new QTableWidgetItem(m_qsFreeSize));
       string strShowSize = strFreeSize;
       showSize(strShowSize);
       diskInfoTableWidget->setItem(nInSert, 3, new QTableWidgetItem(QString::fromStdString(strShowSize)));
    }
}

void PartitionTool::clearMountPoint()
{
    for(int i = 0; i < m_nPartitionNum; i ++)
    {
        if(m_diskPartitionInfo[i].strMountPoint.size() != 0)
            m_diskPartitionInfo[i].strMountPoint.clear();
        if(m_diskPartitionInfo[i].strPartitionType.size() != 0)
            m_diskPartitionInfo[i].strMountPoint.clear();
    }
}

void PartitionTool::showSize(string &showSize)
{
    m_diskInfo->showSize(showSize);
}

void PartitionTool::setPartitionButtionOn(int row, int column)
{
    if(diskInfoTableWidget->item(row, 0) == 0)
        return;
    QString qsCurrentPart = diskInfoTableWidget->item(row, 0)->text();
    string  strCurrentPart = qsCurrentPart.toStdString();
    if(!(strncmp(strCurrentPart.c_str(), "freesize", 8)))
    {
        m_addPartitionButton->setEnabled(true);
        m_delPartitionButton->setEnabled(false);
    }
    else if(!(strncmp(strCurrentPart.c_str(), "剩余空间", 4)))
    {
        m_addPartitionButton->setEnabled(true);
        m_delPartitionButton->setEnabled(false);
    }

    else
    {
        m_addPartitionButton->setEnabled(false);
        m_delPartitionButton->setEnabled(true);
    }

    if(!diskInfoTableWidget->item(row, 4) == 0)
    {
        QString qsExtend = diskInfoTableWidget->item(row, 4)->text();
        string  strExtend = qsExtend.toStdString();
        if(!strncmp(strExtend.c_str(), "extended", 8))
        {
            m_addPartitionButton->setEnabled(true);
        }
    }
}

void PartitionTool::checkRootSys(QString mountPoint, QString type)
{
    //check wheather can mount point
    for(int i = 0; i < this->m_nPartitionNum; i ++)
    {

        if(m_diskPartitionInfo[i].strMountPoint.size() != 0)
        {
            if(m_diskPartitionInfo[i].strMountPoint == mountPoint.toStdString())
            {
                QMessageBox *qmessage = new QMessageBox();
                if(!m_language)
                {
                    qmessage->setText(tr("can not mount same point!"));
                }
                else
                {
                    qmessage->setText(tr("不能设置相同挂载点!"));
                }
                qmessage->exec();
                return;
            }
        }
    }

    int nCurrentRow = diskInfoTableWidget->currentRow();
    QString qsCurrentRowItem = diskInfoTableWidget->item(nCurrentRow, 0)->text();

    qDebug() << "ren debug qsCurrentRowItem: " << qsCurrentRowItem;
    qDebug() << "ren debug mountPoint: " << mountPoint;
    qDebug() << "ren debug type: " << type;

    for(int i = 0; i < m_nPartitionNum; i ++)
    {
        if(!qsCurrentRowItem.compare(QString::fromStdString(m_diskPartitionInfo[i].strPartitionName)))
        {
            m_diskPartitionInfo[i].strMountPoint = mountPoint.toStdString();
            m_diskPartitionInfo[i].strSystemType = type.toStdString();
        }
    }

    bool isPartOk = false;
    for(int i = 0; i < m_nPartitionNum; i ++)
    {
        if(m_diskPartitionInfo[i].strMountPoint == "/")
        {
            //检测是否能作根分区
            bool isCanBeRoot = compareSize(m_diskPartitionInfo[i].strSize, "4999");
            if(!isCanBeRoot)
            {
                QMessageBox msgBox(this);
                if(this->m_language)
                {
                    msgBox.setText(tr("分区大小不符合根分区需要！"));
                    msgBox.addButton(tr("确定"), QMessageBox::ActionRole);
                    msgBox.setWindowTitle("警告");
                }
                else
                {
                    msgBox.setText(tr("the partition is small for root filesystem！"));
                    msgBox.addButton(tr("Ok"), QMessageBox::ActionRole);
                    msgBox.setWindowTitle("Warnning");
                }
                msgBox.exec();
                m_diskPartitionInfo[i].strMountPoint.clear();
                return;
            }
            isPartOk = true;
        }
        if(m_diskPartitionInfo[i].strMountPoint == "boot")
        {
            //检测是否能做/boot分区
            bool isCanBeRoot = compareSize(m_diskPartitionInfo[i].strSize, "199");
            if(!isCanBeRoot)
            {
                QMessageBox msgBox(this);
                if(this->m_language)
                {
                    msgBox.setText(tr("分区大小不符合启动分区需要！"));
                    msgBox.addButton(tr("确定"), QMessageBox::ActionRole);
                    msgBox.setWindowTitle("警告");
                }
                else
                {
                    msgBox.setText(tr("the partition is small for boot filesystem！"));
                    msgBox.addButton(tr("Ok"), QMessageBox::ActionRole);
                    msgBox.setWindowTitle("Warnning");
                }
                msgBox.exec();
                m_diskPartitionInfo[i].strMountPoint.clear();
                return;
            }
        }
    }

    //update table widget
    diskInfoTableWidget->setItem(nCurrentRow, 1, new QTableWidgetItem(type));
    diskInfoTableWidget->setItem(nCurrentRow, 2, new QTableWidgetItem(mountPoint));

    if(isPartOk)
        emit partitedOk();
    else
        emit partitedNotOk();
}

//检测分区大小
bool PartitionTool::compareSize(string size, string compareSize)
{
    string strSize;
    int nSize;
    int nCompareSize;
    string strUnits;
    int nLen = size.size();
    strSize = size.substr(0, nLen - 2);
    nSize = m_diskInfo->stringtoint(strSize, false);
    nCompareSize = m_diskInfo->stringtoint(compareSize, false);
    strUnits = size.substr(nLen - 2, 2);
    if(strUnits == "GB")
    {
        nSize *= 1000;
    }
    if(strUnits == "KB")
    {
        return false;
    }
    if(nSize <= nCompareSize)
    {
        return false;
    }
    return true;
}

void PartitionTool::checkCanNext()
{
    bool isPartOk = false;
    if(Platform == X86)
    {
        if(partAutoRadioButton->isChecked())
        {
            isPartOk = true;
        }
    }
    for(int i = 0; i < m_nPartitionNum; i ++)
    {
        if(m_diskPartitionInfo[i].strMountPoint == "/")
        {
            isPartOk = true;
        }
    }

    if(isPartOk)
        emit partitedOk();
    else
        emit partitedNotOk();
}

//add a partition slot
void PartitionTool::addPartition()
{
      QString qsType = diskInfoTableWidget->item(diskInfoTableWidget->currentRow(), 0)->text();
      qDebug() << qsType;
      m_addPartition = new AddPartitionDialog(qsType);
      m_addPartition->setLanguage(m_language);
      m_addPartition->setQTableWidget(diskInfoTableWidget);
      m_addPartition->setRowColumn(diskInfoTableWidget->currentRow(), diskInfoTableWidget->currentColumn());
      m_addPartition->show();
      //reveive signals when add partition sure
      QObject::connect(this->m_addPartition, SIGNAL(setTableWidget(QString, QString, QString, QString)), this,
                       SLOT(setPartition1(QString, QString, QString, QString)));
}

bool PartitionTool::getPartitioninfo(DiskPartitionInfo &partinfo, int num)
{
    if(num >= m_nPartitionNum)
        return false;
    if(m_diskPartitionInfo[num].strMountPoint.size() == 0)
        return false;
    partinfo.strPartitionDevice = m_diskPartitionInfo[num].strPartitionDevice;
    partinfo.strPartitionName = m_diskPartitionInfo[num].strPartitionName;
    partinfo.strMountPoint = m_diskPartitionInfo[num].strMountPoint;
    partinfo.strSystemType = m_diskPartitionInfo[num].strSystemType;
    partinfo.isFlag = m_diskPartitionInfo[num].isFlag;
    return true;
}

bool PartitionTool::getPartition()
{
    bool err;
    m_diskInfo = new DiskInfo;
    err = m_diskInfo->getpartition();
    if(!err)
        return false;
    m_nPartitionNum = m_diskInfo->getPartitionNum();
    for(int i = 0; i < m_nPartitionNum; i ++)
    {
       m_diskInfo->partitioninfo(m_diskPartitionInfo[i], i);
    }
    return true;
}

void PartitionTool::Change(int row, int column)
{
    if(diskInfoTableWidget->item(row, 0) == 0)
        return;
    //防止在extend分区建立根分区
    if(diskInfoTableWidget->item(row, 4) == 0)
    {
        return;
    }
    else
    {
        QString qsPartType = diskInfoTableWidget->item(row, 4)->text();
        if(qsPartType.startsWith("extend"))
        {
            return;
        }
    }
    QString qsCurrentPartition = diskInfoTableWidget->item(row, 0)->text();
    //
    if((qsCurrentPartition == "freesize") || (qsCurrentPartition == "剩余空间"))
    {
        qDebug() << "ren debug create a new partition";
    }
    else
    {
        m_partitionInfoClass = new PartitionInfo;
        m_partitionInfoClass->setLanguage(m_language);
        m_partitionInfoClass->setModal(true); //模态对话框
        m_partitionInfoClass->show();
        QObject::connect(m_partitionInfoClass, SIGNAL(clickSure(QString, QString)), this, SLOT(checkRootSys(QString, QString)));
    }
}

//when add a partition and clicked sure
void PartitionTool::setPartition(QString qsSize, QString qsMountPoint, QString qsSystemType, QString qsPartedType)
{
    QString qsDevice = tr("/dev/");
    QString qsBeginSize;
    QString qsEndSize;
    QString qsEndPart;
    QStringList arguments;
    string  strEndSize;
    string  strBeginSize;
    string  strPartSize;
    string  strEndPart;

    //determine whether there is a same mount point.
    //
    //get end part
    int nCurrentRow = diskInfoTableWidget->currentRow();
    QString qsTmpText1;
    if(diskInfoTableWidget->item(nCurrentRow, 4) != 0)
    {
        qsTmpText1 = diskInfoTableWidget->item(nCurrentRow, 4)->text();
    }
    //if((nCurrentRow == 0) && (diskInfoTableWidget->item(nCurrentRow, 4)->text()) != tr("extended"))
    //if((nCurrentRow == 0) && (diskInfoTableWidget->item(nCurrentRow, 4)) == 0)
    if(nCurrentRow == 0)
    {
        if(qsPartedType == tr("logical"))
        {
            if((diskInfoTableWidget->item(nCurrentRow, 4)->text()) != tr("extended"))
            {
                   cout << "no extended part" << endl;
                   return;
            }
            QString qsTmpText;
            if(diskInfoTableWidget->item(nCurrentRow+1, 4) != 0)
            {
                qsTmpText = diskInfoTableWidget->item(nCurrentRow+1, 4)->text();
            }
            if(diskInfoTableWidget->item(nCurrentRow+1, 4) != 0)
            {
                QString qsAddCurrPart = diskInfoTableWidget->item(nCurrentRow+1, 4)->text();
                string  strAddCurrPart = qsAddCurrPart.toStdString();
                int nRowCout = diskInfoTableWidget->rowCount();
                while((!strncmp(strAddCurrPart.c_str(), "logical", 6)) && (nCurrentRow < nRowCout))
                {
                    nCurrentRow ++;
                    if(diskInfoTableWidget->item(nCurrentRow+1, 4) == 0)
                        break;
                    qsAddCurrPart = diskInfoTableWidget->item(nCurrentRow+1, 4)->text();
                    strAddCurrPart = qsAddCurrPart.toStdString();
                }
                nCurrentRow ++;

                if(diskInfoTableWidget->item(nCurrentRow-1, 0) != 0)
                    qsEndPart = diskInfoTableWidget->item(nCurrentRow - 1, 0)->text();
                QString qsEndPartType;
                string strEndPartType;
                if(diskInfoTableWidget->item(nCurrentRow - 1, 0) != 0)
                {
                    qsEndPartType = diskInfoTableWidget->item(nCurrentRow - 1, 4)->text();
                    strEndPartType = qsEndPartType.toStdString();
                }
                strEndPart = qsEndPart.toStdString();
                for(int i = 0; i < m_nPartitionNum; i ++)
                {
                    if(strEndPart == m_diskPartitionInfo[i].strPartitionName)
                    {
                        if(!strncmp(strEndPartType.c_str(), "extended", 8))
                        {
                            qsBeginSize = tr("2MB");//QString::fromStdString(m_diskPartitionInfo[i].strStart);
                            strBeginSize = "2MB";
                        }
                        else
                        {
                            qsBeginSize = QString::fromStdString(m_diskPartitionInfo[i].strEnd);
                            strBeginSize = m_diskPartitionInfo[i].strEnd;
                            int nTmp = m_diskInfo->stringtoint(strBeginSize, false);
                            nTmp += 1;
                            strBeginSize.clear();
                            m_diskInfo->inttostring(nTmp, strBeginSize);
                            strBeginSize += "MB";
                        }
                    }
                }
            }
            else
            {
               qsBeginSize = tr("0");
            }
       }
    }
    else
    {
        if(qsPartedType == tr("logical"))
        {
            if((diskInfoTableWidget->item(nCurrentRow, 4)->text()) != tr("extended"))
            {
                   cout << "no extended part" << endl;
                   return;
            }
            QString qsTmpText;
            if(diskInfoTableWidget->item(nCurrentRow+1, 4) != 0)
            {
                qsTmpText = diskInfoTableWidget->item(nCurrentRow+1, 4)->text();
            }
            if(diskInfoTableWidget->item(nCurrentRow+1, 4) != 0)
            {
                QString qsAddCurrPart = diskInfoTableWidget->item(nCurrentRow+1, 4)->text();
                string  strAddCurrPart = qsAddCurrPart.toStdString();
                int nRowCout = diskInfoTableWidget->rowCount();
                while((!strncmp(strAddCurrPart.c_str(), "logical", 6)) && (nCurrentRow < nRowCout))
                {
                    nCurrentRow ++;
                    if(diskInfoTableWidget->item(nCurrentRow+1, 4) == 0)
                        break;
                    qsAddCurrPart = diskInfoTableWidget->item(nCurrentRow+1, 4)->text();
                    strAddCurrPart = qsAddCurrPart.toStdString();
                }
                nCurrentRow ++;
            }
        }else
        {
            if(diskInfoTableWidget->item(nCurrentRow-1, 4) != 0)
            {
                QString qsreduCurrPart = diskInfoTableWidget->item(nCurrentRow-1, 4)->text();
                string  strreduCurrPart = qsreduCurrPart.toStdString();
                while((!strncmp(strreduCurrPart.c_str(), "logical", 6)) && (nCurrentRow > 0))
                {
                    nCurrentRow --;
                    if(diskInfoTableWidget->item(nCurrentRow-1, 4) == 0)
                        break;
                    qsreduCurrPart = diskInfoTableWidget->item(nCurrentRow-1, 4)->text();
                    strreduCurrPart = qsreduCurrPart.toStdString();
                }
            }
        }
        qsEndPart = diskInfoTableWidget->item(nCurrentRow - 1, 0)->text();
        strEndPart = qsEndPart.toStdString();


        QString qsEndPartType;
        string strEndPartType;
        if(diskInfoTableWidget->item(nCurrentRow - 1, 0) != 0)
        {
            qsEndPartType = diskInfoTableWidget->item(nCurrentRow - 1, 4)->text();
            strEndPartType = qsEndPartType.toStdString();
        }
        for(int i = 0; i < m_nPartitionNum; i ++)
        {
            if(strEndPart == m_diskPartitionInfo[i].strPartitionName)
            {
                if(!(strncmp(strEndPartType.c_str(), "extended", 8)) && (qsPartedType == tr("logical")))
                {
                    qsBeginSize = QString::fromStdString(m_diskPartitionInfo[i].strStart);
                    strBeginSize = qsBeginSize.toStdString();
                    int nTmp = m_diskInfo->stringtoint(strBeginSize, false);
                    nTmp += 1;
                    strBeginSize.clear();
                     m_diskInfo->inttostring(nTmp, strBeginSize);
                     strBeginSize += "MB";
                }
                else
                {
                    qsBeginSize = QString::fromStdString(m_diskPartitionInfo[i].strEnd);
                    strBeginSize = m_diskPartitionInfo[i].strEnd;
                    int nTmp = m_diskInfo->stringtoint(strBeginSize, false);
                    if(qsPartedType == tr("logical"))
                    {
                        nTmp += 2;
                    }
                    else
                    {
                        nTmp += 1;
                    }
                    strBeginSize.clear();
                    m_diskInfo->inttostring(nTmp, strBeginSize);
                    strBeginSize += "MB";
                }
            }
        }
    }


    cout << "strEndPart = " << strEndPart << endl;
    QString qsBeginSizeTmp;
    if(qsEndPart.size() == 0)
    {
        if(qsPartedType == tr("logical"))
        {
            strBeginSize = "2MB";
            qsBeginSize = tr("2");
        }
        else
        {
            strBeginSize = "1MB";
            qsBeginSize = tr("1");
        }
        qsBeginSizeTmp = qsBeginSize;
    }
    else
    {
        qsBeginSizeTmp = QString::fromStdString(strBeginSize);
        m_diskInfo->changeToMb(strBeginSize);
        qsBeginSize.clear();
        int len = strBeginSize.size();
        string strBeginSizesub = strBeginSize.substr(0, len - 2);
        qsBeginSize = QString::fromStdString(strBeginSizesub);
    }
    strPartSize = qsSize.toStdString();
    strEndSize = m_diskInfo->addMtoM(strBeginSize, strPartSize);
    qsEndSize = QString::fromStdString(strEndSize);

    qDebug() << m_qsCurrentDevice << qsDevice <<qsPartedType << qsBeginSize << qsEndSize;
    qsDevice.append(m_qsCurrentDevice);
    arguments.append(qsDevice);

    arguments.append(qsPartedType);
    arguments.append(qsBeginSize);
    arguments.append(qsEndSize);

    //执行新增分区脚本
    bool err = addPartitionExec(arguments);
    if(!err)
        return;
    //添加挂载以及文件系统信息

    //更新分区表
    for(int i=diskInfoTableWidget->rowCount()-1; i>=0; i--)
    {
        diskInfoTableWidget->removeRow(i);
    }
    clearPartTable();
    updateDiskTableWidget(true);

    bool isPartOk = false;
    for(int i = 0; i < m_nPartitionNum; i ++)
    {
        if(m_diskPartitionInfo[i].strMountPoint == "/")
        {
            isPartOk = true;
        }
    }
    if(isPartOk)
        emit partitedOk();
    else
        emit partitedNotOk();
}

//******************************************************************************************
void PartitionTool::setPartition1(QString qsSize, QString qsMountPoint, QString qsSystemType, QString qsPartedType)
{
    QString qsBeginSize;
    string strBeginSize;
    QString qsDevice = tr("/dev/");
    QString qsCurrentDevice;
    QString qsEndSize;
    string  strEndPart;
    int nCurrentRow = diskInfoTableWidget->currentRow();
    int nDiskRow = diskInfoTableWidget->rowCount();
    qsPartedType .trimmed();
    if(nCurrentRow == 0)
    {
        if(qsPartedType == tr("logical"))
        {
            if(nDiskRow == 1)
            {
                qsBeginSize = "2MB";
                strBeginSize = "2MB";
            }
            else
            {
                if(diskInfoTableWidget->item(1, 4) != 0)
                    qsCurrentDevice = diskInfoTableWidget->item(1, 4)->text();
                if(qsCurrentDevice.startsWith("logical", Qt::CaseSensitive))
                {
                    int nLogical = 0;
                    QString qsDevice;
                    qsDevice = diskInfoTableWidget->item(nLogical, 0)->text();
                    strEndPart = qsDevice.toStdString();
                    while(qsCurrentDevice.startsWith("logical", Qt::CaseSensitive))
                    {
                        if((nDiskRow-1) == nLogical)
                        {
                            if(diskInfoTableWidget->item(nLogical, 4) != 0)
                                qsCurrentDevice = diskInfoTableWidget->item(nLogical, 4)->text();
                            if(qsCurrentDevice.startsWith("logical", Qt::CaseSensitive))
                            {
                                QString qsDevice;
                                qsDevice = diskInfoTableWidget->item(nLogical, 0)->text();
                                strEndPart = qsDevice.toStdString();
                                qsCurrentDevice.clear();
                            }
                            break;
                        }
                        nLogical ++;
                        qsCurrentDevice.clear();
                        if(diskInfoTableWidget->item(nLogical, 4) != 0)
                            qsCurrentDevice = diskInfoTableWidget->item(nLogical, 4)->text();
                        if(qsCurrentDevice.startsWith("logical", Qt::CaseSensitive))
                        {
                            QString qsDevice;
                            qsDevice = diskInfoTableWidget->item(nLogical, 0)->text();
                            strEndPart = qsDevice.toStdString();
                        }
                    }
                    for(int i = 0; i < m_nPartitionNum; i ++)
                    {
                        if(strEndPart == m_diskPartitionInfo[i].strPartitionName)
                        {
                            if(m_diskPartitionInfo[i].strPartitionType == "extended")
                            {
                                strBeginSize = m_diskPartitionInfo[i].strStart;
                            }
                            else
                            {
                                strBeginSize = m_diskPartitionInfo[i].strEnd;
                            }
                            int nTmp = m_diskInfo->stringtoint(strBeginSize, false);
                            nTmp += 1;
                            strBeginSize.clear();
                            m_diskInfo->inttostring(nTmp, strBeginSize);
                            strBeginSize += "MB";
                            qsBeginSize = QString::fromStdString(strBeginSize);
                        }
                    }
                }
                else
                {
                    qsBeginSize = "2MB";
                    strBeginSize = "2MB";
                }
            }
        }
        else
        {
            qsBeginSize = "1MB";
            strBeginSize = "1MB";
        }
    }
    else
    {
        if(qsPartedType.startsWith("logical", Qt::CaseSensitive))
        {
            if(nCurrentRow == (nDiskRow-1))
            {
                if(diskInfoTableWidget->item(nCurrentRow, 4) != 0)
                {
                    qsCurrentDevice = diskInfoTableWidget->item(nCurrentRow, 4)->text();
                    QString qsDevice = diskInfoTableWidget->item(nCurrentRow, 0)->text();
                    strEndPart = qsDevice.toStdString();
                }
                for(int i = 0; i < m_nPartitionNum; i ++)
                {
                    if(strEndPart == m_diskPartitionInfo[i].strPartitionName)
                    {
                        if(m_diskPartitionInfo[i].strPartitionType == "extended")
                        {
                            strBeginSize = m_diskPartitionInfo[i].strStart;
                        }
                        else
                        {
                            strBeginSize = m_diskPartitionInfo[i].strEnd;
                        }
                        int nTmp = m_diskInfo->stringtoint(strBeginSize, false);
                        nTmp += 1;
                        strBeginSize.clear();
                        m_diskInfo->inttostring(nTmp, strBeginSize);
                        strBeginSize += "MB";
                        qsBeginSize = QString::fromStdString(strBeginSize);
                    }
                }
            }
            else
            {
                if(diskInfoTableWidget->item(nCurrentRow + 1, 4) != 0)
                {
                    qsCurrentDevice = diskInfoTableWidget->item(nCurrentRow + 1, 4)->text();
                    QString qsDevice = diskInfoTableWidget->item(nCurrentRow + 1, 0)->text();
                    strEndPart = qsDevice.toStdString();
                }
                if(qsCurrentDevice.startsWith("logical", Qt::CaseSensitive))
                {
                    int nLogical = nCurrentRow+1;
                    while(qsCurrentDevice.startsWith("logical", Qt::CaseSensitive))
                    {
                        if((nDiskRow-1) == nLogical)
                        {
                            if(diskInfoTableWidget->item(nLogical, 4) != 0)
                                qsCurrentDevice = diskInfoTableWidget->item(nLogical, 4)->text();
                            if(qsCurrentDevice.startsWith("logical", Qt::CaseSensitive))
                            {
                                QString qsDevice = diskInfoTableWidget->item(nLogical, 0)->text();
                                strEndPart = qsDevice.toStdString();
                            }
                            break;
                        }
                        nLogical ++;
                        qsCurrentDevice.clear();
                        if(diskInfoTableWidget->item(nLogical, 4) != 0)
                            qsCurrentDevice = diskInfoTableWidget->item(nLogical, 4)->text();
                        if(qsCurrentDevice.startsWith("logical", Qt::CaseSensitive))
                        {
                            QString qsDevice = diskInfoTableWidget->item(nLogical, 0)->text();
                            strEndPart = qsDevice.toStdString();
                        }
                    }
                    for(int i = 0; i < m_nPartitionNum; i ++)
                    {
                        if(strEndPart == m_diskPartitionInfo[i].strPartitionName)
                        {
                            if(m_diskPartitionInfo[i].strPartitionType == "extended")
                            {
                                strBeginSize = m_diskPartitionInfo[i].strStart;
                            }
                            else
                            {
                                strBeginSize = m_diskPartitionInfo[i].strEnd;
                            }
                            int nTmp = m_diskInfo->stringtoint(strBeginSize, false);
                            nTmp += 1;
                            strBeginSize.clear();
                            m_diskInfo->inttostring(nTmp, strBeginSize);
                            strBeginSize += "MB";
                            qsBeginSize = QString::fromStdString(strBeginSize);
                        }
                    }
                }
                else
                {
                    qsCurrentDevice = diskInfoTableWidget->item(nCurrentRow, 0)->text();
                    qsCurrentDevice.trimmed();
                    strEndPart = qsCurrentDevice.toStdString();
                    for(int i = 0; i < m_nPartitionNum; i ++)
                    {
                        if(strEndPart == m_diskPartitionInfo[i].strPartitionName)
                        {
                            strBeginSize = m_diskPartitionInfo[i].strStart;
                            int nTmp = m_diskInfo->stringtoint(strBeginSize, false);
                            nTmp += 1;
                            strBeginSize.clear();
                            m_diskInfo->inttostring(nTmp, strBeginSize);
                            strBeginSize += "MB";
                            qsBeginSize = QString::fromStdString(strBeginSize);
                        }
                    }
                }
            }
        }
        else
        {
            QString qsCurrentDeviceType = diskInfoTableWidget->item(nCurrentRow - 1, 4)->text();
            qsCurrentDeviceType.remove("\n");
            string strCurrentDeviceType;
            qsCurrentDevice = diskInfoTableWidget->item(nCurrentRow - 1, 0)->text();
            qsCurrentDevice.remove("\n");
            qsCurrentDevice.trimmed();
            qsCurrentDeviceType.trimmed();
            //if(qsCurrentDeviceType == tr("logical"))
            if(qsCurrentDeviceType.startsWith("logical", Qt::CaseSensitive))
            {
                int nLogical = nCurrentRow - 1;
                while(qsCurrentDeviceType.startsWith("logical", Qt::CaseSensitive))
                {
                    if(nLogical == 0)
                    {
                        qsCurrentDevice = diskInfoTableWidget->item(nLogical, 0)->text();
                        qsCurrentDevice.trimmed();
                        qsCurrentDevice.remove("\n");
                        strEndPart = qsCurrentDevice.toStdString();
                        qsCurrentDeviceType.clear();
                        qsCurrentDeviceType = diskInfoTableWidget->item(nLogical, 4)->text();
                        qsCurrentDeviceType.remove("\n");
                        strCurrentDeviceType = qsCurrentDeviceType.toStdString();
                        break;
                    }
                    qsCurrentDevice = diskInfoTableWidget->item(nLogical, 0)->text();
                    qsCurrentDevice.remove("\n");
                    qsCurrentDevice.trimmed();
                    strEndPart = qsCurrentDevice.toStdString();
                    qsCurrentDeviceType = diskInfoTableWidget->item(nLogical, 4)->text();
                    qsCurrentDeviceType.remove("\n");
                    strCurrentDeviceType = qsCurrentDeviceType.toStdString();
                    nLogical -= 1;
                }
                if(strCurrentDeviceType == "extended")
                {
                    for(int i = 0; i < m_nPartitionNum; i ++)
                    {
                        if(strEndPart == m_diskPartitionInfo[i].strPartitionName)
                        {
                            strBeginSize = m_diskPartitionInfo[i].strEnd;
                            int nTmp = m_diskInfo->stringtoint(strBeginSize, false);
                            nTmp += 1;
                            strBeginSize.clear();
                            m_diskInfo->inttostring(nTmp, strBeginSize);
                            strBeginSize += "MB";
                            qsBeginSize = QString::fromStdString(strBeginSize);
                        }
                    }
                }
                else
                {
                    std::cout << "wrong for up is logical\n";
                }
            }
            else
            {
                if(diskInfoTableWidget->item(nCurrentRow - 1, 4) != 0)
                    qsCurrentDevice = diskInfoTableWidget->item(nCurrentRow - 1, 4)->text();
                int nLogical = nCurrentRow - 1;
                if(qsCurrentDevice.startsWith("logical", Qt::CaseSensitive))
                {
                    while(qsCurrentDevice.startsWith("logical", Qt::CaseSensitive))
                    {
                        if(nLogical == 0)
                        {
                         QString qsDevice = diskInfoTableWidget->item(0, 0)->text();
                         strEndPart = qsDevice.toStdString();
                            break;
                        }
                        else
                        {
                            nLogical --;
                            if(diskInfoTableWidget->item(nLogical, 4) != 0)
                            {
                                qsCurrentDevice.clear();
                                if(diskInfoTableWidget->item(nLogical, 4) != 0)
                                {
                                    qsCurrentDevice = diskInfoTableWidget->item(nCurrentRow - 1, 4)->text();
                                    if(qsCurrentDevice.startsWith("extended", Qt::CaseSensitive))
                                    {
                                        QString qsDevice = diskInfoTableWidget->item(nCurrentRow - 1, 0)->text();
                                        strEndPart = qsDevice.toStdString();
                                        break;
                                    }
                                }
                            }

                        }
                    }
                }
                else
                {
                    if(diskInfoTableWidget->item(nCurrentRow - 1, 4) != 0)
                    {
                        qsCurrentDevice = diskInfoTableWidget->item(nCurrentRow - 1, 4)->text();
                        QString qsDevice = diskInfoTableWidget->item(nCurrentRow - 1, 0)->text();
                        strEndPart = qsDevice.toStdString();
                    }
                }
                for(int i = 0; i < m_nPartitionNum; i ++)
                {
                    if(strEndPart == m_diskPartitionInfo[i].strPartitionName)
                    {
                        strBeginSize = m_diskPartitionInfo[i].strEnd;
                        int nTmp = m_diskInfo->stringtoint(strBeginSize, false);
                        nTmp += 1;
                        strBeginSize.clear();
                        m_diskInfo->inttostring(nTmp, strBeginSize);
                        strBeginSize += "MB";
                        qsBeginSize = QString::fromStdString(strBeginSize);
                    }
                }
            }
        }
    }

    //计算结束点
    string strPartSize = qsSize.toStdString();
    string strEndSize = m_diskInfo->addMtoM(strBeginSize, strPartSize);
    qsEndSize = QString::fromStdString(strEndSize);

     qsDevice.append(m_qsCurrentDevice);
    qDebug() << qsDevice <<qsPartedType << qsBeginSize << qsEndSize;
    QStringList arguments;
    arguments.append(qsDevice);
    arguments.append(qsPartedType);
    arguments.append(qsBeginSize);
    arguments.append(qsEndSize);

    //执行新增分区脚本
    bool err = addPartitionExec(arguments);
    if(!err)
        return;

    //更新分区表
    for(int i=diskInfoTableWidget->rowCount()-1; i>=0; i--)
    {
        diskInfoTableWidget->removeRow(i);
    }
    clearPartTable();
    updateDiskTableWidget(true);

    bool isPartOk = false;
    for(int i = 0; i < m_nPartitionNum; i ++)
    {
        if(m_diskPartitionInfo[i].strMountPoint == "/")
        {
            isPartOk = true;
        }
    }
    if(isPartOk)
        emit partitedOk();
    else
        emit partitedNotOk();
    m_addPartitionButton->setEnabled(false);
    m_delPartitionButton->setEnabled(false); //modify time: 20110715
}
//******************************************************************************************

bool PartitionTool::addPartitionExec(QStringList arguments)
{
    QDesktopWidget* desktop = QApplication::desktop();
    int width = desktop->width();
    int height = desktop->height();
    QMessageBox msgBox(this);
    int x = (width - msgBox.width())/2;
    int y = (height - msgBox.height())/2;
    msgBox.setGeometry(x, y, msgBox.width(), msgBox.height());
    QString creatSH = "/etc/gaea/scripts/handcrt.sh";
    if(QProcess::execute(creatSH, arguments) != 0)
    {
        if(m_language)
        {
            msgBox.setText(tr("创建分区失败！"));
            msgBox.addButton(tr("确定"), QMessageBox::ActionRole);
            msgBox.setWindowTitle("创建分区");
        }
        else
        {
            msgBox.setText(tr("add partition failed!"));
            msgBox.addButton(tr("ok"), QMessageBox::ActionRole);
            msgBox.setWindowTitle("add partition");
            //QMessageBox::information(this, tr("add partition"), tr("add partition failed!"));
        }
        msgBox.exec();
        return false;
    }
    if(m_language)
    {
        msgBox.setText(tr("创建分区成功！"));
        msgBox.addButton(tr("确定"), QMessageBox::ActionRole);
        msgBox.setWindowTitle("创建分区");
       // msgBox.setGeometry();
    }
    else
    {
        msgBox.setText("create partition OK!");
        msgBox.addButton(tr("ok"), QMessageBox::ActionRole);
        msgBox.setWindowTitle("add partition");
        //QMessageBox::information(this, tr("add partition"), tr("create partition OK!"));
    }
    msgBox.exec();
    return true;
}

bool PartitionTool::delPartitionExec(QStringList arguments)
{
    QDesktopWidget* desktop = QApplication::desktop();
    int width = desktop->width();
    int height = desktop->height();
    QMessageBox msgBox(this);
    int x = (width - msgBox.width())/2;
    int y = (height - msgBox.height())/2;
    msgBox.setGeometry(x, y, msgBox.width(), msgBox.height());

    QString deleteCMD = "/etc/gaea/scripts/handdel.sh";
    if(QProcess::execute(deleteCMD, arguments) != 0)
    {
        if(m_language)
        {
            msgBox.setText("删除分区失败!");
            msgBox.addButton(tr("确定"), QMessageBox::ActionRole);
            msgBox.setWindowTitle("删除分区");
        }
        else
        {
            msgBox.setText("delete partition failed!");
            msgBox.setWindowTitle("delete partition");
        }
      msgBox.exec();
      return false;
    }

    if(m_language)
    {
        msgBox.setText("删除分区成功!");
        msgBox.addButton(tr("确定"), QMessageBox::ActionRole);
        msgBox.setWindowTitle("删除分区");
    }
    else
    {
        msgBox.setText("delete partition is OK!");
        msgBox.setWindowTitle("delete partition");
    }
    msgBox.exec();
    return true;
}

void PartitionTool::diskinfoUnshown()
{
    isAutoPartition = true;
    emit partitedOk();
    diskInfoTableWidget->setEnabled(false);
    diskSelComboBox->setEnabled(false);
}

void PartitionTool::diskinfoShown()
{
    bool isEmitSig = false;
    isAutoPartition = false;
    diskInfoTableWidget->setEnabled(true);
    diskSelComboBox->setEnabled(true);
    for(int i = 0; i < m_nPartitionNum; i ++)
    {
        if(m_diskPartitionInfo[i].strMountPoint == "/")
        {
              isEmitSig = true;
        }
    }
    if(isEmitSig)
        emit partitedOk();
    else
        emit partitedNotOk();
}

//当选择不同主硬盘时调用此函数
void PartitionTool::diskinfoShown(int index)
{
    //set add and del partition button false
    m_addPartitionButton->setEnabled(false);
    m_delPartitionButton->setEnabled(false);

    isAutoPartition = false;
    emit partitedNotOk();

    //delete tablewidget row
    for(int i=diskInfoTableWidget->rowCount()-1; i>=0; i--)
    {
        diskInfoTableWidget->removeRow(i);
    }

    //udate tablewidget
    clearPartTable();
    updateDiskTableWidget(true);
}

void PartitionTool::delPartition()
{
    //
    bool del = false;
    int ret;
    if(this->m_language)
    {
        ret = QMessageBox::information(this, tr("删除分区"), tr("你确认删除分区吗？"),tr("确定"), tr("取消") );
    }
    else
    {
        ret = QMessageBox::information(this, tr("delPartition"), tr("Do you want to delete partition?"),tr("Yes"), tr("No"));
    }
       switch (ret) {
             case 0:
                 // Save was clicked
                 del = true;
                 break;
             case 1:
                 // Cancel was clicked
                 break;
             default:
                 // should never be reached
                 break;
             }

    //modify by 20110714
    if(del)
    {
        QStringList arguments;
        int nLen = diskInfoTableWidget->currentRow();
        QString qsDelPart = diskInfoTableWidget->item(nLen, 0)->text();
        QString qsPath = "/dev/";
        qsPath.append(qsDelPart);
        qDebug() << "del: " << qsPath;
        arguments.append(qsPath);
            bool err = delPartitionExec(arguments);
            if(!err)
            {
                cout << "del part failed" << endl;
                return;
            }

            for(int i=diskInfoTableWidget->rowCount()-1; i>=0; i--)
            {
                diskInfoTableWidget->removeRow(i);
            }
        clearPartTable();
        updateDiskTableWidget(true);
    }
}

PartitionInfo::PartitionInfo(QWidget *parent)
    : QDialog(parent)
{
    systemLabel = new QLabel;
    mountLabel = new QLabel;
    formattingLabel = new QLabel;
    systemSelComboBox = new QComboBox;
    mountSelComboBox = new QComboBox;
    //add file system
    if(Platform == X86)
    {
        systemSelComboBox->insertItem(0, "ext4");
        systemSelComboBox->insertItem(1, "ext3");
        systemSelComboBox->insertItem(2, "ext2");
        systemSelComboBox->insertItem(3, "linux-swap");
        systemSelComboBox->insertItem(4, "fat16");
        systemSelComboBox->insertItem(5, "fat32");
        systemSelComboBox->insertItem(6, "ntfs");
    }else if(Platform == LOONGSON)
    {
        systemSelComboBox->insertItem(0, "ext3");
        systemSelComboBox->insertItem(1, "ext2");
        systemSelComboBox->insertItem(2, "linux-swap");
        systemSelComboBox->insertItem(3, "fat16");
        systemSelComboBox->insertItem(4, "fat32");
        systemSelComboBox->insertItem(5, "ntfs");
    }

    mountSelComboBox->insertItem(0, "/");
    mountSelComboBox->insertItem(1, "boot");
    mountSelComboBox->insertItem(2, "usr");
    mountSelComboBox->insertItem(3, "home");
    mountSelComboBox->insertItem(4, "mnt");
    mountSelComboBox->insertItem(4, "none");

    pbSure = new QPushButton;
    pbCancel = new QPushButton;
    formattingCheckBox = new QCheckBox;


    QObject::connect(this->systemSelComboBox, SIGNAL(activated(const QString)), this, SLOT(isSwap(const QString)));
    QObject::connect(this->pbSure,SIGNAL(clicked()),this,SLOT(clickedPbSure()));
    QObject::connect(this->pbCancel,SIGNAL(clicked()),this,SLOT(clickedPbCancel()));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(systemLabel,0,0,1,1);
    layout->addWidget(systemSelComboBox,0,1,1,3);
    layout->addWidget(mountLabel,1,0,1,1);
    layout->addWidget(mountSelComboBox,1,1,1,3);
    //layout->addWidget(formattingCheckBox,2,0,1,1);
    //layout->addWidget(formattingLabel,2,1,1,2);
    layout->addWidget(pbSure,3,2,1,1);
    layout->addWidget(pbCancel,3,3,1,1);

    QDesktopWidget* desktop = QApplication::desktop();
    int width = desktop->width();
    int height = desktop->height();
    int x = (width - 300)/2;
    int y = (height - 160)/2;
    this->setGeometry(x, y, 300, 160);
    QPalette   palette;
    palette.setColor(QPalette::Background, QColor(240,248,255));
    this->setPalette(palette);

    setLayout(layout);
    setWindowTitle(tr("add partiton"));
}

void PartitionInfo::isSwap(const QString &text)
{
   if((text == tr("linux-swap")) || (text == tr("fat16")) || (text == tr("fat32"))
            || ((text == tr("ntfs"))))
   {
       mountSelComboBox->setCurrentIndex(4);
       mountSelComboBox->setEnabled(false);
   }
   else
   {
       if(!mountSelComboBox->isEnabled())
       {
           mountSelComboBox->setEnabled(true);
           mountSelComboBox->setCurrentIndex(0);
       }
   }
}

void PartitionInfo::setLanguage(bool language)
{
    if(language)
    {
//        tor->load("/ect/gaea/trans/zh.qm");
//        qApp->installTranslator(tor);

        systemLabel->setText(tr("文件系统:"));
        mountLabel->setText(tr("挂载点:"));
        formattingLabel->setText(tr("自动格式化"));
        pbSure->setText(tr("确定"));
        pbCancel->setText(tr("取消"));
        setWindowTitle(tr("文件系统"));
    }
    else
    {
//        tor->load("/etc/gaea/trans/en.qm");
//        qApp->installTranslator(tor);

        systemLabel->setText(tr("Type:"));
        mountLabel->setText(tr("Mount:"));
        formattingLabel->setText(tr("Auto format"));
        pbSure->setText(tr("Sure"));
        pbCancel->setText(tr("Cancel"));
        setWindowTitle(tr("partiton system"));
    }
}

void PartitionInfo::clickedPbCancel()
{
        this->close();
}

void PartitionInfo::clickedPbSure()
{
    emit clickSure(mountSelComboBox->currentText(), systemSelComboBox->currentText());
    this->close();
}


