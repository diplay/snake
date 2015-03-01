#include "Scoreboard.h"
#include <cstdio>

extern Resources resources;

Scoreboard::Scoreboard(Vector2 pos, GAME_MODE mode)
{
	this->mode = mode;
	setPosition(pos);
	score = 0;
	energy = 0;
	text = new TextField();
	addChild(text);
	text->setPosition(0, 0);
	TextStyle style;
	style.font = resources.getResFont("invaders")->getFont();
	style.color = Color::White;
	style.hAlign = TextStyle::HALIGN_LEFT;
	text->setStyle(style);
	updateText();
}

void Scoreboard::updateText()
{
	char str[255];
	if(mode == MODE_CLASSIC)
		sprintf(str, "SCORE: %d", score);
	else
		sprintf(str, "SCORE: %d\nENERGY: %d", score, energy);
	text->setText(str);
}

void Scoreboard::setScore(int score, int energy)
{
	this->score = score;
	this->energy = energy;
	updateText();
}
