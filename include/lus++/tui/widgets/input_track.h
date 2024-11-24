// Created by oldlonecoder on 8/13/24.
//

//#ifndef INTUI_GLOBALS_H
//#define INTUI_GLOBALS_H
/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all Code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/


#pragma once

#include <lus++/tui/widget.h>
#include <lus++/tui/events.h>

namespace lus::ui
{


/**
 * @brief This widget is used to display the keyboard and mouse events.
 * 
 */
class LUS_API intrack : public widget
{

    CLASSNAME(intrack)
    event ev{}; ///< kev max length = 21 chars. ; mouse repost : 25 chars
    
    
    rectangle _key_area_{{0,0},ui::size{21,1}};
    rectangle _mouse_area_{{21,0},ui::size{25,1}};

    widget::painter_dc _key_stat_painter_;
    widget::painter_dc _mouse_stat_painter_;


public:
    intrack();
    intrack(widget* _parent, const std::string& a_id);
    ~intrack() override {};
    log::action update_event_data(const event& e);
    log::code setup_ui() override;

protected:
    log::code draw() override;
    

};

}
