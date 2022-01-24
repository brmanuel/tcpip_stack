#include "../../tcp_public.h"
#include "isis_rtr.h"

bool isis_is_protocol_enable_on_node(node_t *node)
{
    isis_node_info_t *info = EXTRACT_NODE_INFO(node);
    if (info) {
        return true;
    }
    return false;
}


    
int
isis_init(node_t *node, op_mode enable_or_disable)
{
    
    switch(enable_or_disable) {
    case CONFIG_DISABLE:
        if (isis_is_protocol_enable_on_node(node)){
            free(node->node_nw_prop.isis_node_info);
            node->node_nw_prop.isis_node_info = NULL;
        }
        break;
    case CONFIG_ENABLE:
        if (!isis_is_protocol_enable_on_node(node)){
            node->node_nw_prop.isis_node_info = (isis_node_info_t *) malloc(sizeof(isis_node_info_t));
        }
        break;
    default:
        assert(0 && "unexpected op_mode");
    }
    
    return 0;
}


void
isis_show_node(node_t *node)
{
    printf("Protocol ISIS: %s\n",
           isis_is_protocol_enable_on_node(node) ? "Enabled" : "Disable");
}
