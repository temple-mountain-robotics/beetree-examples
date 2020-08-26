//------------------------------------------------------------------------------
// Copyright (C) 2020 The BeeTree Authors - All Rights Reserved
//
// This file is subject to the terms and conditions defined in the
// file 'LICENSE', which is part of this source code package.
//------------------------------------------------------------------------------
#pragma once

#include "helloworld/application.hpp"

namespace helloworld {
bool install(Application& app);
void teardown(Application& app);
}  // namespace helloworld