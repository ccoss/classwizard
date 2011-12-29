#ifndef DIALOG_H_
#define DIALOG_H_

//#include <QtGui>
#include <QTranslator>
#include <QDialog>
#include <QTableWidget>
#include <QLabel>
#include <QComboBox>
#include <QRadioButton>
#include "architecture.h"

class QTableWidgetItem;

extern QTranslator *tor;
extern Architecture Platform;
//extern Architecture Platform;
class AddPartitionDialog : public QDialog
{
	Q_OBJECT
	public:
                AddPartitionDialog(QString qsType, QWidget* parent = 0);
                void setQTableWidget(QTableWidget* tbl);
                void setRowColumn(int row, int column);
                void setLanguage(bool language);
		~AddPartitionDialog(){}

        signals:
                void explains();
                void setTableWidget(QString qsSize, QString qsMountPoint, QString qsSystemType, QString qsPartedType);
        private:
                QLabel*        m_descriptLabel;
                QLabel*        m_partitionMethodLabel;
                QLabel*        m_partitionSizeLabel;
                QLabel*        m_partitionTypeLabel;
                QLabel*        m_partitionMountLabel;

                QLineEdit*     m_partitionSizeLEdit;

                QComboBox*     m_partitionTypeCBox;
                QComboBox*     m_partitionMountCBox;

                QRadioButton*  m_primaryPartitionRButton;
                QRadioButton*  m_extendedPartitionRButton;
                QRadioButton*  m_logicalPartitionRButton;

                QPushButton*   m_sureButton;
                QPushButton*   m_cancelButton;

                QTableWidget*  m_tblwidget;

                int            m_row;
                int            m_column;

            public slots:
                void clickSure();
                void clickCancel();
};

#endif
