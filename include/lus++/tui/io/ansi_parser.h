//
// Created by oldlonecoder on 24-11-17.
//

//#ifndef ANSI_PARSER_H
//#define ANSI_PARSER_H
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

#include <lus++/tui/events.h>
#include <lus++/tui/io/polling.h>

namespace lus::ui::io
{



class LUS_API ansi_parser
{
    descriptor _data_{};
public:
    ansi_parser() = default;
    ~ansi_parser() = default;

    explicit ansi_parser(const descriptor& _data);
    //...
    
};


}//namespace lus::ui::io

//#endif //ANSI_PARSER_H
