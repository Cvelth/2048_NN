#include "GameGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GameGUI w;
	w.show();
	return a.exec();
}
