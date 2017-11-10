#include "GameGUI.hpp"
#include "QEvent.h"
GameGUI::GameGUI(QWidget *parent) : QWidget(parent), control_blocked(false) {
	currentScore = new QLineEdit("0");
	currentScore->setReadOnly(true);
	currentScore->setFont(Settings::defaultFont);
	currentScore->setAlignment(Qt::AlignRight);
	maxScore = new QLineEdit("0");
	maxScore->setReadOnly(true);
	maxScore->setFont(Settings::defaultFont);
	maxScore->setAlignment(Qt::AlignRight);
	score = new QHBoxLayout();
	score->addWidget(currentScore);
	score->addWidget(maxScore);

	newGame = new QPushButton("Restart");
	newGame->setFont(Settings::defaultFont);
	connect(newGame, &QPushButton::clicked, this, &GameGUI::restart);
	exit = new QPushButton("Exit");
	exit->setFont(Settings::defaultFont);
	connect(exit, &QPushButton::clicked, this, &GameGUI::close);
	collectButton = new QPushButton("Collect game data");
	collectButton->setFont(Settings::defaultFont);
	collectButton->setCheckable(true);
	loadNetworkButton = new QPushButton("Load network file");
	loadNetworkButton->setFont(Settings::defaultFont);
	connect(loadNetworkButton, &QPushButton::clicked, this, &GameGUI::loadNetwork);
	buttons = new QHBoxLayout();
	buttons->addWidget(newGame);
	buttons->addWidget(exit);

	core = new GameCore<4>();
	collector = new GameCollector<4>("gameData.dat", core->getField());
	field = new FieldWidget<4>(core->getField());

	layout = new QVBoxLayout();
	layout->addLayout(score);
	layout->addWidget(field);
	layout->addLayout(buttons);
	layout->addWidget(collectButton);
	layout->addWidget(loadNetworkButton);

	setLayout(layout);

	resize(minimumSize());

	updateScore();
}
GameGUI::~GameGUI() {
	delete field;
	delete core;

	delete currentScore;
	delete maxScore;
	delete score;
	delete newGame;
	delete exit;
	delete buttons;
	delete layout;

	if (network) delete network;
}
void GameGUI::restart() {
	control_blocked = false;
	core->restart();
	//To implement
	field->update();
}

bool GameGUI::eventFilter(QObject *o, QEvent *ev) {
	if (!control_blocked && ev->type() == QEvent::KeyPress) {
		switch (static_cast<QKeyEvent*>(ev)->key()) {
			case Qt::Key::Key_Down:
			case Qt::Key::Key_S:
				core->down();
				if (collectButton->isChecked())
					collector->saveMove(Move::Down);
				break;
			case Qt::Key::Key_Left:
			case Qt::Key::Key_A:
				core->left();
				if (collectButton->isChecked())
					collector->saveMove(Move::Left);
				break;
			case Qt::Key::Key_Right:
			case Qt::Key::Key_D:
				core->right();
				if (collectButton->isChecked())
					collector->saveMove(Move::Right);
				break;
			case Qt::Key::Key_Up:
			case Qt::Key::Key_W:
				core->up();
				if (collectButton->isChecked())
					collector->saveMove(Move::Up);
				break;
			case Qt::Key::Key_Escape:
				close();
				break;
		}
		field->update();
		updateScore();
		ev->accept();
		return true;
	}
	return QObject::eventFilter(o, ev);
}

void GameGUI::updateScore() {
	currentScore->setText(QString::number(core->score()));
	maxScore->setText(QString::number(core->record()));
	if (core->isOver())
		currentScore->setStyleSheet("QLineEdit { background: rgb(255, 100, 100); }");
	else
		currentScore->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); }");
}

#include "..\..\MyNeuralNetwork\AbstractNetworkCore\Storage.hpp"
#include <QFileDialog>
void GameGUI::loadNetwork() {
	auto filename = QFileDialog::getOpenFileName(this, tr("Open Neural Network"), "",
												 tr("MyNeuralNetwork file (*.mnn)")).toStdString();
	network = mnn::load_from_file(filename);
	simulateGame();
}

#include "..\..\MyNeuralNetwork\AbstractNetworkCore\AbstractNetwork.hpp"
#include <chrono>
#include <thread>
using namespace std::chrono_literals;
auto const step = 500ms;
void GameGUI::simulateGame() {
	restart();
	control_blocked = true;

	while (!core->isOver()) {
		auto f = core->getField()->getNormalizedCellValues();
		network->calculate(f);
		auto o = network->getOutputs();
		bool turn = false;
		if (o[0] > o[1])
			if (o[0] > o[2])
				if (o[0] > o[3])
					turn = core->up();
				else
					turn = core->right();
			else
				if (o[2] > o[3])
					turn = core->left();
				else
					turn = core->right();
		else
			if (o[1] > o[2])
				if (o[1] > o[3])
					turn = core->down();
				else
					turn = core->right();
			else
				if (o[2] > o[3])
					turn = core->left();
				else
					turn = core->right();

		field->update();
		updateScore();
		repaint();

		if (!turn) break;
		std::this_thread::sleep_for(step);
	}

	control_blocked = false;
}