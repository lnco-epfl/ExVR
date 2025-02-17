
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

// qt-utility
#include "gui/ex_widgets/ex_combo_box_text_w.hpp"

// local
#include "gui/widgets/connections/data_models/connectors/connector_node_data_model.hpp"

namespace tool::ex {


class ExKeyboardButtonTrigger : public ExItemW<QWidget>{

public:

    ExKeyboardButtonTrigger(QString name ="");
    ExKeyboardButtonTrigger *init_widget(bool enabled = true);

    void update_from_arg(const Arg &arg) override;
    Arg convert_to_arg() const override;

    ExComboBoxTextW timeMode = {"time_mode"};
    ExComboBoxTextW keys = {"key"};
};

class ExKeyboardButtonTriggerEmbeddedW : public NodeContainerW<ExKeyboardButtonTrigger>{
    Q_OBJECT
public:
    void initialize() override;
    QString button();
};
class KeyboardButtonEmbeddedW : public NodeContainerW<ExComboBoxTextW>{
Q_OBJECT
public:
    void initialize() override;
};

class CheckKeyboardButtonNodeDataModel : public TypedConnectorDataModel<Connector::Type::Check_keyboard_button, ExKeyboardButtonTriggerEmbeddedW>{
Q_OBJECT
public slots:
    void compute() override;
public:
    void init_ports_caption() override;
};

class FilterKeyboardButtonNodeDataModel : public TypedConnectorDataModel<Connector::Type::Filter_keyboard_button, KeyboardButtonEmbeddedW>{
    Q_OBJECT
public slots:
    void compute() override;
public:
    void init_ports_caption() override;
};
}


