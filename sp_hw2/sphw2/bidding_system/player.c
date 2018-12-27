/*
./player [host_id] [player_index] [random_key]
host_id -> host
player_index -> {'A', 'B', 'C', 'D'}  != id
random_key -> to verify a response really from that player 



host1.FIFO ←−−read host1 −−−write → host1_A.FIFO ←−−read player_A −−−write→ host1.FIFO 

response to host:
[player_index] [random_key] [money]\n 

*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>


int
main(int argc, char **argv)
{



    exit(0);
}








