// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

//! [0]
class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent = nullptr);

private:
    QLabel *findLabel;
    QLineEdit *findLineEdit;
    QPushButton *findButton;

    QLabel *replaceLabel;
    QLineEdit *replaceLineEdit;
    QPushButton *replaceButton;

    QCheckBox *caseCheckBox;

    QPushButton *moreButton;
    QCheckBox *wholeWordsCheckBox;
    QCheckBox *fromStartCheckBox;
    QCheckBox *backwardCheckBox;
    QDialogButtonBox *buttonBox;


    QWidget *findReplaceWindow;
};
//! [0]

#endif
