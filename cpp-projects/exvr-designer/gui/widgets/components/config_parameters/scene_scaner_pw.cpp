
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

#include "scene_scaner_pw.hpp"

// qt-utility
#include "gui/ex_widgets/ex_pushbutton_w.hpp"
#include "gui/ex_widgets/ex_float_spin_box_w.hpp"
#include "gui/ex_widgets/ex_combo_box_index_w.hpp"
#include "qt_logger.hpp"

// local
#include "gui/ex_widgets/ex_component_w.hpp"
#include "gui/ex_widgets/ex_tab_w.hpp"
#include "gui/ex_widgets/ex_oob_filtering_w.hpp"

using namespace tool;
using namespace tool::ex;
using namespace tool::ui;

struct SceneScanerInitConfigParametersW::Impl{
    ExComponentW kinectManager{"kinect_manager"};
};

SceneScanerInitConfigParametersW::SceneScanerInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}


void SceneScanerInitConfigParametersW::insert_widgets(){
    add_widget(F::gen(L::HB(), {m_p->kinectManager()}, LStretch{false}, LMargins{true}, QFrame::Box));
}

void SceneScanerInitConfigParametersW::init_and_register_widgets(){
    add_input_ui(m_p->kinectManager.init_widget(Component::Type::K2_manager, "Kinect manager component: "));
}




struct SceneScanerConfigParametersW::Impl{

    TransformSubPart tr{"global_transform"};

    // display
    ExCheckBoxW displayClouds{"display_clouds"};
    ExCheckBoxW displayColliders{"display_colliders"};

    // rendering
    ExFloatSpinBoxW sizePoints{"size_points"};
    ExComboBoxIndexW rendering{"rendering"};
    ExSelectColorW tint{"tint"};
    ExCheckBoxW renderCircles{"circles"};
    ExCheckBoxW parabloidFragCones{"cones"};
    ExComboBoxIndexW parabloidGeoDetails{"details"};

    // filtering obb
    ExCheckBoxW filterPointsOutsideOBB{"filter_points_outside_obb"};
    ExCheckBoxW displayFilteringOBB{"display_filtering_obb"};    
    ExTabW<OBBFilteringW> filteringObbTab{"filtering_obb_tab"};

    QTabWidget *tw = nullptr;
};

SceneScanerConfigParametersW::SceneScanerConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}


void SceneScanerConfigParametersW::insert_widgets(){

    add_sub_part_widget(m_p->tr);

    add_widget(m_p->tw = new QTabWidget());
    m_p->tw->addTab(
        F::gen(L::VB(),{
            F::gen(L::HB(),{W::txt("Size points: "),m_p->sizePoints()}, LStretch{true}, LMargins{false},QFrame::NoFrame),
            F::gen(L::HB(),{W::txt("Rendering: "), m_p->rendering()}, LStretch{true}, LMargins{false},QFrame::NoFrame),
            F::gen(L::HB(),{W::txt("Points color tint: "), m_p->tint()}, LStretch{true}, LMargins{false},QFrame::NoFrame),
            m_p->renderCircles(), m_p->parabloidFragCones(),
            F::gen(L::HB(),{W::txt("Parbloid geo details: "), m_p->parabloidGeoDetails()}, LStretch{true}, LMargins{false},QFrame::NoFrame
        )}, LStretch{true}, LMargins{true}, QFrame::NoFrame), "Rendering"
    );

    m_p->tw->addTab(
        F::gen(L::VB(),{
            m_p->displayClouds(), m_p->displayColliders()
        }, LStretch{true}, LMargins{true}, QFrame::NoFrame), "Display"
    );

    m_p->tw->addTab(
        F::gen(L::VB(),{
            F::gen(L::HB(), {m_p->filterPointsOutsideOBB(), m_p->displayFilteringOBB()}, LStretch{true}, LMargins{false},QFrame::NoFrame),
            m_p->filteringObbTab()
        },LStretch{false}, LMargins{true},QFrame::NoFrame), "Filtering oriented bounding box"
    );
}

void SceneScanerConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->tr.init_widget("Global model transform"));
    // rendering
    add_input_ui(m_p->sizePoints.init_widget(MinV<qreal>{0.0001}, V<qreal>{0.0030}, MaxV<qreal>{0.05}, StepV<qreal>{0.0001},4));
    add_input_ui(m_p->rendering.init_widget({"Quad", "ParabloidFrag", "ParabloidGeo"}, 2));
    add_input_ui(m_p->parabloidGeoDetails.init_widget({"None", "Low", "Middle", "Hight"}, 3));
    add_input_ui(m_p->tint.init_widget("Points tint", QColor(255,255,255,0)));
    add_input_ui(m_p->renderCircles.init_widget("Use circles", true));
    add_input_ui(m_p->parabloidFragCones.init_widget("Parabloid frag cones", true));
    // display
    add_input_ui(m_p->displayClouds.init_widget("Display clouds ", true));
    add_input_ui(m_p->displayColliders.init_widget("Display colliders ", false));
    // filtering obb
    add_input_ui(m_p->filterPointsOutsideOBB.init_widget("Remove points outside", false));
    add_input_ui(m_p->displayFilteringOBB.init_widget("Display", false));

    add_input_ui(m_p->filteringObbTab.init_widget("Filtering OBBs", OBBFilteringW::generate_init_any_array(QColor(200,0,0,50)), 10, QTabWidget::TabPosition::North));

//    DsbSettings pos   = {MinV<qreal>{-10000.}, V<qreal>{0.},MaxV<qreal>{10000.}, StepV<qreal>{0.1}, 3};
//    DsbSettings rot   = {MinV<qreal>{-360.}, V<qreal>{0.},MaxV<qreal>{360.}, StepV<qreal>{0.1}, 2};
//    DsbSettings scale = {MinV<qreal>{0}, V<qreal>{1.},MaxV<qreal>{10000.}, StepV<qreal>{0.1}, 3};

//    add_input_ui(m_p->filteringObbTrTab.init_widget("Filtering OBBs",
//        ExTransformationW::generate_init_any_array(
//            "OBB transform",
//            Vector3dSettings{pos,pos,pos},
//            Vector3dSettings{rot,rot,rot},
//            Vector3dSettings{scale,scale,scale},
//            true
//        ),
//        10, QTabWidget::TabPosition::North)
//    );
}


