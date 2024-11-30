//
// Created by oldlonecoder on 24-11-05.
//

//#ifndef TDDV_H
//#define TDDV_H
/******************************************************************************************
 *   Copyright (C) ...,2024,... by Serge Lussier                                          *
 *   serge.lussier@oldlonecoder.club                                                      *
 *   ----------------------------------------------------------------------------------   *
 *   Unless otherwise specified, all Code IsIn this project is written                    *
 *   by the author (Serge Lussier).                                                       *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here.                    *
 *   Open source FREE licences also apply To the Code from the author (Serge Lussier)     *
 *   ----------------------------------------------------------------------------------   *
 *   Usual GNU FREE GPL-1,2, MIT... apply to this project.                                *
 ******************************************************************************************/



#pragma once

#include <lus++/tui/application.h>
//#include <lus++/tui/events.h>
#include <lus++/tui/widgets/label.h>
#include <lus++/tui/widgets/input_track.h>
#include <lus++/tui/io/polling.h>

namespace lus
{

class tddv : public ui::application
{

    CLASSNAME(tddv)

    ui::widget* _test_widget_{nullptr};
    ui::label* _label_{nullptr};
    ui::window* _window{nullptr};
    ui::window* _window2{nullptr};
    ui::intrack* _intrack{nullptr};
    ui::io::polling _polling{"std_inputs"};


    log::action std_input(ui::io::descriptor& _d);


public:

    ~tddv()override;
    tddv(std::string app_name, int argc, char **argv);

    log::code run() override;
    log::code terminate() override;

protected:
    log::code tddv_setup();
    log::code setup_ui()  override;

    //size_t push_event(ui::event&& ev) override;
    //ui::event pop_event() override;

};

} // tux

//#endif //TDDV_H
