#include <QApplication>
#include "RecipeBookUIContext.h"

#include <QFont>

int main(int argc, char *argv[])
{
#ifndef Q_OS_ANDROID
	QApplication app(argc, argv);
#else
	QGuiApplication app(argc, argv);
#endif

	bool bAndroid = false;
#ifdef Q_OS_ANDROID
    // Default font size on android too large
    QFont font = app.font();
    font.setPixelSize(14);
    app.setFont(font);
	bAndroid = true;
#endif

	recipebook::UI::RecipeBookUIContext dataHandler(bAndroid);
	if(!dataHandler.setupQml())
	{
		return -1;
	}

	return app.exec();
}
