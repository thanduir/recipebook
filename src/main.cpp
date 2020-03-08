#include "UI\recipebookwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RecipeBookWindow w;
	w.show();
	return a.exec();
}
