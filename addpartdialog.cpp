#include <QDesktopWidget>
#include <QGridLayout>
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>

#include "addpartdialog.h"

AddPartitionDialog::AddPartitionDialog(QString qsType, QWidget* parent)
	:QDialog(parent)
{
    this->setModal(true);
    QDesktopWidget* desktop = QApplication::desktop();
    int width = desktop->width();
    int height = desktop->height();
    int x = (width - 300)/2;
    int y = (height - 150)/2;
    this->setGeometry(x, y, 300, 150);

    m_descriptLabel = new QLabel(tr("Create a new partition:"));
    m_partitionMethodLabel = new QLabel(tr("partition method:"));
    m_partitionSizeLabel = new QLabel(tr("partition size:"));
    m_partitionTypeLabel = new QLabel(tr("partition type:"));
    m_partitionMountLabel = new QLabel(tr("mount point:"));

    m_partitionSizeLEdit = new QLineEdit;
    m_partitionTypeCBox = new QComboBox;

    m_partitionTypeCBox->insertItem(0, "ext2");
    m_partitionTypeCBox->insertItem(4, "fat16");
    m_partitionTypeCBox->insertItem(5, "fat32");
    m_partitionTypeCBox->insertItem(6, "NTFS");
    m_partitionTypeCBox->insertItem(6, "linux-swap");

    m_partitionMountCBox = new QComboBox;
    m_partitionMountCBox->insertItem(0, "/");
    m_partitionMountCBox->insertItem(0, "/boot");
    m_partitionMountCBox->insertItem(0, "/usr");
    m_partitionMountCBox->insertItem(0, "/tmp");
    m_partitionMountCBox->insertItem(0, "/home");
    m_partitionMountCBox->insertItem(0, "/mnt");

    m_primaryPartitionRButton = new QRadioButton("Primary");
    m_extendedPartitionRButton = new QRadioButton("Extended");
    m_logicalPartitionRButton = new QRadioButton("Logical");
    m_primaryPartitionRButton->setChecked(true);


    m_sureButton = new QPushButton(tr("sure"));
    m_cancelButton = new QPushButton(tr("cancel"));

    QGridLayout *layout = new QGridLayout(this);


    layout->addWidget(m_descriptLabel, 0, 0, 1, 1);
    layout->addWidget(m_partitionMethodLabel, 1, 0);
    if(!(qsType == tr("freesize") || qsType == tr("剩余空间")))
    {
        layout->addWidget(m_logicalPartitionRButton, 1, 1);
         m_primaryPartitionRButton->setChecked(false);
         m_logicalPartitionRButton->setChecked(true);
    }
    else
    {
        layout->addWidget(m_primaryPartitionRButton, 1, 1);
        layout->addWidget(m_extendedPartitionRButton, 1, 2);
    }
    layout->addWidget(m_partitionSizeLabel, 2, 0);
    layout->addWidget(m_partitionSizeLEdit, 2, 1, 1, 2);
    //layout->addWidget(m_partitionMountLabel, 3, 0);
    //layout->addWidget(m_partitionMountCBox, 3, 1, 1, 2);
    //layout->addWidget(m_partitionTypeLabel, 4, 0);
    //layout->addWidget(m_partitionTypeCBox, 4, 1, 1, 2);
    layout->addWidget(m_sureButton, 5, 2);
    layout->addWidget(m_cancelButton, 5, 3);
    
    connect(this->m_sureButton, SIGNAL(clicked()), this, SLOT(clickSure()));
    connect(this->m_cancelButton, SIGNAL(clicked()), this, SLOT(clickCancel()));

    setWindowTitle(tr("Create Partition"));
}

void AddPartitionDialog::setLanguage(bool language)
{
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

        m_descriptLabel->setText(tr("Create a new partition:"));
        m_partitionMethodLabel->setText(tr("partition method:"));
        m_partitionSizeLabel->setText(tr("partition size(MB):"));
        m_partitionTypeLabel->setText(tr("partition type:"));
        m_partitionMountLabel->setText(tr("mount point:"));

        m_sureButton->setText(tr("sure"));
        m_cancelButton->setText(tr("cancel"));

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

        m_descriptLabel->setText(tr("Create a new partition:"));
        m_partitionMethodLabel->setText(tr("partition method:"));
        m_partitionSizeLabel->setText(tr("partition size(MB):"));
        m_partitionTypeLabel->setText(tr("partition type:"));
        m_partitionMountLabel->setText(tr("mount point:"));

        m_sureButton->setText(tr("sure"));
        m_cancelButton->setText(tr("cancel"));
    }
}

void AddPartitionDialog::setQTableWidget(QTableWidget *tbl)
{
    m_tblwidget = tbl;
}

void AddPartitionDialog::setRowColumn(int row, int column)
{
    m_row = row;
    m_column = column;
}

void AddPartitionDialog::clickSure()
{
    QString qsSize = m_partitionSizeLEdit->text();
    QString qsMountPoint = m_partitionMountCBox->currentText();
    QString qsSystemType = m_partitionTypeCBox->currentText();
    QString qsPartType;
    if(m_primaryPartitionRButton->isChecked())
        qsPartType = tr("primary");
    if(m_extendedPartitionRButton->isChecked())
        qsPartType = tr("extended");
    if(m_logicalPartitionRButton->isChecked())
        qsPartType = tr("logical");

    emit setTableWidget(qsSize, qsMountPoint, qsSystemType, qsPartType);
    this->close();
}

void AddPartitionDialog::clickCancel()
  {
      this->close();
  }
