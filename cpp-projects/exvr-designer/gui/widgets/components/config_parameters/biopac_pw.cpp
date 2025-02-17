
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

#include "biopac_pw.hpp"

// qt-utility
#include "gui/ex_widgets/ex_combo_box_index_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_spin_box_w.hpp"
#include "gui/ex_widgets/ex_line_edit_w.hpp"

// local
#include "gui/ex_widgets/ex_resource_w.hpp"
#include "gui/ex_widgets/ex_component_w.hpp"

using namespace tool::ex;

struct BiopacInitConfigParametersW::Impl{

    ExCheckBoxW debugBypassDevice{"debug_bypass"};
    ExResourceW debugLog{"debug_log_file"};
    ExComboBoxIndexW device{"device_type"};
    ExComboBoxIndexW connection{"device_connection"};
    ExComboBoxIndexW readIoMode{"read_digital_mode"};
    QLabel *info = nullptr;

    using CB = ExCheckBoxW;
    std::array<ExCheckBoxW, 16> channels{
        CB{"channel0"}, CB{"channel1"}, CB{"channel2"}, CB{"channel3"},
        CB{"channel4"}, CB{"channel5"}, CB{"channel6"}, CB{"channel7"},
        CB{"channel8"}, CB{"channel9"}, CB{"channel10"},CB{"channel11"},
        CB{"channel12"},CB{"channel13"},CB{"channel14"},CB{"channel15"},
    };

    using LE = ExLineEditW;
    std::array<ExLineEditW, 16> channelsName{
        LE{"channel0_name"}, LE{"channel1_name"}, LE{"channel2_name"}, LE{"channel3_name"},
        LE{"channel4_name"}, LE{"channel5_name"}, LE{"channel6_name"}, LE{"channel7_name"},
        LE{"channel8_name"}, LE{"channel9_name"}, LE{"channel10_name"},LE{"channel11_name"},
        LE{"channel12_name"},LE{"channel13_name"},LE{"channel14_name"},LE{"channel15_name"},
    };

    std::array<ExLineEditW, 16> channelsPresetUid{
        LE{"channel0_preset_uid"}, LE{"channel1_preset_uid"}, LE{"channel2_preset_uid"}, LE{"channel3_preset_uid"},
        LE{"channel4_preset_uid"}, LE{"channel5_preset_uid"}, LE{"channel6_preset_uid"}, LE{"channel7_preset_uid"},
        LE{"channel8_preset_uid"}, LE{"channel9_preset_uid"}, LE{"channel10_preset_uid"},LE{"channel11_preset_uid"},
        LE{"channel12_preset_uid"},LE{"channel13_preset_uid"},LE{"channel14_preset_uid"},LE{"channel15_preset_uid"},
    };

    static constexpr std::array samplingRates = {
        10, 25, 50, 100, 200, 250, 500, 1000, 2000, 2500, 5000, 10000, 20000, 25000
    };

    std::vector<QWidget*> channelsParentWidgets;
    ExLineEditW serialNumber{"serial"};
    ExComboBoxIndexW samplingRate{"sampling_rate_id"};
    ExSpinBoxW samplesPerCall{"nb_samples_per_call"};
//    ExSpinBoxW maxNbSecondsToSave{"max_nb_seconds_to_save"};
    ExResourceW presetFile{"preset_xml_file"};
    ExComponentW logger{"logger"};
//    ExSpinBoxW writeEveryNbLines{"write_every_nb_lines"};
};

BiopacInitConfigParametersW::BiopacInitConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){}

void BiopacInitConfigParametersW::insert_widgets(){

    add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Device:"), m_p->device(), ui::W::txt("Connection:"),m_p->connection(), ui::W::txt("Serial MP150:"), m_p->serialNumber()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Sampling rate (Hz):"), m_p->samplingRate(), ui::W::txt("Nb samples per call per channel:"), m_p->samplesPerCall()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Read digital mode:"), m_p->readIoMode(),
//                                        ui::W::txt("Buffer capacity (seconds):"), m_p->maxNbSecondsToSave()
    }, LStretch{true}, LMargins{false},QFrame::NoFrame));
//    add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Write when reach number of lines (x1000): "), m_p->writeEveryNbLines()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(),{m_p->logger()}, LStretch{false}, LMargins{false},QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(),{m_p->presetFile()}, LStretch{false}, LMargins{false},QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(),{m_p->info = new QLabel("...")}, LStretch{true}, LMargins{false},QFrame::NoFrame));

    QScrollArea *area = new QScrollArea();
    area->setWidgetResizable(true);

    QWidget *w = new QWidget();
    auto scrollL = ui::L::VB();
    w->setLayout(scrollL);
    area->setWidget(w);
    add_widget(area);
    for(size_t ii = 0; ii < 16; ++ii){

        auto pw = ui::F::gen(ui::L::HB(),{
             m_p->channels[ii](),
             ui::W::txt("Name:") ,m_p->channelsName[ii](),
             ui::W::txt("Preset uid: "), m_p->channelsPresetUid[ii]()
         }, LStretch{true}, LMargins{false},QFrame::NoFrame);

        scrollL->addWidget(pw);
        m_p->channelsParentWidgets.emplace_back(pw);
    }

    add_widget(ui::F::gen(ui::L::VB(),{m_p->debugBypassDevice(), m_p->debugLog()}, LStretch{false}, LMargins{true},QFrame::Box));
}

void BiopacInitConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_p->device.init_widget({"MP36","MP150","MP160"}));
    add_input_ui(m_p->connection.init_widget({"USB","UDP"}));
    add_input_ui(m_p->serialNumber.init_widget("AUTO"));
    add_input_ui(m_p->readIoMode.init_widget({"DISABLED", "LOW_BYTES", "HIGHT_BYTES"},0));
    add_input_ui(m_p->presetFile.init_widget(Resource::Type::Text, "Preset xml file (optional): "));
    add_input_ui(m_p->logger.init_widget(Component::Type::Logger, "Associated logger (optional): "));
    m_p->logger.w->setToolTip("If valid, data will be written on the logger at each end of routine.");

    add_input_ui(m_p->samplesPerCall.init_widget(MinV<int>{5}, V<int>{100}, MaxV<int>{100000}, StepV<int>{1}));
//    add_input_ui(m_p->writeEveryNbLines.init_widget(MinV<int>{1}, V<int>{10}, MaxV<int>{1000}, StepV<int>{1}));

    add_input_ui(m_p->samplingRate.init_widget(
        {"10","25","50","100","200","250 (EEG, EOG)", "500 (RESPIRATION)", "1K (ECG, EMG)", "2K", "2.5K", "5K", "10K", "20K", "25K"},7
    ));

//    add_input_ui(m_p->maxNbSecondsToSave.init_widget(MinV<int>{100}, V<int>{1000}, MaxV<int>{10000}, StepV<int>{1}));

    for(size_t ii = 0; ii < 16; ++ii){
        auto idStr = QString::number(ii);
        add_input_ui(m_p->channels[ii].init_widget(QSL("Enable channel ") % QString::number(ii+1), false));
        add_input_ui(m_p->channelsName[ii].init_widget(QSL("channel") % QString::number(ii+1)));
        add_input_ui(m_p->channelsPresetUid[ii].init_widget(""));
    }
    add_input_ui(m_p->debugBypassDevice.init_widget("Enable it for testing the experiment without the device", false));
    add_input_ui(m_p->debugLog.init_widget(Resource::Type::Text, "Use log file as debug input"));
}

void BiopacInitConfigParametersW::create_connections(){

    connect(&m_p->device, &ExComboBoxIndexW::ui_change_signal, this, [&]{
        late_update_ui();
    });
    connect(&m_p->samplesPerCall, &ExSpinBoxW::ui_change_signal, this, [&]{
        late_update_ui();
    });
    connect(&m_p->samplingRate, &ExComboBoxIndexW::ui_change_signal, this, [&]{
        late_update_ui();
    });
    for(size_t ii = 0; ii < m_p->channels.size(); ++ii){
        connect(&m_p->channels[ii], &ExCheckBoxW::ui_change_signal, this, [&]{
            late_update_ui();
        });
    }
}

void BiopacInitConfigParametersW::late_update_ui(){

    int id = m_p->device.w->currentIndex();
    bool extraChannels = id > 0;    
    for(size_t ii = 4; ii < m_p->channels.size(); ++ii){
        m_p->channelsParentWidgets[ii]->setEnabled(extraChannels);
        if(!extraChannels){
            m_p->channels[ii].blockSignals(true);
            m_p->channels[ii].w->setChecked(false);
            m_p->channels[ii].blockSignals(false);
        }
    }

    size_t nbChannelsEnabled = 0;
    for(size_t ii = 0; ii < m_p->channels.size(); ++ii){
        if(m_p->channels[ii].w->isChecked()){
            ++nbChannelsEnabled;
        }
    }

    int nbSamplesPerCall = m_p->samplesPerCall.w->value();
    int samplingRate = m_p->samplingRates[m_p->samplingRate.w->currentIndex()];
    size_t numDataPointsPerSample = nbChannelsEnabled * nbSamplesPerCall;
    double nbAPICallPerSecond = 1.0*samplingRate/nbSamplesPerCall;
    double durationAPICall = 1000.0/nbAPICallPerSecond;

    QStringView channelStr = nbChannelsEnabled > 0 ? QSL("channels") : QSL("channel");
    QStringView valuesStr = numDataPointsPerSample > 0 ? QSL("values") : QSL("value");
    m_p->info->setText(QSL("<b>") % QString::number(nbChannelsEnabled) % QSL("</b> ") % channelStr % QSL(", for a total of <b>") %
        QString::number(numDataPointsPerSample) % QSL("</b> ") % valuesStr % QSL(" per call with <b>") %
        QString::number(nbAPICallPerSecond) % QSL("</b> calls/s to the API with a duration of <b>") % QString::number(durationAPICall) % QSL("</b> ms per call."));
}
