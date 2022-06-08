
/***********************************************************************************
** exvr-designer                                                                  **
** MIT License                                                                    **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                                **
** Permission is hereby granted, free of charge, to any person obtaining a copy   **
** of this software and associated documentation files (the "Software"), to deal  **
** in the Software without restriction, including without limitation the rights   **
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      **
** copies of the Software, and to permit persons to whom the Software is          **
** furnished to do so, subject to the following conditions:                       **
**                                                                                **
** The above copyright notice and this permission notice shall be included in all **
** copies or substantial portions of the Software.                                **
**                                                                                **
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     **
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       **
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    **
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         **
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  **
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  **
** SOFTWARE.                                                                      **
************************************************************************************/

#pragma once

// std
#include <iostream>

// qt-utility
#include "qt_str.hpp"
#include "data/id_key.hpp"

// local
#include "interval.hpp"

namespace tool::ex {


struct Timeline{

    enum Type {Update,Visibility};

    Timeline(Type t) : type(t){}

    static std::unique_ptr<Timeline> copy_with_new_element_id(const Timeline &timelineToCopy){
        std::unique_ptr<Timeline> timeline = std::make_unique<Timeline>(timelineToCopy.type);
        timeline->intervals = timelineToCopy.intervals;
        return timeline;
    }

    bool add_interval(Interval interval);
    bool remove_interval(Interval intervalToRemove);
    void cut(SecondsTS max);
    size_t nb_intervals() const;
    double sum_intervals() const;

    void fill(SecondsTS lenght);
    void clean();

    std::vector<Interval> intervals;
    Type type;

    inline QString to_string() const{
        QString v;
        for(const auto &interval : intervals){
            v = v % interval.to_string();
        }
        return v;
    }

private:

    void merge();

};


}
