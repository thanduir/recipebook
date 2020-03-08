#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_recipebookwindow.h"

class RecipeBookWindow : public QMainWindow
{
	Q_OBJECT

public:
	RecipeBookWindow(QWidget *parent = Q_NULLPTR);

private:
	Ui::RecipeBookWindowClass ui;
};
