// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtWidgets>

#include "finddialog.h"

//! [0]
FindDialog::FindDialog(QWidget *parent) : QDialog(parent)
{
    findReplaceWindow = new QWidget;
    findReplaceWindow->resize(400,100);

    findLabel = new QLabel(tr("Find &what:"));
    findLineEdit = new QLineEdit;
    findLabel->setBuddy(findLineEdit);
    findButton = new QPushButton(tr("&Find"));
    findButton->setDefault(true);

    QHBoxLayout *findLayout = new QHBoxLayout;
    findLayout->addWidget(findLabel);
    findLayout->addWidget(findLineEdit);
    findLayout->addWidget(findButton);

    moreButton = new QPushButton(tr("&More"));
    moreButton->setCheckable(true);
    moreButton->setAutoDefault(false);
    findLayout->addWidget(moreButton);
    connect(moreButton, &QAbstractButton::toggled, findReplaceWindow, &QWidget::setVisible);

    replaceLabel = new QLabel(tr("Replace &what:"));
    replaceLineEdit = new QLineEdit;
    replaceLabel->setBuddy(replaceLineEdit);
    replaceButton = new QPushButton(tr("&Replace"));
    replaceButton->setDefault(true);
    QPushButton *replaceAllButton = new QPushButton(tr("&Replace All"));
    replaceAllButton->setDefault(true);

    QHBoxLayout *replaceLayout = new QHBoxLayout;
    replaceLayout->addWidget(replaceLabel);
    replaceLayout->addWidget(replaceLineEdit);
    replaceLayout->addWidget(replaceButton);
    replaceLayout->addWidget(replaceAllButton);

    caseCheckBox = new QCheckBox(tr("Match &case"));
    wholeWordsCheckBox = new QCheckBox(tr("&Whole words"));
    QHBoxLayout *caseLayout = new QHBoxLayout;
    caseLayout->addWidget(caseCheckBox);
    caseLayout->addWidget(wholeWordsCheckBox);

    fromStartCheckBox = new QCheckBox(tr("Search from &start"));
    fromStartCheckBox->setChecked(true);
    backwardCheckBox = new QCheckBox(tr("Search &backward"));
    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(fromStartCheckBox);
    searchLayout->addWidget(backwardCheckBox);


    QVBoxLayout *extensionLayout = new QVBoxLayout;
    extensionLayout->setContentsMargins(QMargins());
    extensionLayout->addLayout(replaceLayout);
    extensionLayout->addLayout(caseLayout);
    extensionLayout->addLayout(searchLayout);

    findReplaceWindow->setLayout(extensionLayout);


    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(findLayout);


    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(leftLayout, 0, 0);
//    mainLayout->addWidget(buttonBox, 0, 1);
    mainLayout->addWidget(findReplaceWindow, 1, 0, 1, 2);
    mainLayout->setRowStretch(2, 1);

    setLayout(mainLayout);
    setWindowTitle(tr("Find or Replace"));

findReplaceWindow->hide();
}
