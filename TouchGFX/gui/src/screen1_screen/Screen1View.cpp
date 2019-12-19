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
	Unicode::snprintf(uidTextFieldBuffer, UIDTEXTFIELD_SIZE, "%d %d %d %d",
			uid_tab[0], uid_tab[1], uid_tab[2], uid_tab[3]);

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

void Screen1View::chooseAvatar(uint8_t uid_tab[]) {

}
