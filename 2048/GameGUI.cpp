#include "GameGUI.h"
#include "QEvent.h"

GameGUI::GameGUI(QWidget *parent)
	: QWidget(parent)
{
	currentScore = new QLineEdit("0");
	currentScore->setReadOnly(true);
	maxScore = new QLineEdit("0");
	maxScore->setReadOnly(true);
	score = new QHBoxLayout();
	score->addWidget(currentScore);
	score->addWidget(maxScore);

	newGame = new QPushButton("Restart");
	connect(newGame, &QPushButton::clicked, this, &GameGUI::restart);
	exit = new QPushButton("Exit");
	connect(exit, &QPushButton::clicked, this, &GameGUI::close);
	buttons = new QHBoxLayout();
	buttons->addWidget(newGame);
	buttons->addWidget(exit);

	core = new GameCore<4>();

	field = new FieldWidget<4>(core->getField());
	layout = new QVBoxLayout();
	layout->addLayout(score);
	layout->addWidget(field);
	layout->addLayout(buttons);

	setLayout(layout);

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
}

void GameGUI::restart() {
	core->restart();
	//To implement
	field->update();
}

bool GameGUI::eventFilter(QObject *o, QEvent *ev) {
	if (ev->type() == QEvent::KeyPress) {
		switch (static_cast<QKeyEvent*>(ev)->key()) {
			case Qt::Key::Key_Down:
			case Qt::Key::Key_S:
				core->down();
				break;
			case Qt::Key::Key_Left:
			case Qt::Key::Key_A:
				core->left();
				break;
			case Qt::Key::Key_Right:
			case Qt::Key::Key_D:
				core->right();
				break;
			case Qt::Key::Key_Up:
			case Qt::Key::Key_W:
				core->up();
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
