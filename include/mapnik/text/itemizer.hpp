/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2012 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/

#ifndef MAPNIK_TEXT_ITEMIZER_HPP
#define MAPNIK_TEXT_ITEMIZER_HPP

//mapnik
#include <mapnik/text_properties.hpp> //TODO: Move to text/properties.hpp

// stl
#include <string>
#include <list>

// ICU
#include <unicode/unistr.h>
#include <unicode/uscript.h>
#include <unicode/ubidi.h>
namespace mapnik
{

struct text_item
{
    unsigned start; //First char
    unsigned end; //First char after this item
    UScriptCode script;
    char_properties_ptr format;
    UBiDiDirection rtl;
    text_item() :
        start(0), end(0), script(), format(), rtl(UBIDI_LTR)
    {

    }
};

/** This class splits text into parts which all have the same
 * - direction (LTR, RTL)
 * - format
 * - script (http://en.wikipedia.org/wiki/Scripts_in_Unicode)
 **/
class text_itemizer
{
public:
    text_itemizer();
    void add_text(UnicodeString str, char_properties_ptr format);
    std::list<text_item> const& itemize();
    void clear();
    UnicodeString const& get_text() { return text; }
private:
    template<typename T> struct run
    {
        run(T data, unsigned limit) :  limit(limit), data(data){}
        unsigned limit;
        T data;
    };
    typedef run<char_properties_ptr> format_run_t;
    typedef run<UBiDiDirection> direction_run_t;
    typedef run<UScriptCode> script_run_t;
    UnicodeString text;
    std::list<format_run_t> format_runs;
    std::list<direction_run_t> direction_runs;
    std::list<script_run_t> script_runs;
    void itemize_direction();
    void itemize_script();
    void create_item_list();
    std::list<text_item> output;
};
} //ns mapnik

#endif // TEXT_ITEMIZER_HPP