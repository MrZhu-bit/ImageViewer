#pragma once
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>

class Login : public QDialog
{
	Q_OBJECT
 public:
	explicit Login(QWidget *parent = nullptr);
	~Login() override;
 private:
	void readData();
	void iniUI();
	void iniSubWin();
 private slots:
	void on_resetBtn_clicked();
 	void on_loginBtn_clicked();
	void on_viewpassword_stateChanged(int state);
	void on_subFinishBtn_clicked();
 signals:

 private:
	QLabel* usernameLabel{};
 	QLabel* passwordLabel{};
	QLineEdit* usernameLineEdit{};
	QLineEdit* passwordLineEdit{};
	QPushButton* resetBtn{};
	QPushButton* loginBtn{};
 	QCheckBox* viewpassword{};
	QGridLayout* layout{};
	QDialog* subWin{};
	QLabel* subAdminLab{};
	QLabel* subUsernameLab{};
	QLabel* subPasswordLab{};
	QLabel* subRepasswordLab{};
	QLineEdit* subAdminLine{};
	QLineEdit* subUsernameLine{};
	QLineEdit* subPasswordLine{};
	QLineEdit* subRepasswordLine{};
	QPushButton* subFinishBtn{};
	QGridLayout* subLayout{};
	QString usernamefilepath{};
	QString passwordfilepath{};
	QString username{};
	QString password{};
};
