
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


#include <sys/ioctl.h>
#include <lus++/tui/io/polling.h>


namespace lus::ui::io
{


#pragma region _descriptor_



descriptor& descriptor::set_poll_bits(u16 _bits)
{
    _config_.poll_bits = _bits;
    return *this;
}


/*!
 * @brief private called from polling class when have activity on this descriptor
 * @return log::action::continu or log::action::leave.
 */
log::action descriptor::poll_in()
{
    _buffer_.clear();
    u64 count = 0;
    ioctl(_config_.fd,FIONREAD, &count);
    if(!count)
    {
        _buffer_.clear();
        log::message() << " triggering descriptor on zero-byte signal..." << log::eol;
        return log::action::end;
    }
    log::message() << " triggered descriptor handle #" << color::yellow << _config_.fd << color::reset << ": ioctl to read reports:" << color::lightsteelblue3 << count << log::eol;
    _buffer_.reserve(count);
    if(auto bytes = ::read(_config_.fd,_buffer_.data(), count); bytes != count)
    {
        // discard and flush - ignore unreliable sequence.
        log::error() << "poll_in read error: {" << color::lightsteelblue3 << lus::string::bytes(_buffer_) << log::eol;
        _buffer_.clear();
        return log::action::leave;
    }
    if(!_in.empty())
        return _in(*this);

    // Handled or not, we clear the buffer here before returning for the next event.
    _buffer_.clear();
    return log::action::leave;
}


log::action descriptor::poll_out()
{
    return log::action::continu;
}




/*!
 * @brief Create a new shared bare lus::io::descriptor object
 * @return the reference of the new descriptor config_data to be filled with its proper configs
 */
descriptor::config_data& polling::add_descriptor()
{
    log::info() << " adding a new descriptor - the file handle integer will be set." << log::eol;
    _descriptors_.emplace_back(std::make_shared<descriptor>());
    return _descriptors_.back()->_config_;
}


/*!
 * @brief remove descriptor object identified by its fd number, if found in the _descriptors_ array.
 * @param _fd_id
 * @return
 */
log::code polling::remove_descriptor(int _fd_id)
{
    auto count = _descriptors_.size();
    for(const auto& s: _descriptors_)
        if(s->_config_.fd == _fd_id)
            _descriptors_.erase(std::ranges::find(_descriptors_, s));

    _state_ = _descriptors_.empty() ? state::Terminate : state::Run;
    return count == _descriptors_.size() ? log::code::rejected : log::code::accepted;
}


#pragma endregion _descriptor_

#pragma region _polling_

void polling::update_descriptor_state(descriptor& d, log::action _action)
{
    switch(_action)
    {
        case log::action::continu:
            break;
        case log::action::leave:
        case log::action::end:
            remove_descriptor(d.config().fd);
            break;
        default:break;
    }
}


/*!
 * @brief after all modifiations on the _fds_ pool, rebuild the array.
 */
void polling::reset_pollfds()
{
    log::debug() << " (re)-building modified descriptors list or initial polling setup before first run:" << log::eol;
    delete [] _fds_;
    _fds_ = new pollfd[_descriptors_.size()];
    memset(_fds_, 0, sizeof(pollfd) * _descriptors_.size());
    for(int i=0; i<_descriptors_.size(); ++i)
    {
        _fds_[i].fd = _descriptors_[i]->config().fd;
        _fds_[i].events = _descriptors_[i]->config().poll_bits;

    }
}


polling::~polling()
{
    delete [] _fds_;
}


log::code polling::start_polling()
{
    log::info() << " starting polling..." << log::eol;
    reset_pollfds();
    return run();
}


/*!
 * @brief Starts the loop thread
 * @return log::code
 */
log::code polling::run()
{
    log::info() << " polling loop starting..." << log::eol;
    _state_ = Run;
    while(_state_ != state::Terminate)
    {
        log::debug() << " looping..." << log::eol;
        auto r = poll(_fds_, _descriptors_.size(), -1);
        log::debug() << " poll returned: " << r << log::eol;
        if(r == -1)
        {
            log::error() << "poll() failed:" << std::strerror(errno) << log::eol;
            _state_ = state::Terminate;
            continue;
            // @todo Signal the error... doh!
        }
        if(!r)
        {
            log::status() << " timeout (lightly...) " << log::eol;
            continue;
        }
        for(int f = 0; f < r; f++)
        {
            log::debug() << "revents: " << _fds_[f].revents << log::eol;
            if(_fds_[f].revents & POLLIN)
            {
                if(_descriptors_[f]->_flags_.pause || !_descriptors_[f]->_flags_.active)
                {
                    log::status() << "descriptor index #" << f << "is inactive" << log::eol;
                    continue;
                }

                if(auto a = _descriptors_[f]->poll_in(); a != log::action::continu)
                    update_descriptor_state(*_descriptors_[f],a);
                else
                    _fds_[f].events = _descriptors_[f]->config().poll_bits; // Pas si s&ucirc;r qu'on ait besoin de faire &ccedil;a tout le temps &agrave; chaque cycle...
                continue;
            }
            if(_fds_[f].revents & POLLHUP)
            {
                // todo signal hangup event.
                //_state_ = state::Terminate;
                if(!remove_descriptor(_descriptors_[f]->config().fd))
                    log::warning() << "Descriptor " << _descriptors_[f]->config().fd << " not removed, not in this polling list...(?)" << log::eol;
                update_descriptor_state(*_descriptors_[f], log::action::end);
            } // continue naturally until I add code below ...
        }
        // Write is not yet needed.
    }
    return log::code::done;
}

log::code polling::set_state(polling::state _state)
{
    _state = _state;
    return log::code::accepted;
}

#pragma endregion _polling_

}// namespace lus::ui::io;