#include <assert.h>

#include "../../tcp_public.h"
#include "isis_cmdcodes.h"
#include "isis_rtr.h"

static int
isis_init(node_t *node)
{
    if (!isis_is_protocol_enable_on_node(node)){
        node->node_nw_prop.isis_node_info = (isis_node_info_t *) malloc(sizeof(isis_node_info_t));
    }
    printf("init: isis enabled is %d\n", isis_is_protocol_enable_on_node(node));
    return 0;
}


static int
isis_de_init(node_t *node)
{
    if (isis_is_protocol_enable_on_node(node)){
        free(node->node_nw_prop.isis_node_info);
        node->node_nw_prop.isis_node_info = NULL;
    }
    printf("de-init: isis enabled is %d\n", isis_is_protocol_enable_on_node(node));
    return 0;
            
}


static int
isis_config_handler(param_t *param,
                    ser_buff_t *tlv_buf,
                    op_mode enable_or_disable)
{

    int cmdcode = EXTRACT_CMD_CODE(tlv_buf);
    tlv_struct_t *iter;
    char *name;

    TLV_LOOP_BEGIN(tlv_buf, iter) {

        if (strncmp(iter->leaf_id, "node-name", strlen("node-name")) == 0) {
            name = iter->value;
        }
        else {
            assert(0 && "unexpected keyword argument");
        }
        
    } TLV_LOOP_END;

    node_t *node = node_get_node_by_name(topo, name);
    assert(node);

    switch(cmdcode) {
    case ISIS_CONFIG_NODE_ENABLE:
        switch(enable_or_disable) {
        case CONFIG_DISABLE:
            isis_de_init(node);
            break;
        case CONFIG_ENABLE:
            isis_init(node);
            break;
        default:
            assert(0 && "unexpected op_mode");
        }
        break;
    default:
        assert(0 && "unexpected command code");
    }
    
    return 0;
}

int isis_config_cli_tree(param_t *param)
{
    // setup protocol CLI hierarchy
    static param_t isis_proto;
    init_param(&isis_proto, CMD, "isis", isis_config_handler, 0,
               INVALID, 0, "Hook for isis protocol.");
    libcli_register_param(param, &isis_proto);
    set_param_cmd_code(&isis_proto, ISIS_CONFIG_NODE_ENABLE);
}

