#include "GameGUI.h"
#include "QEvent.h"

GameGUI::GameGUI(QWidget *parent)
	: QWidget(parent)
{
	currentScore = new QLineEdit("0");
	currentScore->setReadOnly(true);
	currentScore->setFont(SharedObjects::defaultFont);
	currentScore->setAlignment(Qt::AlignRight);
	maxScore = new QLineEdit("0");
	maxScore->setReadOnly(true);
	maxScore->setFont(SharedObjects::defaultFont);
	maxScore->setAlignment(Qt::AlignRight);
	score = new QHBoxLayout();
	score->addWidget(currentScore);
	score->addWidget(maxScore);

	newGame = new QPushButton("Restart");
	newGame->setFont(SharedObjects::defaultFont);
	connect(newGame, &QPushButton::clicked, this, &GameGUI::restart);
	exit = new QPushButton("Exit");
	exit->setFont(SharedObjects::defaultFont);
	connect(exit, &QPushButton::clicked, this, &GameGUI::close);
	collectButton = new QPushButton("Collect game data");
	collectButton->setFont(SharedObjects::defaultFont);
	collectButton->setCheckable(true);
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
