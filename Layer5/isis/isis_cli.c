#include <assert.h>

#include "../../tcp_public.h"
#include "isis_cmdcodes.h"
#include "isis_rtr.h"
#include "isis_intf.h"

/**
   Extract node-name from the leaf buffer
   only works for commands with only node-name leaf
 **/
static void
get_node_name(ser_buff_t *tlv_buf, char **name)
{
    tlv_struct_t *iter;
    TLV_LOOP_BEGIN(tlv_buf, iter) {

        if (strncmp(iter->leaf_id, "node-name", strlen("node-name")) == 0) {
            *name = iter->value;
        }
        else {
            assert(0 && "unexpected keyword argument");
        }
        
    } TLV_LOOP_END;
}


static int
isis_config_handler(param_t *param,
                    ser_buff_t *tlv_buf,
                    op_mode enable_or_disable)
{

    int cmdcode = EXTRACT_CMD_CODE(tlv_buf);
    char *name;
    get_node_name(tlv_buf, &name);
    node_t *node = node_get_node_by_name(topo, name);
    assert(node);

    switch(cmdcode) {
    case ISIS_CONFIG_NODE_ENABLE:
        isis_init(node, enable_or_disable);
        break;
    default:
        assert(0 && "unexpected command code");
    }
    
    return 0;
}

static int
isis_show_handler(param_t *param,
                  ser_buff_t *tlv_buf,
                  op_mode enable_or_disable)
{

    int cmdcode = EXTRACT_CMD_CODE(tlv_buf);
    char *name;
    get_node_name(tlv_buf, &name);
    node_t *node = node_get_node_by_name(topo, name);
    assert(node);

    switch(cmdcode) {
    case ISIS_SHOW_NODE:
        isis_show_node(node);
        interface_t *intf;
        ITERATE_NODE_INTERFACES_BEGIN(node, intf) {
            isis_show_intf(intf);
        } ITERATE_NODE_INTERFACES_END(node, intf);
        break;

    default:
        assert(0 && "unexpected command code");
    }
    
    return 0;
}

static int
isis_config_interface_handler(param_t *param,
                              ser_buff_t *tlv_buf,
                              op_mode enable_or_disable)
{
    int cmdcode = EXTRACT_CMD_CODE(tlv_buf);
    char *node_name;
    char *intf_name;
    tlv_struct_t *iter;
    
    TLV_LOOP_BEGIN(tlv_buf, iter) {

        if (strncmp(iter->leaf_id, "node-name", strlen("node-name")) == 0) {
            node_name = iter->value;
        }
        else if (strncmp(iter->leaf_id, "intf-name", strlen("intf-name")) == 0) {
            intf_name = iter->value;
        }
        else {
            assert(0 && "unexpected keyword argument");
        }
        
    } TLV_LOOP_END;
    
    node_t *node = node_get_node_by_name(topo, node_name);
    assert(node);
    
    switch(cmdcode) {
    case ISIS_CONFIG_INTERFACE_ENABLE:
        interface_t *intf = node_get_intf_by_name(node, intf_name);
        if (!intf) {
            printf("No interface on node %s with name %s\n",
                   node_name, intf_name);
            return -1;
        }
        printf("configuring node %s interface %s\n", node_name, intf_name);
        isis_intf_init(intf, enable_or_disable);
        break;
    case ISIS_CONFIG_INTERFACE_ALL_ENABLE:
        for (uint32_t i = 0; i < MAX_INTF_PER_NODE; i++){
            interface_t *intf = node->intf[i];
            if (!intf)
                break;
            isis_intf_init(intf, enable_or_disable);
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
               INVALID, 0, "Config hook isis.");
    libcli_register_param(param, &isis_proto);
    set_param_cmd_code(&isis_proto, ISIS_CONFIG_NODE_ENABLE);

    {
        static param_t isis_proto_intf;
        init_param(&isis_proto_intf, CMD, "interface", 0, 0, INVALID, 0,
                   "Config hook isis interface.");
        libcli_register_param(&isis_proto, &isis_proto_intf);

        {
            static param_t isis_proto_intf_all;
            init_param(&isis_proto_intf_all,
                       CMD,
                       "all",
                       isis_config_interface_handler,
                       0, INVALID, 0,
                       "Config isis interface all.");
            libcli_register_param(&isis_proto_intf, &isis_proto_intf_all);
            set_param_cmd_code(&isis_proto_intf_all,
                               ISIS_CONFIG_INTERFACE_ALL_ENABLE);
        }        
        {
            static param_t isis_proto_intf_name;
            init_param(&isis_proto_intf_name,
                       LEAF,
                       0,
                       isis_config_interface_handler,
                       0, // leaf value validator accepts all values.
                       STRING,
                       "intf-name",
                       "Config isis interface <intf-name>.");
            libcli_register_param(&isis_proto_intf, &isis_proto_intf_name);
            set_param_cmd_code(&isis_proto_intf_name,
                               ISIS_CONFIG_INTERFACE_ENABLE);
        }
    }
}

int isis_show_cli_tree(param_t *param)
{
    // setup protocol CLI hierarchy
    static param_t isis_proto;
    init_param(&isis_proto, CMD, "isis", isis_show_handler, 0,
               INVALID, 0, "Hook for isis protocol.");
    libcli_register_param(param, &isis_proto);
    set_param_cmd_code(&isis_proto, ISIS_SHOW_NODE);
}
