#include "../../tcp_public.h"
#include "isis_pkt.h"
#include "isis_constants.h"

bool
isis_pkt_trap_rule(char *pkt, size_t pkt_size)
{
    ethernet_hdr_t *eth_pkt = (ethernet_hdr_t *) pkt;
    return eth_pkt->type == ETHERNET_TYPE_ISIS;
}


void
isis_pkt_receive(void *arg, size_t arg_size)
{
    uint16_t *isis_pkt_type = arg;
    switch (*isis_pkt_type) {
    case ETHERNET_PAYLOAD_TYPE_ISIS_HELLO:
        break;
    case ETHERNET_PAYLOAD_TYPE_ISIS_LSP:
        break;
    default:
        break;
    }
}
