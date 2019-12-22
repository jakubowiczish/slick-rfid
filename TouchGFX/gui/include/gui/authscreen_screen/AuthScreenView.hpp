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

    void invalidateAvatar();
protected:
};

#endif // AUTHSCREENVIEW_HPP
