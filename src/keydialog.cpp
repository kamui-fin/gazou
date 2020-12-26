#include <QDebug>
#include <QKeyEvent>
#include <QLayout>
#include <QDialogButtonBox>
#include <QRegularExpression>
#include <iostream>
#include "keydialog.h"

KeyDialog::KeyDialog(QDialog *parent) : QDialog(parent)
{

    resize(251, 95);
    verticalLayout = new QVBoxLayout();
    label = new QLabel();
    verticalLayout->addWidget(label, 0, Qt::AlignHCenter);
    buttonBox = new QDialogButtonBox();
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    // connect(buttonBox->button(QDialogButtonBox::Ok), &QDialogButtonBox::clicked, &KeyDialog::save);
    label->setText("Press a set of keys to begin");
    verticalLayout->addWidget(buttonBox);
    setLayout(verticalLayout);
}

void KeyDialog::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    QKeySequence *sequence;
    if (key == Qt::Key_Control ||
        key == Qt::Key_Shift ||
        key == Qt::Key_Alt ||
        key == Qt::Key_Meta)
    {
        sequence = new QKeySequence(e->modifiers());
    }
    else if (key == Qt::Key_Super_L || key == Qt::Key_Super_R)
    {
        return;
    }
    else
    {
        sequence = new QKeySequence(e->key() | e->modifiers());
    }
    keyseq = sequence->toString();
    label->setText(keyseq);
}

QString KeyDialog::getKeySeq()
{
    return keyseq;
}
