
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

// base
#include "utility/unordered_map.hpp"

// qt-utility
#include "data/id_key.hpp"
#include "qt_str.hpp"

// local
#include "component.hpp"
#include "states.hpp"

namespace tool::ex {

struct Connection{

    enum class Type{
        Component,Connector
    };

    Connection() = delete;
    Connection(ConnectionKey id) : m_key(IdKey::Type::Connection, id.v){}
    Connection(const Connection &) = delete;
    Connection& operator=(const Connection&) = delete;

    constexpr auto key() const noexcept -> int{ return m_key();}
    constexpr auto c_key() const noexcept -> ConnectionKey{return ConnectionKey{key()};}

    static auto copy_with_new_element_id(const Connection &connectionToCopy, umap<int,int> keysMapping) -> std::unique_ptr<Connection>{

        std::unique_ptr<Connection> connection = std::make_unique<Connection>(ConnectionKey{-1});

        if(connectionToCopy.startType == Type::Component){
            connection->startKey = connectionToCopy.startKey;
        }else{
            connection->startKey = keysMapping[connectionToCopy.startKey];
        }

        if(connectionToCopy.endType == Type::Component){
            connection->endKey = connectionToCopy.endKey;
        }else{
            connection->endKey = keysMapping[connectionToCopy.endKey];
        }

        connection->startType       = connectionToCopy.startType;
        connection->endType         = connectionToCopy.endType;
        connection->startIndex      = connectionToCopy.startIndex;
        connection->endIndex        = connectionToCopy.endIndex;
        connection->slot            = connectionToCopy.slot;
        connection->signal          = connectionToCopy.signal;
        connection->startDataType   = connectionToCopy.startDataType;
        connection->endDataType     = connectionToCopy.endDataType;

        return connection;
    }

    inline auto to_string() const -> QString{
        return
            QSL("Connection(") % QString::number(key()) %
            QSL("|signal:") % signal % QSL("|startDataType:") % startDataType %
            QSL("|endDataType:")  % endDataType % QSL("|slot:") % slot % QSL(")");
    }


    Type startType;
    Type endType;

    int startKey;
    int endKey;

    int startIndex;
    int endIndex;

    QString slot;
    QString signal;

    QString startDataType;
    QString endDataType;

    bool selected = false;

private:
    IdKey m_key;
};

static auto operator<(const std::unique_ptr<Connection> &l, const std::unique_ptr<Connection> &r) -> bool{
    if(l->key() == r->key()){
        return false;
    }
    if(
        (l->startKey == r->startKey) &&
        (l->endKey == r->endKey) &&
        (l->startIndex == r->startIndex) &&
        (l->endIndex == r->endIndex)){
        return false;
    }
    return true;
}

[[maybe_unused]] static auto operator==(const std::unique_ptr<Connection> &l, const std::unique_ptr<Connection> &r) -> bool{
    return !(l < r) && !(r < l);
}
}








