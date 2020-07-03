#include <QGuiApplication>
#include "RecipeBookUIContext.h"

#include <QFont>

int main(int argc, char *argv[])
{
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);

#ifdef Q_OS_ANDROID
    // Default font size on android too large
    QFont font = app.font();
    font.setPixelSize(14);
    app.setFont(font);
#endif

	recipebook::UI::RecipeBookUIContext dataHandler;
	if(!dataHandler.setupQml())
	{
		return -1;
	}

	return app.exec();
}
