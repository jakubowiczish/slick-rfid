#include <gui/screen1_screen/Screen1View.hpp>
#include <iostream>
#include <sstream>
#include "term_io.h"
#include <bits/stdc++.h>

Screen1View::Screen1View() {

}

void Screen1View::setupScreen() {
	Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen() {
	Screen1ViewBase::tearDownScreen();
}

void Screen1View::setUidText(uint8_t uid_tab[]) {
	Unicode::snprintf(uidTextFieldBuffer, UIDTEXTFIELD_SIZE, "%d %d %d %d", uid_tab[0], uid_tab[1], uid_tab[2], uid_tab[3]);

	uidTextField.setVisible(true);
	uidTextField.invalidate();
	avatarChoiceTextField.setVisible(true);
	avatarChoiceTextField.invalidate();
	szczygi.setVisible(true);
	rafal.setVisible(true);
	zajma.setVisible(true);
	dybczak.setVisible(true);
	plotnik.setVisible(true);
}

void Screen1View::showChosenAvatar(uint8_t id) {
	if (id == 1) {
		zajmaHandler();
	} else if (id == 2) {
		szczygiHandler();
	} else if (id == 3) {
		dybczakHandler();
	} else if (id == 4) {
		rafalHandler();
	} else if (id == 5) {
		plotnikHandler();
	}
}

void Screen1View::zajmaHandler(uint8_t id) {
	zajma.setVisible(true);
	plotnik.setVisible(false);
	szczygi.setVisible(false);
	dybczak.setVisible(false);
	rafal.setVisible(false);
	clicked_id = id;
}

void Screen1View::szczygiHandler(uint8_t id) {
	zajma.setVisible(false);
	plotnik.setVisible(false);
	szczygi.setVisible(true);
	dybczak.setVisible(false);
	rafal.setVisible(false);
	clicked_id = id;
}

void Screen1View::dybczakHandler(uint8_t id) {
	zajma.setVisible(false);
	plotnik.setVisible(false);
	szczygi.setVisible(false);
	dybczak.setVisible(true);
	rafal.setVisible(false);
	clicked_id = id;
}

void Screen1View::rafalHandler(uint8_t id) {
	zajma.setVisible(false);
	plotnik.setVisible(false);
	szczygi.setVisible(false);
	dybczak.setVisible(false);
	rafal.setVisible(true);
	clicked_id = id;
}

void Screen1View::plotnikHandler(uint8_t id) {
	zajma.setVisible(false);
	plotnik.setVisible(true);
	szczygi.setVisible(false);
	dybczak.setVisible(false);
	rafal.setVisible(false);
	clicked_id = id;
}
