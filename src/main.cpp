#include "UI\recipebook.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RecipeBook w;
	w.show();
	return a.exec();
}
