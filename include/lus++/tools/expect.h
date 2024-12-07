//
// Created by oldlonecoder on 24-12-07.
//

//#ifndef EXPECT_H
//#define EXPECT_H
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

#include <lus++/definitions.implementation.h>
#include <any>
#include <optional>
#include <lus++/tools/logger.h>

namespace lus
{
/*!
 * @brief Trying to implement my own very specific expect return value class using std::any and std::optional.

 * @author Serge Lussier (serge.lussier@oldlonecoder.club)
 *
 */
template <typename T> class expect
{
    std::any _expected_{};
    rem::type _error_{rem::type::none};

public:
    expect() : _expected_() {}
    explicit expect(std::any expected) : _expected_(expected) {}
    expect(const expect<T> &other) : _expected_(other._expected_) {}
    expect(expect<T> &other) : _expected_(other._expected_) {}
    expect(expect<T> &&other) noexcept : _expected_(other._expected_) {}
    expect(log& unexpected)
    {
        _expected_ = unexpected;
        _error_ = static_cast<rem::type>(std::any_cast<log&>(_expected_).entry_type());
    }

    expect<T> &operator=(const expect<T> &other) = default;
    expect<T> &operator=(expect<T> &other) = default;
    expect<T> &operator=(expect<T> &&other) noexcept = default;
    expect<T> &operator=(log &unexpected) { _expected_ = unexpected; _error_ = static_cast<rem::type>(unexpected.entry_type()); return *this; }

    ~expect(){ _expected_.reset(); }

    const std::any &value() const { return _expected_; }

    void reset() { _expected_.reset(); }

    auto operator *() -> std::optional<T>
    {
        if (_error_ == rem::type::none)
            return std::any_cast<T>(_expected_);
        return false;
    }

    auto operator()()-> std::optional<T>
    {
        if (_error_ == rem::type::none)
            return std::any_cast<T>(_expected_);
        return false;
    }

    explicit operator bool() const {return _error_ == rem::type::none && _expected_.has_value();}
    bool operator !() const {return _error_ != rem::type::none;} //&& _expected_.has_value();}

};


}
