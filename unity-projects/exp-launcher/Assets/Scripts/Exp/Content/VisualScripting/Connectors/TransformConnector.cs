﻿
/***********************************************************************************
** exvr-exp                                                                       **
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

namespace Ex {

    public class TransformConnector : ExConnector {

        private TransformValue uiValue = null;
        private TransformValue value = new TransformValue();

        protected override bool initialize() {
     
            uiValue = m_config.get_transform_value(valueStr);

            add_signals(1);
            add_slot(0, (arg) => { base_slot1(arg); });
            return true;
        }

        protected override void slot1(object inputValue) {

            if (inputValue != null) {
                value = (TransformValue)inputValue;
            }
            send_output();
        }

        protected override void update_from_gui() {
            uiValue = m_config.get_transform_value(valueStr);
            pre_start_routine();
        }

        protected override void pre_start_routine() {
            if (inputGO.Count == 0) {

                value.position = uiValue.position;
                value.rotation = uiValue.rotation;
                value.scale    = uiValue.scale;

                send_output();
            }
        }

        void send_output() {
            invoke_signal(0, value);
            send_connector_infos_to_gui(Converter.to_string(value));            
        }
    }
}