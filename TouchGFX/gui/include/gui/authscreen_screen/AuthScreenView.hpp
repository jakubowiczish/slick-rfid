#ifndef AUTHSCREENVIEW_HPP
#define AUTHSCREENVIEW_HPP

#include <gui_generated/authscreen_screen/AuthScreenViewBase.hpp>
#include <gui/authscreen_screen/AuthScreenPresenter.hpp>

class AuthScreenView : public AuthScreenViewBase
{
public:
    AuthScreenView();
    virtual ~AuthScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void showUid(uint8_t uid_tab[]);

    void showAvatar(uint8_t id);

    void showBitmapAvatar(uint16_t bitmap_value);

    void saveAvatar();

    void invalidateAvatar();

    void szczygiHandler();
    void zajmaHandler();
    void dybczakHandler();
    void plotnikHandler();
    void rafalHandler();
    void tomsiaHandler();
    void radojHandler();
    void capalaHandler();

private:
    uint8_t clickedId;
};

#endif // AUTHSCREENVIEW_HPP
