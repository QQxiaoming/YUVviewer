#ifndef BOOTSELECTBOX_H
#define BOOTSELECTBOX_H

#include <QDialog>

namespace Ui {
class ImgExportWindow;
}

class ImgExport : public QDialog
{
    Q_OBJECT

public:
    explicit ImgExport(QWidget *parent = nullptr);
    ~ImgExport();
    void export_png(QImage *Img, const QString &name);
    void export_bayer(QImage *Img, const QString &sequence,int bit, const QString &fileName);

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    void buttonBoxAccepted(void);
    void buttonBoxRejected(void);

private:
    Ui::ImgExportWindow *ui;
};

#endif // BOOTSELECTBOX_H
