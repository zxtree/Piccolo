#include "runtime/core/base/macro.h"
#include "runtime/core/base/log.h"

#include "network.h"

namespace Piccolo {
    void NetNode::init() {
        LOG_DEBUG("net node init");

        //m_kcpcb = ikcp_create(123, (void*)0);
    }

    void NetNode::update() {
        LOG_DEBUG("update");
        
    }
}