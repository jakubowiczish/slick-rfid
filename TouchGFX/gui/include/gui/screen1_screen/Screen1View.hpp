#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

class Screen1View : public Screen1ViewBase {
	public:
		Screen1View();
		virtual ~Screen1View() {
		}
		virtual void setupScreen();
		virtual void tearDownScreen();

		void showAuthScreen();

		void setUidText(uint8_t uid_tab[]);
		void showChosenAvatar(uint8_t id);

		void plotnikHandler();
		void zajmaHandler();
		void rafalHandler();
		void dybczakHandler();
		void szczygiHandler();

		void invalidateAvatarButtons();

		void handlePlotnikVisibility();
		void handleZajmaVisibility();
		void handleRafalVisibility();
		void handleDybczakVisibility();
		void handleSzczygiVisibility();

		void makeTextFieldsVisible();
		void makeAllAvatarsVisible();

	private:
		uint8_t clicked_id = 0;
};

#endif // SCREEN1VIEW_HPP
