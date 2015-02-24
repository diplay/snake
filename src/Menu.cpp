#include "Menu.h"

extern Resources resources;

Menu::Menu()
{
	style.font = resources.getResFont("main")->getFont();
	style.color = Color::White;
}

void Menu::addItem(std::string text, EventCallback onClick)
{
	spTextField item = new TextField();
	int position = 0;
	for(auto it : items)
		position = it->getY() * 2;
	position += style.font->getSize();
	addChild(item);
	item->setPosition(0, position);
	item->setText(text);
	item->setStyle(style);
	item->addEventListener(TouchEvent::CLICK, onClick);
	items.push_back(item);
}
