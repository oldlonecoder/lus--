// Created by oldlonecoder on 8/13/24.
//

//#ifndef INTUI_GLOBALS_H
//#define INTUI_GLOBALS_H
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


#include <lus++/tui/events.h>
#include <csignal>
#include <memory>
#include <lus++/tools/cadres.h>
#include <lus++/tui/widgets/screen.h>
#include <lus++/tui/io/polling.h>
#include <lus++/tui/io/ansi_parser.h>
#include <lus++/tui/io/ansi_colours_parser.h>

namespace lus::ui
{

class LUS_API application
{

    CLASSNAME_START(application)

    static void sig_interrupted(int);
    static void sig_aborted(int);
    static void sig_crash(int);
    static void sig_winch(int);

    std::vector<std::string_view> _args_{};
    std::string _app_name_{};
    static application* _app_;

    events_stream _events_q{"lus++ application global events queue"};
    io::polling _polling{"lus++ application io polling"};

    terminal::screen* _terminal_screen_{nullptr};

public:
    application() = delete;
    virtual ~application();
    application(std::string app_name, int argc, char **argv);
    //application(std::string app_name, int argc, char **argv, char** envp);

    virtual log::code run() = 0; //{ return log::code::notimplemented;}
    virtual log::code terminate();


    static application& app();
    static terminal::screen* screen();
//    static desktop* screen_desktop();
    [[nodiscard]] std::string app_name() const;


protected:
    log::code setup();
    virtual log::code setup_ui();
    virtual log::code install_signals();
    virtual log::code start_events_listening();

    log::action std_infile_no(ui::io::descriptor& _d);


};



}
