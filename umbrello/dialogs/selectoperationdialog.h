/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2014                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

#ifndef SELECTOPERATIONDIALOG_H
#define SELECTOPERATIONDIALOG_H

#include <kdialog.h>

class KComboBox;
class KLineEdit;
class QCheckBox;
class QGroupBox;
class QLabel;
class QRadioButton;
class UMLClassifier;
class UMLView;

/**
 * A dialog used to select an operation.
 *
 * @short A dialog to select an operation.
 * @author Paul Hensgen <phensgen@techie.com>
 * Bugs and comments to umbrello-devel@kde.org or http://bugs.kde.org
 */
class SelectOperationDialog : public KDialog
{
    Q_OBJECT
public:
    SelectOperationDialog(UMLView * parent, UMLClassifier * c, bool enableAutoIncrement = true);
    ~SelectOperationDialog();

    QString getOpText();

    bool isClassOp() const;

    QString getSeqNumber();

    bool autoIncrementSequence();
    void setAutoIncrementSequence(bool state);

    void setSeqNumber(const QString &num);

    void setCustomOp(const QString &op);

    bool setClassOp(const QString &op);

protected:
    void setupOperationsList();

private:
    static const int OP     = 0;
    static const int CUSTOM = 1;

    QGroupBox * m_pOpGB;
    KComboBox * m_pOpCB;
    QLabel * m_pSeqL;
    KLineEdit * m_pOpLE, * m_pSeqLE;
    QLabel * m_pCustomRB, * m_pOpRB;
    QPushButton * m_newOperationButton;
    QCheckBox * m_pOpAS;
    QString m_Text;
    int m_nOpCount;
    int m_id;  ///< takes on the value OP or CUSTOM according to what the user selected
    UMLView *m_pView;
    UMLClassifier *m_classifier;

public slots:
    void slotAutoIncrementChecked(bool state);
    void slotNewOperation();
    void slotIndexChanged(int index);
    void slotTextChanged(const QString &text);
};

#endif
