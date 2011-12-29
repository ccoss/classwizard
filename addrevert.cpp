#include "diskinfo.h"
#include "addrevert.h"
AddRevert::AddRevert(QStringList qslPart, bool language, QWidget* parent)
    :QDialog(parent)
{
    QDesktopWidget* desktop = QApplication::desktop();
    int width = desktop->width();
    int height = desktop->height();
    int x = (width - 300)/2;
    int y = (height - 150)/2;
    this->setGeometry(x, y, 300, 150);

    if(!language)
    {
        qlTitle = new QLabel(tr("RevertPart"));
        qlRevertPart = new QLabel(tr("revertpart"));
    }
    else
    {
        qlTitle = new QLabel(tr("还原设置"));
        qlRevertPart = new QLabel(tr("还原分区"));
    }
    qcRevertPart = new QComboBox(this);
    qcRevertPart->addItems(qslPart);
    if(!language)
    {
        m_sure = new QPushButton(tr("Sure"));
        m_cancel = new QPushButton(tr("Cancel"));
    }
    else
    {
        m_sure = new QPushButton(tr("确定"));
        m_cancel = new QPushButton(tr("取消"));
    }

    QGridLayout *layout = new QGridLayout();
    layout->addWidget(qlRevertPart, 0, 0, 1, 1);
    layout->addWidget(qcRevertPart, 0, 1, 1, 3);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(m_sure);
    hlayout->addWidget(m_cancel);

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addLayout(layout);
    vlayout->addLayout(hlayout);

    this->setLayout(vlayout);
    if(!language)
    {
        setWindowTitle(tr("SetRevert"));
    }
    else
    {
        setWindowTitle(tr("还原设置"));
    }

    connect(m_sure, SIGNAL(clicked()), this, SLOT(clickedsure()));
    connect(m_cancel, SIGNAL(clicked()), this, SLOT(clickedcancel()));
    this->show();
}

void AddRevert::clickedsure()
{
    //write to file
    QFile file("/etc/gaea/scripts/revert.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return;
    //
    QString currentPartition = qcRevertPart->currentText();
    QTextStream out(&file);
    out << currentPartition << "\n";
    file.close();
    emit revertpart(currentPartition);
}

void AddRevert::clickedcancel()
 {
     this->close();
 }
