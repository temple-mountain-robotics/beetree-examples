//------------------------------------------------------------------------------
// Copyright (C) 2020 The BeeTree Authors - All Rights Reserved
//
// This file is subject to the terms and conditions defined in the
// file 'LICENSE', which is part of this source code package.
//------------------------------------------------------------------------------

#include "helloworld/application.hpp"

namespace helloworld {

void Application::sense()
{   
    // synchronize the d_ins with the platform
    platform.discrete_inputs_controller().sync(); 

    // update the model from the refreshed registry
    helloworld_model.user_btn_depressed 
        = platform.discrete_inputs().discrete_input_bank()[DiscreteInputValueKey::USER_BTN_DEPRESSED];
}

void Application::process() { bte::TreeX::loop(this->tree, 10_ms); }

void Application::actuate() { 
    // signal to the platform to flush the discrete output registry to the hardware
    platform.discrete_outputs().discrete_output_bank()[DiscreteOutputValueKey::LED] = debug_svc.model().is_led_set;
    platform.discrete_outputs().discrete_output_bank()[DiscreteOutputValueKey::DEBUG_PIN] = debug_svc.model().is_debug_pin_set;
    platform.discrete_outputs_controller().flush(); 
}

void Application::on_idle(bool first_time)
{
    // advance the registry trace only once per scan
    if (first_time)
    {
        registry_trace.tick();
    }
}
}  // namespace helloworld