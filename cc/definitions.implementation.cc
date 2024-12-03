//
// Created by oldlonecoder on 24-12-03.
//


#include <lus++/definitions.implementation.h>



bool operator !(rem::code c)
{
    return (c != rem::code::accepted)&&
           (c != rem::code::ok)      &&
           (c != rem::code::success) &&
           (c != rem::code::done)    &&
           (c != rem::code::ready)   &&
           (c != rem::code::finish)  &&
           (c != rem::code::complete);
}

operator bool(rem::action enum_action)
{
    return (enum_action != rem::action::continu)&&
       (enum_action != rem::action::enter)      &&
       (enum_action != rem::action::commit) &&
       (enum_action != rem::action::begin);
}



namespace lus::definitions::rem
{


std::map<rem::type, const char*> enums_type_dictionary = {
    {rem::type::none,        "none"},
    {rem::type::err  ,       "err"},
    {rem::type::warning,     "warning"},
    {rem::type::fatal,       "fatal"},
    {rem::type::except,      "exception"},
    {rem::type::message,     "message"},
    {rem::type::output,      ""},
    {rem::type::debug,       "debug"},
    {rem::type::info,        "info"},
    {rem::type::comment,     "comment"},
    {rem::type::syntax,      "syntax error"},
    {rem::type::status,      "status"},
    {rem::type::test,        "test"},
    {rem::type::interrupted, "interrupted"},
    {rem::type::aborted,     "aborted"},
    {rem::type::segfault,    "segmentation fault"},
    {rem::type::log,         "log"}
};


std::map<rem::code, const char*> return_codes_dictionary = {
    {rem::code::ok,             "ok"},
    {rem::code::accepted,       "accepted"},
    {rem::code::success,        "success "},
    {rem::code::rejected,       "rejected"},
    {rem::code::failed,         "failed"},
    {rem::code::empty,          "empty"},
    {rem::code::full,           "full"},
    {rem::code::notempty,       "not empty"},
    {rem::code::implemented,    "implemented"},
    {rem::code::notimplemented, "not implemented"},
    {rem::code::untested,       "untested"},
    {rem::code::eof,            "end of file"},
    {rem::code::eos,            "end of stream or string"},
    {rem::code::null_ptr,       "null pointer"},
    {rem::code::notexist,       "does not exist"},
    {rem::code::exist,          "exist"},
    {rem::code::unexpected,     "unexpected"},
    {rem::code::expected,       "expected"},
    {rem::code::blocked,        "blocked"},
    {rem::code::locked,         "locked"},
    {rem::code::overflow,       "buffer overflow"},
    {rem::code::oob,            "out of boundaries"},
    {rem::code::reimplement,    "need to be re-implemented in derived instances"},
    {rem::code::done,           "done"},
    {rem::code::complete,       "completed"},
    {rem::code::finish,         "finished"},
    {rem::code::undefined,      "undefined"},
    {rem::code::ready,          "Ready"},
    {rem::code::terminate,      "terminate"},
    {rem::code::timeout,        "timeout"},
    {rem::code::divbyzero,      "division by zero"},
    {rem::code::notvisible,     "not visible"},
    {rem::code::cancel,         "cancel"},
    {rem::code::object_ptr,     "object"},
    {rem::code::object_id,      "object id"},
};


std::map<rem::action, const char*> actions_dictionary= {
    {rem::action::enter,   "enter"},
    {rem::action::leave,   "leave"},
    {rem::action::begin,   "begin"},
    {rem::action::end,     "end"},
    {rem::action::commit,  "commit"},
    {rem::action::continu, "continue"},
    {rem::action::dismiss, "dismissed"},
};


std::map<rem::fn, const char*> functions_dictionary= {

    {rem::fn::endl,           "end line"},
    {rem::fn::func,            "function"},
    {rem::fn::file,           "file"},
    {rem::fn::line,           "line"},
    {rem::fn::stamp,          "stamp"},
    {rem::fn::hour,           "hour"},
    {rem::fn::minute,         "minute"},
    {rem::fn::seconds,        "seconds"},
    {rem::fn::weekday,        "weekday"},
    {rem::fn::month,          "month name"},
    {rem::fn::dayname,        "day name"},
    {rem::fn::day,            "day"},
    {rem::fn::monthnum,       "month number"},
    {rem::fn::year,           "year"}
};



std::map<rem::type, std::pair<glyph::type, color::pair>> types_database={

    {rem::type::none,        {glyph::computer,  {color::aquamarine1         , color::reset }}},
    {rem::type::err,         {glyph::err1,      {color::red4                , color::reset }}},
    {rem::type::warning,     {glyph::warning,   {color::yellow              , color::reset }}},
    {rem::type::fatal,       {glyph::dead_head, {color::hotpink4            , color::reset }}},
    {rem::type::except,      {glyph::bolt,      {color::skyblue3            , color::reset }}},
    {rem::type::message,     {glyph::comment,   {color::green4              , color::reset }}},
    {rem::type::output,      {glyph::notepad,   {color::grey39              , color::reset }}},
    {rem::type::debug,       {glyph::bug,       {color::pink3               , color::reset }}},
    {rem::type::info,        {glyph::info,      {color::lightskyblue4       , color::reset }}},
    {rem::type::comment,     {glyph::comment,   {color::grey42              , color::reset }}},
    {rem::type::syntax,      {glyph::handwrite, {color::lighcoreateblue     , color::reset }}},
    {rem::type::status,      {glyph::admin,     {color::lightcyan3          , color::reset }}},
    {rem::type::test,        {glyph::file,      {color::aquamarine3         , color::reset }}},
    {rem::type::interrupted, {glyph::circle_x,  {color::khaki3              , color::reset }}},
    {rem::type::aborted,     {glyph::stop,      {color::red4                , color::reset }}},
    {rem::type::segfault,    {glyph::bomb,      {color::sandybrown          , color::reset }}},
    {rem::type::log,         {glyph::log,       {color::orangered1          , color::reset }}}
    //...
};


std::map<rem::action      , std::pair<glyph::type, color::pair>> actions_color_db = {
    {rem::action::enter   , {glyph::enter        , {color::white               , color::reset}}},
    {rem::action::leave   , {glyph::file         , {color::white               , color::reset}}},
    {rem::action::begin   , {glyph::chronos      , {color::white               , color::reset}}},
    {rem::action::end     , {glyph::dead_head    , {color::white               , color::reset}}},
    {rem::action::commit  , {glyph::pencil_dr    , {color::white               , color::reset}}},
    {rem::action::continu , {glyph::success      , {color::white               , color::reset}}},
       {rem::action::dismiss , {glyph::arrow_dhead_right , {color::yellow         , color::reset}}},
};

std::map<rem::code, std::pair<glyph::type, color::pair>> codes_database={
    {rem::code::ok,              {glyph::thumb_up,        {color::lime              ,color::reset }}},
    {rem::code::accepted,        {glyph::thumb_up,        {color::lime              ,color::reset }}},
    {rem::code::success,         {glyph::success,         {color::darkgreen         ,color::reset }}},
    {rem::code::rejected,        {glyph::thumb_down,      {color::hotpink4          ,color::reset }}},
    {rem::code::failed,          {glyph::poop,            {color::darkgoldenrod     ,color::reset }}},
    {rem::code::empty,           {glyph::arrowright,      {color::lighcoreategrey   ,color::reset }}},
    {rem::code::full,            {glyph::small_dot,       {color::white             ,color::reset }}},
    {rem::code::notempty,        {glyph::big_dot,         {color::white             ,color::reset }}},
    {rem::code::implemented,     {glyph::arrow_head_right,{color::lime              ,color::reset }}},
    {rem::code::notimplemented,  {glyph::err1,            {color::orange3           ,color::reset }}},
    {rem::code::untested,        {glyph::flag,            {color::yellow            ,color::reset }}},
    {rem::code::eof,             {glyph::baseline,        {color::white             ,color::reset }}},
    {rem::code::eos,             {glyph::baseline,        {color::white             ,color::reset }}},
    {rem::code::null_ptr,        {glyph::circle_x,        {color::hotpink4            ,color::reset }}},
    {rem::code::notexist  ,      {glyph::circle_x ,       {color::white               ,color::reset }}},
    {rem::code::exist     ,      {glyph::star5 ,          {color::white             ,color::reset }}},
    {rem::code::unexpected,      {glyph::flag ,           {color::yellow            ,color::reset }}},
    {rem::code::expected  ,      {glyph::cat ,            {color::white             ,color::reset }}},
    {rem::code::blocked   ,      {glyph::unauth ,         {color::indianred3        ,color::reset }}},
    {rem::code::locked    ,      {glyph::err3 ,           {color::white             ,color::reset }}},
    {rem::code::overflow  ,      {glyph::dead_head,       {color::red4              ,color::reset }}},
    {rem::code::oob       ,      {glyph::alien,           {color::lightcoral        ,color::reset }}},
    {rem::code::reimplement,     {glyph::log,             {color::yellow            ,color::reset }}},
    {rem::code::done       ,     {glyph::ok,              {color::yellow            ,color::reset }}},
    {rem::code::complete   ,     {glyph::ok,              {color::yellow            ,color::reset }}},
    {rem::code::finish     ,     {glyph::ok,              {color::lime              ,color::reset }}},
    {rem::code::undefined  ,     {glyph::err3,            {color::red               ,color::reset }}},
    {rem::code::ready      ,     {glyph::ok,              {color::lime              ,color::reset }}},
    {rem::code::terminate  ,     {glyph::flag,            {color::hotpink4          ,color::reset }}},
    {rem::code::timeout    ,     {glyph::chronos,         {color::lime              ,color::reset }}},
    {rem::code::divbyzero  ,     {glyph::circle_x,        {color::red4                ,color::reset }}},
    {rem::code::notvisible ,     {glyph::circle_x,        {color::yellow              ,color::reset }}},
    {rem::code::cancel     ,     {glyph::circle_x,        {color::red4                ,color::reset }}},
    {rem::code::object_ptr ,     {glyph::edit_pencil_br,{color::lightsteelblue3     ,color::reset }}},
    {rem::code::object_id ,      {glyph::arrowright,      {color::yellow            ,color::reset }}},

};


std::map<rem::fn, std::pair<glyph::type, color::pair>> functions_database={

    {rem::fn::stamp     ,      {glyph::chronos  ,   {color::skyblue3          ,color::reset }}},
    {rem::fn::func      ,      {glyph::function ,   {color::skyblue3          ,color::reset }}},
    {rem::fn::file      ,      {glyph::file     ,   {color::lightcyan3        ,color::reset }}},
    {rem::fn::line      ,      {glyph::baseline ,   {color::lime              ,color::reset }}},
    {rem::fn::stamp     ,      {glyph::chronos  ,   {color::yellow            ,color::reset }}},
    {rem::fn::hour      ,      {glyph::chronos  ,   {color::white             ,color::reset }}},
    {rem::fn::minute    ,      {glyph::chronos  ,   {color::white             ,color::reset }}},
    {rem::fn::seconds   ,      {glyph::chronos  ,   {color::white             ,color::reset }}},
    {rem::fn::weekday   ,      {glyph::fabric   ,   {color::yellow            ,color::reset }}},
    {rem::fn::month     ,      {glyph::chronos  ,   {color::white             ,color::reset }}},
    {rem::fn::dayname   ,      {glyph::star5    ,   {color::white             ,color::reset }}},
    {rem::fn::day       ,      {glyph::star5    ,   {color::white             ,color::reset }}},
    {rem::fn::monthnum  ,      {glyph::star5    ,   {color::white             ,color::reset }}},
    {rem::fn::year      ,      {glyph::star5    ,   {color::white             ,color::reset }}}

};


std::string rem::to_string(rem::type ty)
{
    return enums_type_dictionary[ty];
}

std::string rem::to_string(rem::code cod)
{
    return return_codes_dictionary[cod];
}


std::pair<glyph::type, color::pair> rem::type_attributes(rem::type ty)
{
    return types_database[ty];
}

std::pair<glyph::type, color::pair> rem::return_code_attributes(rem::code cod)
{
    return codes_database[cod];
}

std::pair<glyph::type, color::pair> rem::function_attributes(rem::fn fn)
{
    return functions_database[fn];
}

std::pair<glyph::type, color::pair> rem::action_attributes(rem::action a)
{
    return actions_color_db[a];
}

std::string rem::to_string(rem::fn fn)
{
    return functions_dictionary[fn];
}

std::string rem::to_string(rem::action ac)
{
    return actions_dictionary[ac];
}


} // namespace lus::definitions::rem
