#pragma once
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QBoxLayout>

#include "FieldWidget.hpp"
#include "GameCore.hpp"
#include "GameCollector.hpp"

namespace mnn {
	class AbstractNetwork;
}

class GameGUI : public QWidget {
	Q_OBJECT

public:
	GameGUI(QWidget *parent = Q_NULLPTR);
	~GameGUI();

private:
	QLineEdit* currentScore;
	QLineEdit* maxScore;
	QHBoxLayout* score;

	QPushButton* newGame;
	QPushButton* exit;
	QPushButton* collectButton;
	QPushButton* loadNetworkButton;
	QHBoxLayout* buttons;

	GameCore<4>* core;
	FieldWidget<4>* field;
	QVBoxLayout* layout;

	GameCollector<4>* collector;
	mnn::AbstractNetwork *network;

	bool control_blocked;

protected:
	virtual bool eventFilter(QObject *obj, QEvent *event) override;

protected slots:
	void restart();
	void updateScore();
	void loadNetwork();
	void simulateGame();
};