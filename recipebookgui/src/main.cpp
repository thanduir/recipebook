#include <QGuiApplication>
#include "RecipeBookUIContext.h"

int main(int argc, char *argv[])
{
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);

	recipebook::UI::RecipeBookUIContext dataHandler;
	if(!dataHandler.setupQml())
	{
		return -1;
	}

	return app.exec();
}
