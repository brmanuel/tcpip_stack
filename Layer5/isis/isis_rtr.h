#ifndef __ISIS_RTR__
#define __ISIS_RTR__



typedef struct isis_node_info_ {

} isis_node_info_t;


#define EXTRACT_NODE_INFO(node_t) \
    ((isis_node_info_t *) (node_t->node_nw_prop.isis_node_info))


bool isis_is_protocol_enable_on_node(node_t *node);

#endif
