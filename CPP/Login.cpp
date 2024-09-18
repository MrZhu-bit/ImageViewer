#include "../HPP/Login.h"
#include <string>
#include <QFile>
#include <QMessageBox>
#include <QCoreApplication>
using namespace std;
Login::Login(QWidget* parent)
{
	iniUI();
	iniSubWin();
	usernamefilepath = QCoreApplication::applicationDirPath() + "/userdata/username.bin";
	passwordfilepath = QCoreApplication::applicationDirPath() + "/userdata/password.bin";
}
Login::~Login()
{
	delete subWin;
}
void Login::readData()
{
	QFile usernamefile(usernamefilepath);
	QFile passwordfile(passwordfilepath);
	if (usernamefile.open(QIODevice::ReadOnly) && passwordfile.open(QIODevice::ReadOnly))
	{
		QByteArray usernameArray = usernamefile.readAll();
		QByteArray passwordArray = passwordfile.readAll();
		username = QString::fromUtf8(usernameArray);
		password = QString::fromUtf8(passwordArray);
	}
	else
	{
		QMessageBox::warning(this,"登录失败","请点击重置账号以注册账号");
	}
	if (username.isEmpty() || password.isEmpty())
	{
		QMessageBox::warning(this,"登录失败","请点击重置账号以注册账号");
	}
}
void Login::iniUI()
{
	usernameLabel = new QLabel("用户名",this);
	passwordLabel = new QLabel(" 密码 ",this);

	usernameLineEdit = new QLineEdit(this);
	passwordLineEdit = new QLineEdit(this);
	usernameLineEdit->setEchoMode(QLineEdit::Normal);
	passwordLineEdit->setEchoMode(QLineEdit::Password);

	resetBtn = new QPushButton("重置账号",this);
	loginBtn = new QPushButton("登录",this);

	viewpassword = new QCheckBox("显示密码",this);

	layout = new QGridLayout(this);

	layout->addWidget(usernameLabel,0,0);
	layout->addWidget(usernameLineEdit,0,1);
	layout->addWidget(resetBtn,0,2);

	layout->addWidget(passwordLabel,1,0);
	layout->addWidget(passwordLineEdit,1,1);
	layout->addWidget(viewpassword,1,2);

	layout->addWidget(loginBtn,2,0,1,3);

	setLayout(layout);

	resetBtn->setObjectName("resetBtn");
	loginBtn->setObjectName("loginBtn");
	viewpassword->setObjectName("viewpassword");
	QMetaObject::connectSlotsByName(this);

	setWindowTitle("登录");
	resize(300,80);
}
void Login::iniSubWin()
{
	subWin = new QDialog();
	subAdminLab = new QLabel("管理员密码",subWin);
	subUsernameLab = new QLabel("用户名",subWin);
	subPasswordLab = new QLabel("密码",subWin);
	subRepasswordLab = new QLabel("确认密码",subWin);
	subAdminLine = new QLineEdit(subWin);
	subUsernameLine = new QLineEdit(subWin);
	subPasswordLine = new QLineEdit(subWin);
	subRepasswordLine = new QLineEdit(subWin);
	subFinishBtn = new QPushButton("完成",subWin);
	subLayout = new QGridLayout(subWin);

	subLayout->addWidget(subAdminLab,0,0,1,1);
	subLayout->addWidget(subAdminLine,0,1,1,1);
	subLayout->addWidget(subUsernameLab,1,0,1,1);
	subLayout->addWidget(subUsernameLine,1,1,1,1);
	subLayout->addWidget(subPasswordLab,2,0,1,1);
	subLayout->addWidget(subPasswordLine,2,1,1,1);
	subLayout->addWidget(subRepasswordLab,3,0,1,1);
	subLayout->addWidget(subRepasswordLine,3,1,1,1);
	subLayout->addWidget(subFinishBtn,4,0,1,2);

	subWin->setLayout(subLayout);

	connect(subFinishBtn,&QPushButton::clicked,
		this,&Login::on_subFinishBtn_clicked);

	subWin->setWindowTitle("重置账号");
}
void Login::on_resetBtn_clicked()
{
	subWin->exec();
}
void Login::on_subFinishBtn_clicked()
{
	if (subAdminLine->text() == "Namedifficult" && !subUsernameLine->text().isEmpty() && !subPasswordLine->text().isEmpty() && subPasswordLine->text() == subRepasswordLine->text())
	{
		QFile usernamefile(usernamefilepath);
		QFile passwordfile(passwordfilepath);
		if (usernamefile.open(QIODevice::WriteOnly) && passwordfile.open(QIODevice::WriteOnly))
		{
			QByteArray usernameArray = subUsernameLine->text().toUtf8();
			QByteArray passwordArray = subPasswordLine->text().toUtf8();
			usernamefile.write(usernameArray);
			passwordfile.write(passwordArray);
			usernamefile.close();
			passwordfile.close();
		}
		else
		{
			QMessageBox::warning(subWin,"读取错误","用户数据文件写入错误,请重试!");
		}
		QMessageBox::information(subWin,"重置账户","账户重置成功!");
		subAdminLine->clear();
		subUsernameLine->clear();
		subPasswordLine->clear();
		subRepasswordLine->clear();
		subWin->hide();
	}
	else if (subAdminLine->text() != "Namedifficult" && !subUsernameLine->text().isEmpty() && !subPasswordLine->text().isEmpty() && subPasswordLine->text() == subRepasswordLine->text())
	{
		QMessageBox::warning(subWin,"输入错误","管理员密码错误!");
	}
	else if (subAdminLine->text() == "Namedifficult" && (subUsernameLine->text().isEmpty() || subPasswordLine->text().isEmpty()) && subPasswordLine->text() == subRepasswordLine->text())
	{
		QMessageBox::warning(subWin,"输入错误","账号和密码不能为空!");
	}
	else if (subAdminLine->text() == "Namedifficult" && !subUsernameLine->text().isEmpty() && !subPasswordLine->text().isEmpty() && subPasswordLine->text() != subRepasswordLine->text())
	{
		QMessageBox::warning(subWin,"输入错误","两次输入的密码不一致!");
	}
}
void Login::on_loginBtn_clicked()
{
	readData();
	if (username != usernameLineEdit->text() && password != passwordLineEdit->text())
	{
		QMessageBox::information(this,"登录失败","请输入正确的账号和密码");
	}
	else if (username != usernameLineEdit->text() && password == passwordLineEdit->text())
	{
		QMessageBox::information(this,"登录失败","请输入正确的账号");
	}
	else if (username == usernameLineEdit->text() && password != passwordLineEdit->text())
	{
		QMessageBox::information(this,"登录失败","请输入正确的密码");
	}
	else if (username == usernameLineEdit->text() && password == passwordLineEdit->text())
	{
		accept();
	}
}
void Login::on_viewpassword_stateChanged(int state)
{
	if (state == Qt::Checked)
	{
		passwordLineEdit->setEchoMode(QLineEdit::Normal);
	}
	else if (state == Qt::Unchecked)
	{
		passwordLineEdit->setEchoMode(QLineEdit::Password);
	}
}




