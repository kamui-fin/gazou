#include <QDialog>
#include <QDialogButtonBox>
#include <QLayout>
#include <QLabel>

class KeyDialog : public QDialog
{
    Q_OBJECT

public:
    KeyDialog(QDialog *parent = 0);
    QString getKeySeq();

protected:
    void keyPressEvent(QKeyEvent *event);
    void save();

private:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QDialogButtonBox *buttonBox;

    QString keyseq;
};
