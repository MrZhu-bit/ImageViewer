#include "../HPP/mainwindow.h"
#include "../HPP/ui_mainwindow.h"
#include "../HPP/Test.h"
#include <QFileDialog>
#include <QMessageBox>
//#include <iostream>
#include <QTimer>
#include <QWheelEvent>
using namespace std;
using namespace cv;
MainWindow::MainWindow(QWidget* parent):encrypter(),decrypter(),fullFileName(),MatImage(),
QMainWindow(parent),ui(new Ui::MainWindow),PixmapImage(),QImg(),fileIndex(),fileList()
{
	pixmapItem = new QGraphicsPixmapItem();
	ui->setupUi(this);
	ui->scene->addItem(pixmapItem);
}

MainWindow::~MainWindow()
{
	delete ui;
	delete pixmapItem;
}

void MainWindow::on_btnOpen_clicked()
{
	fullFileName = QFileDialog::getOpenFileName(this,"打开图片",
		"D:/Clion/Project/OpenCV_Learning/Encrypt/ImageViewer/Resource/Test",
		"Images (*.png *.jpg *.jpeg *.webp *.bmp )");
	if (fullFileName.isEmpty()){return;}

	QFileInfo fileInfo(fullFileName);
	QString directoryPath = fileInfo.absolutePath();

	QDir fileDir(directoryPath);

	QStringList filters;
	filters << "*.png" << "*.jpg" << "*jpeg";
	fileList = fileDir.entryList(filters,QDir::Files);

	fileIndex = fileList.indexOf(fileInfo.fileName());

	for (QString& fileName : fileList)
	{
		fileName = fileDir.absoluteFilePath(fileName);
	}

	loadImage(fullFileName);
}

void MainWindow::on_btnClose_clicked()
{
	PixmapImage = QPixmap();
	QImg = QImage();
	MatImage = Mat();
	pixmapItem->setPixmap(PixmapImage);
	ui->statusbar->showMessage("未打开图片");
}

void MainWindow::on_btnEncrypt_clicked()
{
	if (MatImage.empty()){return;}
	Mat enImage = encrypter.encrypt(MatImage);
	if (enImage.empty()){return;}
	if (MessageBox("EncryptedImage","你是否想保存加密图片","EncryptedImage",enImage))
	{
		QFileInfo TempFileInfo(fullFileName);
		QString TempBaseName = TempFileInfo.completeBaseName();
		QString TempPath = TempFileInfo.absolutePath();
		QString NewFileName = QString("%1/%2_Encrypted.%3")
			.arg(TempPath)
			.arg(TempBaseName)
			.arg("png");
		imwrite(NewFileName.toLocal8Bit().toStdString(),enImage);
	}
}

void MainWindow::on_btnDecrypt_clicked()
{
	if (MatImage.empty()){return;}
	if (!IsDecrypted){MessageBox("TryToDecrypt","未加密图片无法被解密");return;}
	if (MessageBox("DecryptedImage","你是否想保存解密图片","DecryptedImage",MatImage))
	{
		QFileInfo TempFileInfo(fullFileName);
		QString TempBaseName = TempFileInfo.completeBaseName();
		QString TempPath = TempFileInfo.absolutePath();
		QString NewFileName = QString("%1/%2_Decrypted.%3")
			.arg(TempPath)
			.arg(TempBaseName)
			.arg("png");
		imwrite(NewFileName.toLocal8Bit().toStdString(),MatImage);
	}
}

void MainWindow::on_btnToLeft_clicked()
{
	if (fileList.isEmpty()){return;}
	fileIndex--;
	while (!(fileIndex>=0 && fileIndex<fileList.size()))
	{
		fileIndex += fileList.size();
	}
	fullFileName = fileList[fileIndex];
	loadImage(fullFileName);
}
void MainWindow::on_btnToRight_clicked()
{
	if (fileList.isEmpty()){return;}
	fileIndex++;
	while (!(fileIndex>=0 && fileIndex<fileList.size()))
	{
		fileIndex -= fileList.size();
	}
	fullFileName = fileList[fileIndex];
	loadImage(fullFileName);
}

void MainWindow::loadImage(const QString& ImageName)
{
	QImg.load(ImageName);
	MatImage = imread(ImageName.toLocal8Bit().toStdString(),IMREAD_UNCHANGED);
	Mat TempDeIMG = decrypter.decrypt(MatImage);
	if (TempDeIMG.empty())
	{
		IsDecrypted=false;
		ui->statusbar->showMessage("未加密图片");
	}
	else
	{
		MatImage=TempDeIMG;
		QImg=MatToQImage(MatImage);
		IsDecrypted=true;
		ui->statusbar->showMessage("已加密图片");
	}
	PixmapImage.convertFromImage(QImg);
	pixmapItem->setPixmap(PixmapImage);
	ui->graph->fitInView(pixmapItem,Qt::KeepAspectRatio);
	ui->scene->setSceneRect(pixmapItem->boundingRect());
}

Mat MainWindow::QImageToMat(const QImage& Image)
{
	Mat Matrix;
	switch (Image.format())
	{
	case QImage::Format_Grayscale8:
		Matrix = Mat(Image.height(),Image.width(),CV_8UC1,
			(void*)Image.constBits(),Image.bytesPerLine());
		break;
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		Matrix = Mat(Image.height(),Image.width(),CV_8UC4,
			(void*)Image.constBits(),Image.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		Matrix = Mat(Image.height(),Image.width(),CV_8UC3,
			(void*)Image.constBits(),Image.bytesPerLine());
		cvtColor(Matrix,Matrix,COLOR_RGB2BGR);
		break;
	case QImage::Format_RGBA64:
		Matrix = Mat(Image.height(),Image.width(),CV_16UC4,
			(void*)Image.constBits(),Image.bytesPerLine());
		cvtColor(Matrix,Matrix,COLOR_RGBA2BGRA);
		break;
	default:
		return Mat();
	}
	return Matrix.clone();
}

QImage MainWindow::MatToQImage(const Mat& Matrix)
{
	QImage Image;
	switch(Matrix.type())
	{
	case CV_8UC1:
		Image = QImage((const unsigned char*)Matrix.data, Matrix.cols,
			Matrix.rows, Matrix.step, QImage::Format_Grayscale8);
		break;
	case CV_8UC3:
		Image = QImage((const unsigned char*)Matrix.data, Matrix.cols,
			Matrix.rows, Matrix.step, QImage::Format_RGB888);
		Image = Image.rgbSwapped();
		break;
	case CV_8UC4:
		Image = QImage((const unsigned char*)Matrix.data, Matrix.cols,
			Matrix.rows, Matrix.step, QImage::Format_RGBA8888);
		Image = Image.rgbSwapped();
		break;
	case CV_16UC4:
		Image = QImage((const unsigned char*)Matrix.data, Matrix.cols,
			Matrix.rows, Matrix.step, QImage::Format_RGBA64);
		Image = Image.rgbSwapped();
		break;
	default:
		return QImage();
	}

	return QImage(Image);
}

bool MainWindow::MessageBox(const char* messageTitle,const char* message,const char* ImgTitle,const Mat EnDeImage)
{
	QWidget *window = new QWidget();

	window->setWindowTitle(ImgTitle);

	QLabel *imageLabel = new QLabel(window);
	QPixmap pixmapToShow = QPixmap::fromImage(MatToQImage(EnDeImage));

	imageLabel->setPixmap(pixmapToShow);
	imageLabel->setAlignment(Qt::AlignCenter);

	QVBoxLayout *layout = new QVBoxLayout(window);
	layout->addWidget(imageLabel);
	window->setLayout(layout);

	window->show();

	int w = EnDeImage.cols;
	int h = EnDeImage.rows;
	float scale1 = w / 960.0;
	float scale2 = h / 540.0;
	float scale = std::max(scale1,scale2);

	window->resize(int(w/scale),int(h/scale));
	window->move(0,0);

	bool result = false;
	QMessageBox msgBox;

	msgBox.setWindowTitle(messageTitle);
	msgBox.setText(message);

	QPushButton* accept = msgBox.addButton("是",QMessageBox::AcceptRole);
	QPushButton* reject = msgBox.addButton("否",QMessageBox::RejectRole);
	msgBox.exec();

	if (msgBox.clickedButton() == accept){result=true;}
	else if (msgBox.clickedButton() == reject){result=false;}

	window->close();
	delete window;
	return result;
}

bool MainWindow::MessageBox(const char* messageTitle, const char* message)
{
	bool result = false;
	QMessageBox msgBox;
	msgBox.setWindowTitle(messageTitle);
	msgBox.setText(message);
	QPushButton* accept = msgBox.addButton("是",QMessageBox::AcceptRole);
	QPushButton* reject = msgBox.addButton("否",QMessageBox::RejectRole);
	msgBox.exec();
	if (msgBox.clickedButton() == accept){result=true;}
	else if (msgBox.clickedButton() == reject){result=false;}
	return result;
}
void MainWindow::resizeEvent(QResizeEvent* event)
{
	QMainWindow::resizeEvent(event);

	if (!PixmapImage.isNull())
	{
		ui->graph->fitInView(pixmapItem,Qt::KeepAspectRatio);
		ui->scene->setSceneRect(pixmapItem->boundingRect());
	}
}
void MainWindow::wheelEvent(QWheelEvent* event)
{
	if (event->angleDelta().y() > 0)
	{
		on_btnToLeft_clicked();
	}
	else if (event->angleDelta().y() < 0)
	{
		on_btnToRight_clicked();
	}
}


