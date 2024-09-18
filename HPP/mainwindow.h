#pragma once
#include "../HPP/decode.h"
#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QDir>
#include <vector>
#include <QLabel>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
 Q_OBJECT

 public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();
 private slots:
	void on_btnOpen_clicked();
	void on_btnClose_clicked();
	void on_btnEncrypt_clicked();
	void on_btnDecrypt_clicked();
	void on_btnToLeft_clicked();
	void on_btnToRight_clicked();
 protected:
	void resizeEvent(QResizeEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
 private:
	void loadImage(const QString& ImageName);
	Mat QImageToMat(const QImage& QImage);
	QImage MatToQImage(const Mat& Matrix);
	bool MessageBox(const char* messageTitle,const char* message,const char* ImgTitle,const Mat EnDeImage);
	bool MessageBox(const char* messageTitle,const char* message);
 private:
	Ui::MainWindow* ui;
	QGraphicsPixmapItem* pixmapItem;
	QPixmap PixmapImage;
	QImage QImg;
	Mat MatImage;
	QString fullFileName;
	QStringList fileList;
	int fileIndex;
	Matcrypt encrypter;
	Matcrypt decrypter;
	bool IsDecrypted;
};



