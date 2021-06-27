#include "keydialog.h"
#include <QDialogButtonBox>
#include <QKeyEvent>
#include <QLayout>

KeyDialog::KeyDialog(QDialog *parent) : QDialog(parent) {

    resize(251, 95);
    verticalLayout = new QVBoxLayout();
    label = new QLabel();
    verticalLayout->addWidget(label, 0, Qt::AlignHCenter);
    buttonBox = new QDialogButtonBox();
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel |
                                  QDialogButtonBox::Ok);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    label->setText("Press a set of keys to begin");
    verticalLayout->addWidget(buttonBox);
    setLayout(verticalLayout);
}

void KeyDialog::keyPressEvent(QKeyEvent *e) {
    int key = e->key();
    QKeySequence *sequence;
    switch (key) {
    case Qt::Key_Control:
    case Qt::Key_Shift:
    case Qt::Key_Alt:
    case Qt::Key_Meta:
        sequence = new QKeySequence(e->modifiers());
        break;
    case Qt::Key_Super_L:
    case Qt::Key_Super_R:
        return;
    default:
        sequence = new QKeySequence(e->key() | e->modifiers());
    }
    keyseq = sequence->toString();
    label->setText(keyseq);
}

QString KeyDialog::getKeySeq() { return keyseq; }
