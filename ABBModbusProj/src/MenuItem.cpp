/*
 * MenuItem.cpp
 *
 *  Created on: 1.2.2016
 *      Author: krl
 */

#include "MenuItem.h"

MenuItem::MenuItem(PropertyEdit *property): pe(property), updated(false) {

}

MenuItem::~MenuItem() {

}

bool MenuItem::event(menuEvent e) {
	bool handled = true;
	clearChange();
	switch(e) {
	case ok:
		if(pe->getFocus()) {
			if (pe->accept())
				updated = true;
			pe->setFocus(false);

		}
		else {
			pe->setFocus(true);
		}
		break;
	case back:
		if(pe->getFocus()) {
			pe->cancel();
			pe->setFocus(false);
		}
		else {
			handled = false;
		}
		break;
	case show:
		break;
	case up:
		if(pe->getFocus()) pe->increment();
		else handled = false;
		break;
	case down:
		if(pe->getFocus()) pe->decrement();
		else handled = false;
		break;
	}
	if(handled) pe->display();

	return handled;
}

bool MenuItem::hasChanged() {
	return updated;
}

void MenuItem::clearChange() {
	updated = false;
}
