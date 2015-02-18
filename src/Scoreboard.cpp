#include "Scoreboard.h"
#include <cstdio>

extern Resources resources;

Scoreboard::Scoreboard(Vector2 pos)
{
	setPosition(pos);
	score = 0;
	energy = 0;
	text = new TextField();
	addChild(text);
	text->setPosition(0, 0);
	TextStyle style;
	style.font = resources.getResFont("main")->getFont();
	style.color = Color::White;
	text->setStyle(style);
	update();
}

void Scoreboard::update()
{
	char str[255];
	sprintf(str, "SCORE: %d\nENERGY: %d", score, energy);
	text->setText(str);
}

void Scoreboard::setScore(int score, int energy)
{
	this->score = score;
	this->energy = energy;
	update();
}
