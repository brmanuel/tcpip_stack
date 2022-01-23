#ifndef __ISIS_INTF__
#define __ISIS_INTF__


typedef struct isis_intf_info_ {

} isis_intf_info_t;


#define EXTRACT_INTF_INFO(intf_t) \
    (isis_intf_info_t *) intf_t->intf_nw_props.isis_intf_info


bool isis_is_protocol_enable_on_intf(interface_t *intf);


#endif
