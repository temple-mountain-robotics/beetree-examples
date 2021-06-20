//------------------------------------------------------------------------------
// Copyright (C) 2020 The BeeTree Authors - All Rights Reserved
//
// This file is subject to the terms and conditions defined in the
// file 'LICENSE', which is part of this source code package.
//------------------------------------------------------------------------------
#include "helloworld/install.hpp"
#include "beetree/nodes/blackboard.hpp"
#include "helloworld/helloworld.tree"  // for root node

namespace helloworld {

bool install(Application& app)
{
    // configure our trace in ordre to view log statements on beeview and
    // observe the tree
    app.trace.configure(app.platform.vcp(), bte::TraceVerbosity::Debug);

    // configure the tree's root
    app.tree.configure_root(helloworld::tree::root);
    // enable tracing with 0 as the trace id.
    // The 0 id signifies to beeview that this is the main tree.
    app.tree.configure_trace(0, true);

    // install services and models to the black board for all nodes to use
    bte::blackboard::install_writeable<bte::IDebugService>(app.debug_svc);
    bte::blackboard::install_readonly<HelloworldModel>(app.helloworld_model);

    return true;  // successful app installation
}

void teardown(Application& app)
{
    // do nothing
};
}  // namespace helloworld