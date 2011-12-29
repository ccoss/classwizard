#ifndef ADDREVERT_H
#define ADDREVERT_H

#include <QtGui>

class AddRevert : public QDialog
{
    Q_OBJECT
    public:
        AddRevert(QStringList qslPart, bool language, QWidget* parent = 0);
        ~AddRevert(){};

    private:
        QLabel*         qlTitle;
        QLabel*         qlRevertPart;
        QComboBox*      qcRevertPart;

        QString         m_qsRevertPart;
        QString         m_qsRootPart;
        QStringList     m_qslPart;

        QPushButton*    m_sure;
        QPushButton*    m_cancel;
        bool            m_language;

    public slots:
        void            clickedsure();
        void            clickedcancel();
    signals:
            void revertpart(QString qsRevertPart);

};
#endif // ADDREVERT_H
