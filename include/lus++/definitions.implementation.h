/******************************************************************************************
 *   Copyright (C) ...,2024,2025,... by Serge Lussier                                     *
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

#include <lus++/defs.h>
#include <lus++/tools/colors.h>
#include <lus++/tools/glyphes.h>
#include <expected>

namespace lus::definitions
{
std::string LUS_API to_string(rem::type::type enum_type);
std::string LUS_API to_string(rem::code enum_code);
std::string LUS_API to_string(rem::fn   enum_fn);
std::string LUS_API to_string(rem::action enum_action);
std::pair<glyph::type, color::pair> LUS_API type_attributes         (rem::type enum_type);
std::pair<glyph::type, color::pair> LUS_API return_code_attributes  (rem::code enum_code);
std::pair<glyph::type, color::pair> LUS_API function_attributes     (rem::fn enum_fn);
std::pair<glyph::type, color::pair> LUS_API action_attributes       (rem::action enum_action);


}


operator bool(rem::code enum_code);
operator bool(rem::action enum_action);
