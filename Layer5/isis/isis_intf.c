#include "../../tcp_public.h"
#include "isis_intf.h"

bool isis_is_protocol_enable_on_intf(interface_t *intf)
{
    isis_intf_info_t *info = EXTRACT_INTF_INFO(intf);
    if (info) {
        return true;
    }
    return false;
}
