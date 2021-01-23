#include <QDialog>

class SelectorWidget : public QDialog
{
    Q_OBJECT
public:
    explicit SelectorWidget(QWidget *parent = nullptr);
    QPixmap selectedPixmap;
    static QRect savedRect;
    void grabUsingSavedRect();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap desktopPixmap;
    QRect selectedRect;
};
