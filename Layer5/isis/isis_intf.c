#include "../../tcp_public.h"
#include "isis_intf.h"
#include "isis_rtr.h"

bool isis_is_protocol_enable_on_intf(interface_t *intf)
{
    isis_intf_info_t *info = EXTRACT_INTF_INFO(intf);
    if (info) {
        return true;
    }
    return false;
}


int
isis_intf_init(interface_t *intf, op_mode enable_or_disable)
{
    switch(enable_or_disable) {
    case CONFIG_DISABLE:
        if (isis_is_protocol_enable_on_intf(intf)){
            printf("isis intf %s de_init\n", intf->if_name);
            free(intf->intf_nw_props.isis_intf_info);
            intf->intf_nw_props.isis_intf_info = NULL;
        }
        break;
    case CONFIG_ENABLE:
        /*
          constraints on enabling:
          1) if proto is not already enabled at node level, do nothing
          2) if protocol is already enabled on interface, do nothing
          3) else enable it.
         */
        if (!isis_is_protocol_enable_on_node(intf->att_node)) {
            printf("Error: isis not enabled at device level.");
        }
        else if (isis_is_protocol_enable_on_intf(intf)){
            printf("isis already enabled at interface %s. do nothing",
                   intf->if_name);
        }
        else {
            printf("isis intf %s init\n", intf->if_name);
            intf->intf_nw_props.isis_intf_info = (isis_intf_info_t *) malloc(sizeof(isis_intf_info_t));
        }
        break;
    default:
        assert(0 && "unexpected op_mode");
    }
     
    return 0;
}


void
isis_show_intf(interface_t *intf)
{
    if(isis_is_protocol_enable_on_intf(intf)){
        printf("%s: Enabled\n",
               intf->if_name);
    }
}
