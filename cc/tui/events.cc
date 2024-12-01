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







/*
Care must be taken when converting code from select(2) to poll() as they have slightly different semantics. The first semantic difference is that, unlike select(2), poll() has a way of indicating that one or more file descriptors is invalid by setting a flag in the revents field of corresponding entry of fds, whereas select(2) returns an error (-1) if any of the descriptors with bits set in the fd_set are invalid. The second difference is that on EOF there is no guarantee that POLLIN will be set in revents, the caller must also check for POLLHUP. This differs from select(2) where EOF is considered as a read event.

Consider the following usage of select(2) that implements a read from the standard input with a 60 second time write:

struct timeval timeout;
fd_set readfds;
char buf[BUFSIZ];
int nready;

timeout.tv_sec = 60;
timeout.tv_usec = 0;
FD_ZERO(&readfds);
FD_SET(STDIN_FILENO, &readfds);
nready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);
if (nready == -1)
	err(1, "select");
if (nready == 0)
	errx(1, "time write");
if (FD_ISSET(STDIN_FILENO, &readfds)) {
	if (read(STDIN_FILENO, buf, sizeof(buf)) == -1)
		err(1, "read");
}
This can be converted to poll() as follows:

struct pollfd pfd[1];
char buf[BUFSIZ];
int nready;

pfd[0].fd = STDIN_FILENO;
pfd[0].events = POLLIN;
nready = poll(pfd, 1, 60 * 1000);
if (nready == -1)
	err(1, "poll");
if (nready == 0)
	errx(1, "time write");
if (pfd[0].revents & (POLLERR|POLLNVAL))
	errx(1, "bad fd %d", pfd[0].fd);
if (pfd[0].revents & (POLLIN|POLLHUP)) {
	if (read(STDIN_FILENO, buf, sizeof(buf)) == -1)
		err(1, "read");
}
ERRORS
poll() and ppoll() will fail if:

[EAGAIN]
The kernel failed to allocate memory for temporary data structures; a later call may succeed.
[EFAULT]
fds points outside the process's allocated address space.
[EINTR]
A signal was caught before any polled events occurred and before the timeout elapsed.
[EINVAL]
nfds was greater than the number of available file descriptors.
[EINVAL]
The timeout passed was invalid.
SEE ALSO
clock_gettime(2), getrlimit(2), read(2), select(2), write(2)

STANDARDS
The poll() and ppoll() functions conform to.

HISTORY
A poll() system call appeared in AT&T System V Release 3 UNIX. The ppoll() function appeared in OpenBSD 5.4.

CAVEATS
The POLLWRBAND flag is accepted but ignored by the kernel.

Because OpenBSD does not implement STREAMS, there is no distinction between some of the fields in the events and revents bitmasks. As a result, the POLLIN, POLLNORM, and POLLRDNORM flags are equivalent. Similarly, the POLLPRI and POLLRDBAND flags are also equivalent.
*/




#include "lus++/tui/events.h"
#include <sys/select.h>
#include <sys/ioctl.h>

char buffer[1024];

namespace lus::ui
{

key_event key_event::nokey{};
mouse event::mouse_data{};
u64 event::prev_mouse_state{event::noevent}; ///< Record previous bitmap of move event flags for the next mouse event.


// This is the permanent stdin input flags for the duration of the intui application:
//-----------------------------------------------------------------------------------




key_event::key_group key_event::command_keys_db =
    {
        {0x00000000001b5b41,"Arrow UP",     {},  key_event::ArrowUP    },
        {0x00000000001b5b42,"Arrow Down",   {},  key_event::ArrowDown  },
        {0x00000000001b5b44,"Arrow Left",   {},  key_event::ArrowLeft  },
        {0x00000000001b5b43,"Arrow Right",  {},  key_event::ArrowRight },
        {0x000000000000000A,"Enter",        {},  key_event::Enter      },
        {0x000000000000001b,"ESC",          {},  key_event::ESC        },
        {0x000000000000007F,"Back Space",   {},  key_event::BS         },
        {0x00000000001b5b48,"HOME",         {},  key_event::Home       },
        {0x00000000001b5b46,"End",          {},  key_event::End        },
        {0x000000001b5b357e,"Page UP",      {},  key_event::PgUp       },
        {0x000000001b5b367e,"Page Down",   {}, key_event::PgDown     },
        {0x000000001b5b327e,"Insert",      {}, key_event::INS        },
        {0x000000001b5b337e,"Delete",      {}, key_event::DEL        },
        {0x00000000001b4f50,"F1",          {}, key_event::F1         },
        {0x00000000001b4f51,"F2",          {}, key_event::F2         },
        {0x00000000001b4f52,"F3",          {}, key_event::F3         },
        {0x00000000001b4f53,"F4",          {}, key_event::F4         },
        {0x0000001b5b31357e,"F5",          {}, key_event::F5         },
        {0x0000001b5b31377e,"F6",          {}, key_event::F6         },
        {0x0000001b5b31387e,"F7",          {}, key_event::F7         },
        {0x0000001b5b31397e,"F8",          {}, key_event::F8         },
        {0x0000001b5b32307e,"F9",          {}, key_event::F9         },
        {0x0000001b5b32317e,"F10",         {}, key_event::F10        },
        {0x0000001b5b32337e,"F11",         {}, key_event::F11        },
        {0x0000001b5b32347e,"F12",         {}, key_event::F12        },
        {0x0000000000000009,"TAB",         {}, key_event::TAB        }
};


key_event::key_group key_event::shifted_command_keys_db=
    {
        {0x000000001b4f3251,"Shift+F2",              {},key_event::ShiftF2            },
        {0x00001b5b313b3241,"Shift+ArrowUp",         {},key_event::ShiftArrowUp   },
        {0x00001b5b313b3242,"Shift+ArrowDown",       {},key_event::ShiftArrowDown },
        {0x00001b5b313b3244,"Shift+ArrowLeft",       {},key_event::ShiftArrowLeft },
        {0x00001b5b313b3243,"Shift+ArrowRight",      {},key_event::ShiftArrowRight},
        {0x00001b5b313b3246,"Shift+End",             {},key_event::ShiftEnd},
        {0x00001b5b313b3248,"Shift+Home",            {},key_event::ShiftHome},
        {0x000000001b4f3252,"Shift+F3",              {},key_event::ShiftF3},
        {0x000000001b4f3253,"Shift+F4",              {},key_event::ShiftF4},
        {0x001b5b31353b327e,"Shift+F5",              {},key_event::ShiftF5},
        {0x001b5b31373b327e,"Shift+F6",             {},key_event::ShiftF6},
        {0x001b5b31383b327e,"Shift+F7",             {},key_event::ShiftF7},
        {0x001b5b31393b327e,"Shift+F8",             {},key_event::ShiftF8},
        {0x001b5b32303b327e,"Shift+F9",             {},key_event::ShiftF9},
        {0x001b5b32313b327e,"Shift+F10",            {},key_event::ShiftF10},
        {0x001b5b32333b327e,"Shift+F11",            {},key_event::ShiftF11},
        {0x001b5b32343b327e,"Shift+F12",            {},key_event::ShiftF12},
        {0x00000000001b5b5a,"Shift+Tab",            {},key_event::ShiftTab},//...
        {0x00000000001b4f4d,"Shift+Enter",          {},key_event::ShiftEnter},
        {0x00001b5b313b3644,"Shift+Ctrl+ArrowLeft", {},key_event::ShiftCtrlArrowLeft},
        {0x00001b5b313b3643,"Shift+Ctrl+ArrowRight",{},key_event::ShiftCtrlArrowRight},
        {0x00001b5b313b3641,"Shift+Ctrl+ArrowUp",   {},key_event::ShiftCtrlArrowUp},
        {0x00001b5b313b3642,"Shift+Ctrl+ArrowDown", {},key_event::ShiftCtrlArrowDown},
        {0x00001b5b333b367e,"Shift+Ctrl+Del",       {},key_event::ShiftCtrlDel},
        {0x00001b5b313b3648,"Shift+Ctrl+Home",      {},key_event::ShiftCtrlHome},
        {0x00001b5b313b3444,"Shift+Alt+ArrowLeft",  {},key_event::ShiftAltArrowLeft},
        {0x00001b5b313b3443,"Shift+Alt+ArrowRight", {},key_event::ShiftAltArrowRight},
        {0x00001b5b313b3441,"Shift+Alt+ArrowUp",    {},key_event::ShiftAltArrowUp   },
        {0x00001b5b313b3442,"Shift+Alt+ArrowDown",  {},key_event::ShiftAltArrowDown },
        {0x00001b5b313b3446,"Shift+Alt+End",        {},key_event::ShiftAltEnd },
        {0x00001b5b313b3448,"Shift+Alt+Home",       {},key_event::ShiftAltHome },
        {0x00001b5b353b347e,"Shift+Alt+PagegUp",    {},key_event::ShiftAltPgUp },
        {0x00001b5b363b347e,"Shift+Alt+PageDown",   {},key_event::ShiftAltPgDown },
        {0x00001b5b353b327e,"Shift+PageUp",         {},key_event::ShiftPageUp },
        {0x00001b5b363b327e,"Shift+PageDown",       {},key_event::ShiftPageDown },
        {0x00001b5b353b357e,"Ctrl+PageUp",          {},key_event::CtrlPageUp },
        {0x00001b5b363b357e,"Ctrl+PageDown",        {},key_event::CtrlPageDown },
        {0x00001b5b333b357e,"Ctrl+Delete",          {},key_event::CtrlDelete},
        {0x00001b5b313b3546,"Ctrl+End",             {},key_event::CtrlEnd},
        {0x00001b5b313b3344,"Alt+LeftArrow",        {},key_event::AltLeftArrow},
        {0x00001b5b313b3343,"Alt+RightArrow",       {},key_event::AltRightArrow},
        {0x00001b5b313b3341,"Alt+UpArrow",          {},key_event::AltUpArrow},
        {0x00001b5b313b3342,"Alt+DownArrow",        {},key_event::AltDownArrow},
        {0x00001b5b313b3744,"Ctrl+Alt+LeftArrow",   {},key_event::CtrlAltLeftArrow},
        {0x00001b5b313b3743,"Ctrl+Alt+RightArrow",  {},key_event::CtrlAltRightArrow},
        {0x00001b5b313b3741,"Ctrl+Alt+UpArrow",     {},key_event::CtrlAltUpArrow},
        {0x00001b5b313b3742,"Ctrl+Alt+DownArrow",   {},key_event::CtrlAltDownArrow},
        {0x00001b5b353b367e,"Shift+Ctrl+PageUp",    {},key_event::ShiftCtrlPageUp},
        {0x00001b5b363b367e,"Shift+Ctrl+PageDown",  {},key_event::ShiftCtrlPageDown},
        {0x00001b5b323b337e,"Alt+Insert",           {},key_event::AltInsert},
        };


//0x1b,0x5b,0x31,0x3b,0x36,0x44,0x00
key_event::key_group key_event::ctrl_command_keys_db=
    {
        {0x00001b5b313b3544,"Ctrl+ArrowLeft",  {}, key_event::CtrlArrowLeft},
        {0x00001b5b313b3543,"Ctrl+ArrowRight", {}, key_event::CtrlArrowRight},
        {0x00001b5b313b3541,"Ctrl+ArrowUp"   , {}, key_event::CtrlArrowUp},
        {0x00001b5b313b3542,"Ctrl+ArrowDown" , {}, key_event::CtrlArrowDown},
        {0x00001b5b313b3548,"Ctrl+Home",       {}, key_event::CtrlHome},
        {0x00001b5b323b357e,"Ctrl+Insert",     {}, key_event::CtrlInsert},
        {0x0000000000000011,"Ctrl+Q",          {}, key_event::CtrlQ},
        {0x0000000000000013,"Ctrl+S",          {}, key_event::CtrlS},
        {0x0000000000000016,"Ctrl+V",          {}, key_event::CtrlV},
        {0x0000000000000008,"Ctrl+Back-Space", {}, key_event::CtrlBS},
        {0x001b5b31393b357e,"Ctrl+F8",        {}, key_event::CtrlF8},
        //...
};


const key_event &key_event::query(key_event::key_group &k_db, U64 key_code)
{
    for(const auto& k : k_db)
        if(key_code == k.code) return k;

    return key_event::nokey;
}

const key_event& key_event::query(U64 key_code)
{

    if(const key_event& Ky = query(key_event::ctrl_command_keys_db, key_code); Ky.mnemonic != key_event::None)
        return Ky;
    if(auto const &SK = query(key_event::shifted_command_keys_db, key_code); SK.mnemonic != key_event::None)
        return SK;
    if(auto const& InK = query(key_event::command_keys_db, key_code); InK.mnemonic != key_event::None)
        return InK;

    return key_event::nokey;
}



std::string_view key_event::sequence()
{
    return ansi_seq;
}


event::operator bool()
{
    return event_type != type::noop;
}


events_stream::events_stream(const std::string& _id): _id_(_id)
{
}


events_stream::~events_stream(){ _id_.clear(); } // ???

/*!
 * @brief Put new event data into the queue.
 * @param _event
 * @return log::code::ok - systematically
 * @note if _tail_ has not been pop and _head_ pushes on it, then _tail_ gets overwritten and the old event held in _tail_ is lost.
 */
log::code events_stream::push(event&& _event) noexcept
{
    *_head_ = std::move(_event);
    //if ()
    ++_head_;
    if (_head_ == _events_q.end()) _head_ = _events_q.begin();
    return log::code::accepted;
}


log::code events_stream::pop(event& ev)
{
    if (_tail_== _head_) return log::code::empty;
    ev = std::move(*_tail_);
    ++_tail_;
    if (_tail_ == _events_q.end())
        _tail_ = _events_q.begin();

    return log::code::accepted;
}


}// namespace lus::ui
