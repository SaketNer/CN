#include "impl.h"
#include <stdio.h>

unsigned char* serialize(struct packet* pkt) {
    /* Todo: Please write the code here*/
}

struct packet *deserialize(unsigned char* buffer) {
    /* Todo: Please write the code here*/
}

void router_init(struct distance_table *my_dt, int my_node, int *my_link_costs, int num_nodes)
{
    /* Todo: Please write the code here*/
}

void router_update(struct distance_table *my_dt, int my_node, unsigned char* packet_buffer, int *my_link_costs, int num_nodes)
{
    /* Todo: Please write the code here*/
}
