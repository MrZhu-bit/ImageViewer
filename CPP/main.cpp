#include "../HPP/mainwindow.h"
#include "../HPP/Login.h"
#include <QApplication>
#include <QWidget>
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Login L;

	if (L.exec() == QDialog::Accepted)
	{
		MainWindow W;
		W.show();
		return QApplication::exec();
	}
	else
	{
		return 0;
	}
}