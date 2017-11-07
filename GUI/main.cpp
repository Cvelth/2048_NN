#include <QtWidgets/QApplication>
#include "GameGUI.hpp"
int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	GameGUI w;
	a.installEventFilter(&w);
	w.show();
	return a.exec();
}