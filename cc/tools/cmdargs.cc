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


#include <lus++/tools/cmdargs.h>

namespace lus::cmd
{


cmd_switch::iterator line::query(std::string_view SwitchData)
{
    auto It = arguments.begin();
    for (; It != arguments.end(); It++)
    {
        if ((SwitchData == (*It)->switch_text ) || (SwitchData == (*It)->switch_char))
            return It;
    }
    return arguments.end();
}

line::~line()
{
    arguments.clear(); // doh ...
    log::info() << log::fn::func << "arguments switches infos are cleared.";
}


/*!
 * @brief input operator
 * @param Arg
 * @return Pointer to cmd_switch instance.
 * @author &copy;2023/2024, oldlonecoder/serge.lussier@oldlonecoder.club
 *
 * @note Because of the action<>, We must instantiate a unique non-copiable cmd_switch using <i>new</i> to ensure
 * that there will be NO move/copy of the instances. We may instantiate directly into the arguments using emplace_back tho. -
 * to be considered...
 */
cmd_switch &line::operator<<(const cmd_switch &Arg)
{
    arguments.push_back(new cmd_switch);
    cmd_switch* Sw = arguments.back();
    Sw->name = Arg.name;
    Sw->switch_char = Arg.switch_char;
    Sw->switch_text = Arg.switch_text;
    Sw->descriptions = Arg.descriptions;
    Sw->required = Arg.required;

    return *Sw;
}

log::code line::input(const std::vector<std::string_view>& StrArray)
{
    auto a = arguments.end();

    if(StrArray.empty())
        return log::code::empty;

    for(auto sv : StrArray)
    {
        auto next = query(sv);

        log::debug() << color::cyan << "line::input" << color::white << '(' << color::yellow << sv << color::white << ") :";

        if(next == arguments.end())
        {
            // It is not a switch - so must be an argument data for the CurArg/NextArg...
            if((a != arguments.end()) && 
                ((*a)->required > (*a)->count) && ((*a)->required > 0))
            {
                (*a)->arguments.emplace_back(sv);
                ++(*a)->count;
                (*a)->enabled = true;
                log::debug() << color::yellow << (*a)->name << color::reset << " arg:" << (*a)->count << " '" << sv << '\'';
            }
            else
            {
                // ... or non-switch arg
                defaults.arguments.emplace_back(sv);
                ++defaults.count;
                defaults.enabled = true;
                log::debug() << color::yellow << defaults.name << color::reset << " arg:" << defaults.count << " '" << sv << '\'';
            }
        }
        else
        {
            if ((a != arguments.end()) && ((*a)->count < (*a)->required))
            {
                log::error() << " argument " << color::yellow << (*a)->name << color::reset << " is missing " << color::red4 << (*a)->required - (*a)->count << color::reset << " arguments / " << color::lime << (*a)->required;
                return log::code::failed;
            }
            a = next;
            (*a)->enabled = true;    
        }
    }
    return log::code::ok;
}


log::action  line::process()
{
    auto r = log::action::end;
    if (!arguments.empty())
    {
        for (auto* arg : arguments)
        {
            if (arg->enabled)
            {
                if (arg->required > arg->count)    
                {
                    arg->enabled = false;
                    throw log::except() << " cmd_switch '" << color::yellow << arg->name
                        << color::reset << " is missing "
                        << color::red4 << arg->required - arg->count
                        << color::reset << " arguments: requires "
                        << color::lime << arg->required;
                    
                    // throw did not work? :
                    log::except() << " if this is shown, it means exceptions are deactivated. i suggest to enable it... ";
                    log::write() << usage();
                    return log::action::leave;

                }
                if (arg->delegate_call_back(*arg) != log::action::continu) 
                    return log::action::leave;
            }
        }
    }
    if(!defaults.arguments.empty())
    {
        if(defaults.enabled)
        {
             r = defaults.delegate_call_back(defaults);
             if (r != log::action::continu)
                 return log::action::leave;
        }
    }
    return log::action::continu;
}

cmd_switch &line::operator[](const std::string &ArgName)
{
    for(auto* Arg :arguments)
    {
        if(Arg->name == ArgName) return *Arg;
    }
    return defaults;
}

std::string line::usage()
{
    lus::string Str;
    Str << "usage:\n";
    lus::string Unused, Used, Used0;
    Unused = Used = "%s%s";
    Unused << color::red1 << glyph::unauth;
    Used   << color::lightgreen2 << glyph::ok;

    Str << "------------------------------------------------------------------------------\n";
    for (auto* Arg : arguments)
    {
        Str << "%-2s | %-20s | %-45s %s" <<
            Arg->switch_char <<
            Arg->switch_text <<
            Arg->descriptions <<
            (Arg->enabled ? Used : Unused) <<
            color::reset << '\n';

        Str << "------------------------------------------------------------------------------\n";
    }
    return Str();

}


cmd_switch &line::add_cmd(const std::string &cId)
{
    arguments.push_back(new cmd_switch);
    cmd_switch* Sw = arguments.back();
    Sw->name = cId;
    return *Sw;
}


cmd_switch::~cmd_switch()
{
    arguments.clear();
    delegate_call_back.disconnect_all();
}

cmd_switch &cmd_switch::set_descriptions(const std::string &Txt)
{
    descriptions = Txt;
    return *this;
}

cmd_switch &cmd_switch::set_char(const std::string &Txt)
{
    switch_char = Txt;
    return *this;
}

cmd_switch &cmd_switch::set_long_switch(const std::string &Txt)
{
    switch_text = Txt;
    return *this;
}

cmd_switch &cmd_switch::set_required(int N)
{
    required = N;
    return *this;
}

}