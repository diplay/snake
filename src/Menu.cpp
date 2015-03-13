#include "Menu.h"

extern Resources resources;

Menu::Menu()
{
	style.font = resources.getResFont("invaders")->getFont();
	style.color = Color::White;
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
}

void Menu::addItem(std::string text)
{
	spTextField item = new TextField();
	int position = 0;
	if(!items.empty())
	{
		position = items.back()->getY();
		position += style.font->getSize() * 2;
	}
	addChild(item);
	item->setPosition(0, position);
	item->setText(text);
	item->setStyle(style);
	items.push_back(item);
}

void Menu::addItem(std::string text, EventCallback onClick)
{
	addItem(text);
	items.back()->addEventListener(TouchEvent::CLICK, onClick);
}
