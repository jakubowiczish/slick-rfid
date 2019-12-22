#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void setUidText(uint8_t uid_tab[]);
    uint8_t showChosenAvatar(uint8_t id);

protected:

private:
    void plotnikHandler(uint8_t id);
    void zajmaHandler(uint8_t id);
    void szczygiHandler(uint8_t id);
    void dybczakHandler(uint8_t id);
    void rafalHandler(uint8_t id);
    uint8_t clicked_id = 0;
};


#endif // SCREEN1VIEW_HPP
