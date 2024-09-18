#pragma once
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <QPushButton>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsScene>
QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
 public:
	QWidget *centralwidget;
	QStatusBar *statusbar;
	QPushButton *btnOpen;
	QPushButton *btnClose;
	QPushButton *btnEncrypt;
	QPushButton *btnDecrypt;
	QPushButton *btnToLeft;
	QPushButton *btnToRight;
	QGraphicsView *graph;
	QSpacerItem *topSpacer;
	QHBoxLayout *topLayout;
	QHBoxLayout *downLayout;
	QVBoxLayout *verticalLayout;
	QGraphicsScene *scene;
	void setupUi(QMainWindow *MainWindow)
	{
		if (MainWindow->objectName().isEmpty())
			MainWindow->setObjectName("MainWindow");
		MainWindow->resize(800, 600);
		MainWindow->setWindowIcon(QIcon(":/Resource/Icons/Ubisoft.ico"));

		centralwidget = new QWidget(MainWindow);
		centralwidget->setObjectName("centralwidget");
		MainWindow->setCentralWidget(centralwidget);

		topLayout = new QHBoxLayout();
		btnOpen = new QPushButton("打开", centralwidget);
		btnClose = new QPushButton("关闭", centralwidget);
		btnEncrypt = new QPushButton("加密", centralwidget);
		btnDecrypt = new QPushButton("解密", centralwidget);
		topSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding);

		btnOpen->setObjectName("btnOpen");
		btnClose->setObjectName("btnClose");
		btnEncrypt->setObjectName("btnEncrypt");
		btnDecrypt->setObjectName("btnDecrypt");

		topLayout->setObjectName("topLayout");
		topLayout->addWidget(btnOpen);
		topLayout->addWidget(btnClose);
		topLayout->addWidget(btnEncrypt);
		topLayout->addWidget(btnDecrypt);
		topLayout->addItem(topSpacer);

		downLayout = new QHBoxLayout();

		graph = new QGraphicsView(centralwidget);
		scene = new QGraphicsScene(centralwidget);
		graph->setStyleSheet("background-color:rgb(222,120,151)");
		graph->setScene(scene);

		btnToLeft = new QPushButton(centralwidget);
		btnToRight = new QPushButton(centralwidget);
		btnToLeft->setObjectName("btnToLeft");
		btnToRight->setObjectName("btnToRight");
		btnToLeft->setFixedSize(33,60);
		btnToRight->setFixedSize(33,60);
		btnToLeft->setIcon(QIcon(":/Resource/Icons/ToLeft.png"));
		btnToRight->setIcon(QIcon(":/Resource/Icons/ToRight.png"));

		downLayout->addWidget(btnToLeft);
		downLayout->addWidget(graph);
		downLayout->addWidget(btnToRight);

		verticalLayout = new QVBoxLayout(centralwidget);
		verticalLayout->addLayout(topLayout);
		verticalLayout->addLayout(downLayout);

		statusbar = new QStatusBar(MainWindow);
		statusbar->setObjectName("statusbar");
		statusbar->setStyleSheet("background-color:rgb(60,63,65)");
		statusbar->showMessage("未打开图片");
		MainWindow->setStatusBar(statusbar);

		retranslateUi(MainWindow);
		QMetaObject::connectSlotsByName(MainWindow);
	}

	static void retranslateUi(QMainWindow *MainWindow)
	{
		MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
	}

};

namespace Ui
{
	class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE
