
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


#include <lus++/tools/actions.h>
#include <lus++/tools/string.h>
#include <poll.h>

namespace lus::ui::io 
{
using namespace ::integers;

class polling;


/*!
 * @brief descriptor class for the polling io
 *
 * @note As of 2024/11/29, _buffer_ type attr must be change to raw char array.
 */
class LUS_API descriptor final
{
    CLASSNAME_(descriptor)

    struct flags
    {
        u8 active :1; ///< This descriptor is active and monitored. - If not active then it is not monitored...Obviously ;)
        u8 del    :1; ///< This descriptor is inactive , removed and to be deleted.
        u8 pause  :1;
    }_flags_{0,0};
    lus::signals::action<lus::ui::io::descriptor&> _in{std::string("descriptor:in <- ") + "-1"};
    lus::signals::action<lus::ui::io::descriptor&> _out{std::string("descriptor:out -> ") + "-1"};
    friend class polling;


    std::string _buffer_{};

public:
    using shared = std::shared_ptr<descriptor>; ///< fire and forget :) yep! I am being lazy-corrupted! hehehe
    using list= std::vector<descriptor::shared>;

    void init();
    struct config_data
    {
        u16         poll_bits{0};
        size_t      max_length{1024};
        int         fd{-1};
        std::string::iterator cursor{};
    };

    descriptor()=default;
    descriptor(const descriptor&);
    descriptor(descriptor&& )noexcept =default;

    ~descriptor();

    descriptor& operator =(descriptor&& )noexcept =default;
    descriptor& operator =(const descriptor& );
    descriptor& set_poll_bits(u16 _bits);

    std::string buffer() { return  _buffer_; }
    std::string::iterator begin() { return _buffer_.begin(); }
    std::string::iterator end() { return _buffer_.end(); }
    std::string::iterator& cursor() { return _config_.cursor; }

    bool operator ++();
    bool operator ++(int);
    bool operator --();
    bool operator --(int);

    config_data& config(){ return _config_; }
    void activate() { _flags_.active = 1; }
    lus::signals::action<lus::ui::io::descriptor&>& pollin_action() { return _in; }
    lus::signals::action<lus::ui::io::descriptor&>& pollout_action() { return _out; }
private:
    log::action poll_in();
    log::action poll_out();
    descriptor::config_data _config_;
};



/*!
 * @brief   Main thread terminal/console/screen io monitoring loop:
 */
class LUS_API polling final
{
    CLASSNAME_(polling)

    descriptor::list _descriptors_{};
    pollfd* _fds_{nullptr};

    lus::signals::action<lus::ui::io::descriptor&> _polling_started_{};
    lus::signals::action<lus::ui::io::descriptor&> _polling_ended_{};

    enum state : u8
    {
        Run,
        Stop,
        Pause,
        Terminate
    }_state_{Stop};

    std::string _id_{};
    void update_descriptor_state(descriptor& d, log::action _action);
    void reset_pollfds();

public:
    polling()                                    =default;
    polling(const std::string& _id):_id_(_id){}
    ~polling();

    descriptor::list& descriptors(){ return _descriptors_; }
    const std::string& id(){ return _id_; }
    log::code start_polling();
    log::code run();
    log::code stop();
    log::code pause();
    log::code resume();
    log::code reset();
    log::code end();
    log::code set_state(polling::state _state);

    descriptor::config_data& add_descriptor();
    log::code remove_descriptor(int _fd_id);

};



}
