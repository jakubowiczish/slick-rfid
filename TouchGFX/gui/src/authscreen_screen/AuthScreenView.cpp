#include <gui/authscreen_screen/AuthScreenView.hpp>
#include "BitmapDatabase.hpp"

AuthScreenView::AuthScreenView()
{

}

void AuthScreenView::setupScreen()
{
    AuthScreenViewBase::setupScreen();
}

void AuthScreenView::tearDownScreen()
{
    AuthScreenViewBase::tearDownScreen();
}

//void AuthScreenView::showAuthScreen(uint8_t uid_tab[], uint8_t id) {
////    showUid(uid_tab);
////    showAvatar(id);
//}

void AuthScreenView::showUid(uint8_t uid_tab[]) {
	Unicode::snprintf(uidTextFieldBuffer, UIDTEXTFIELD_SIZE, "%d %d %d %d", uid_tab[0], uid_tab[1], uid_tab[2], uid_tab[3]);
	uidTextField.invalidate();
}

void AuthScreenView::showAvatar(uint8_t id) {
	if (id == 1) {
	    currentAvatarImage.setBitmap(touchgfx::Bitmap(BITMAP_SZCZYGI_ID));
	    invalidateAvatar();
	} else if (id == 2) {
	    currentAvatarImage.setBitmap(touchgfx::Bitmap(BITMAP_DYBCZAK_ID));
	    invalidateAvatar();
	} else if (id == 3) {
	    currentAvatarImage.setBitmap(touchgfx::Bitmap(BITMAP_RAV_ID));
	    invalidateAvatar();
	} else if (id == 4) {
	    currentAvatarImage.setBitmap(touchgfx::Bitmap(BITMAP_ZAJMA_ID));
	    invalidateAvatar();
	} else if (id == 5) {
	    currentAvatarImage.setBitmap(touchgfx::Bitmap(BITMAP_PLOTNIK_ID));
	    invalidateAvatar();
	} else {
	    currentAvatarImage.setBitmap(touchgfx::Bitmap(BITMAP_PLOTNIK_ID));
	    invalidateAvatar();
	}
}

void AuthScreenView::invalidateAvatar() {
    currentAvatarImage.invalidate();
}
