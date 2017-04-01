#pragma once

#include <QtWidgets/QWidget>
#include "ui_GameGUI.h"

class GameGUI : public QWidget
{
	Q_OBJECT

public:
	GameGUI(QWidget *parent = Q_NULLPTR);

private:
	Ui::GameGUIClass ui;
};
