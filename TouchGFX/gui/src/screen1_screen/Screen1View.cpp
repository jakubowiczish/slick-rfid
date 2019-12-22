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

void Screen1View::showAuthScreen() {
    application().gotoAuthScreenScreenCoverTransitionSouth();
}

void Screen1View::setUidText(uint8_t uid_tab[]) {
	Unicode::snprintf(uidTextFieldBuffer, UIDTEXTFIELD_SIZE, "%d %d %d %d", uid_tab[0], uid_tab[1], uid_tab[2], uid_tab[3]);

	makeTextFieldsVisible();
	makeAllAvatarsVisible();
}

void Screen1View::makeTextFieldsVisible() {
	uidTextField.setVisible(true);
	uidTextField.invalidate();
	avatarChoiceTextField.setVisible(true);
	avatarChoiceTextField.invalidate();
}

void Screen1View::makeAllAvatarsVisible() {
	szczygi.setVisible(true);
	rafal.setVisible(true);
	zajma.setVisible(true);
	dybczak.setVisible(true);
	plotnik.setVisible(true);

	invalidateAvatarButtons();
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

void Screen1View::zajmaHandler() {
	xprintf("Zajma is clicked \r\n");
	handleZajmaVisibility();
	clicked_id = 1;
}

void Screen1View::szczygiHandler() {
	xprintf("Szczygi is clicked \r\n");
	handleSzczygiVisibility();
	clicked_id = 2;
}

void Screen1View::dybczakHandler() {
	xprintf("Dybczak is clicked \r\n");
	handleDybczakVisibility();
	clicked_id = 3;
}

void Screen1View::rafalHandler() {
	xprintf("Rafal is clicked \r\n");
	handleRafalVisibility();
	clicked_id = 4;
}

void Screen1View::plotnikHandler() {
	xprintf("Plotnik is clicked \r\n");
	handlePlotnikVisibility();
	clicked_id = 5;
}

void Screen1View::handleZajmaVisibility() {
	zajma.setVisible(true);
	plotnik.setVisible(false);
	szczygi.setVisible(false);
	dybczak.setVisible(false);
	rafal.setVisible(false);

	invalidateAvatarButtons();
}

void Screen1View::handleSzczygiVisibility() {
	zajma.setVisible(false);
	plotnik.setVisible(false);
	szczygi.setVisible(true);
	dybczak.setVisible(false);
	rafal.setVisible(false);

	invalidateAvatarButtons();
}

void Screen1View::handleDybczakVisibility() {
	zajma.setVisible(false);
	plotnik.setVisible(false);
	szczygi.setVisible(false);
	dybczak.setVisible(true);
	rafal.setVisible(false);

	invalidateAvatarButtons();
}

void Screen1View::handleRafalVisibility() {
	zajma.setVisible(false);
	plotnik.setVisible(false);
	szczygi.setVisible(false);
	dybczak.setVisible(false);
	rafal.setVisible(true);

	invalidateAvatarButtons();
}

void Screen1View::handlePlotnikVisibility() {
	zajma.setVisible(false);
	plotnik.setVisible(true);
	szczygi.setVisible(false);
	dybczak.setVisible(false);
	rafal.setVisible(false);

	invalidateAvatarButtons();
}

void Screen1View::invalidateAvatarButtons() {
	szczygi.invalidate();
	rafal.invalidate();
	zajma.invalidate();
	dybczak.invalidate();
	plotnik.invalidate();
}
