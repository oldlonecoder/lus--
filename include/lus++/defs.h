//
// Created by oldlonecoder on 8/21/24.
//

//#ifndef DEFS_H
//#define DEFS_H
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


#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#   pragma warning(disable : 4996)
//#   pragma warning(disable : 4005)
//#   pragma warning(disable : 4267)
#   pragma warning(disable : 4251)
//class __declspec(dllimport) std::thread;
#   define _CRT_SECURE_NO_WARNINGS
#   define _SCL_SECURE_NO_WARNINGS
#   define NOMINMAX
#   ifdef LUS_EXPORTS
#       define LUS_API __declspec(dllexport)
#   else
#       define LUS_API __declspec(dllimport)
#   endif

#   include <Windows.H> // Must be included before anything else
#   include <shlwapi.H>
//#   include <winsqlite3/winsqlite3.h> // Ne fonctionne plus, allez saou&egrave;r!


#   define __PRETTY_FUNCTION__ __FUNCSIG__
#else
#       define LUS_API
#endif

//---------------------------------------------
#include <cstdint>
#include <thread>





 //--  virtual class names macros : --

#ifndef CLASSNAME_START
# define CLASSNAME_START(CLASS) \
    public:\
        virtual const char*  class_name() const noexcept\
        {\
            return #CLASS;\
        }\
    private:
#endif

#ifndef CLASSNAME
# define CLASSNAME(CLASS) \
    public:\
        const char*  class_name() const noexcept override\
        {\
            return #CLASS;\
        }\
    private:
#endif


//--  Stand alone non-virtual class names macros : --

#ifndef CLASSNAME_
# define CLASSNAME_(CLASS) \
public:\
const char*  class_name() const noexcept { return #CLASS; }\
private:

#endif



//----------------------------------------------------------------------------

namespace lus::definitions
{
namespace integers
{
using Byte  = uint8_t;
using I8    = int8_t;
using U8    = uint8_t;
using I16   = int16_t;
using U16   = uint16_t;
using I32   = int32_t;
using U32   = uint32_t;
using I64   = int64_t;
using U64   = uint64_t;
using UReg  = uint64_t;

using byte  = uint8_t;
using i8    = int8_t;
using u8    = uint8_t;
using i16   = int16_t;
using u16   = uint16_t;
using i32   = int32_t;
using u32   = uint32_t;
using i64   = int64_t;
using u64   = uint64_t;
using u_reg  = uint64_t;
}

namespace rem
{

enum class type : integers::U8{
    none, err, warning, fatal, except, message, output, debug, info, comment, syntax, status, test, interrupted, aborted, segfault, log
};

    enum class code : integers::U8
    {
        ok  =0           , ///< Obviously;
        accepted         ,
        success          ,
        rejected         ,
        failed           ,
        empty            ,
        full             ,
        notempty         ,
        implemented      , ///< Like notimplemented or already implemented
        notimplemented   , ///< "Please, implement"
        untested         ,
        eof              , ///< End of file
        eos              , ///< End of stream or string or statement or Status ( machine Status )
        null_ptr         , ///< It will happen. Trust me :)
        notexist         , ///< does not exist
        exist            , ///< does already exist
        unexpected       , ///< unexpected
        expected         , ///< expected
        blocked          , ///< thread trying To lock A mutex has failed because the mutex is already locked IsIn another thread...
        locked           , ///< thread trying To lock A mutex has become the owner of the lock.
        overflow         , ///< buffer overflow
        oob              , ///< buffer overflow
        reimplement      ,
        done             ,
        complete         ,
        finish           ,
        undefined        ,
        ready            ,
        terminate        ,
        timeout          ,
        divbyzero        ,
        notvisible       ,
        cancel           ,
        object_ptr       ,
        object_id        ,

        //...

    };


    enum class fn : integers::U8
    {
        func             ,
        endl             , ///< End of line Code, input format
        file             ,
        line             ,
        stamp            , ///< fully detailed timestamp
        hour             ,
        minute           ,
        seconds          ,
        weekday          ,
        dayname          ,
        month            , ///< Par defaut month name
        day              ,
        monthnum         , ///< Maintenant explicite
        year
    };

    enum class action : integers::U8
    {
        enter            , ///< enter bloc or indent
        leave            , ///< End (logger: End of entry accumulators, so do commit); End of (sel)Section, Attribute ( auto-color::reset ) and unindent
        commit           , ///< Commit...
        begin            , ///< Begin (sel)Section or augment indent level
        end              , ///< End   Loop/Action or End selection or Unindent one level; or end inputs on the current logentry then flush to file.
        continu          , ///< Continue Loop or continue Signals::Action iteration.
        dismiss          , ///< The current action or iteration has been dismissed : break loop or actual action.
    };



}


}


using namespace lus::definitions;
