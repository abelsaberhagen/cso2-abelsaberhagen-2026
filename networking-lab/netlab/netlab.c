#include <stdio.h>
#include "netsim.h"

int msg_status = 0;


void* get_send(void* arg) {
    printf("get_send called\n");
    msg_status = 0;
    char data[5];
    data[1] = 'G'; data[2] = 'E'; data[3] = 'T'; data[4] = *(char *) arg;
    // end of working code
    
    
    data[0] = data[1]^data[2]^data[3]^data[4]; // FIX ME -- add proper checksum computation
    send(5, data);

    return NULL;
}

void ack_send(char arg) {
    char msg[4];
    msg[0] = 'A'; msg[1] = 'C'; msg[2] = 'K'; msg[3] = arg;
    send(4, msg);
}

void recvd(size_t len, void* _data) {
    // FIX ME -- add proper handling of messages
    msg_status = 1;
    char *data = _data;
    ack_send(data[1]);
        
    
    fwrite(data+3,1,len-3,stdout);
    fflush(stdout);
    printf("\n");
}


int main(int argc, char *argv[]) {
    // this code should work without modification
    if (argc != 2) {
        fprintf(stderr, "USAGE: %s n\n    where n is a number between 0 and 3\n", argv[0]);
        return 1;
    }
    
    // FIX ME -- add action if no reply
    char *arg = &argv[1][0];
    void *get_send_func = &get_send((void *)arg);
    while (msg_status == 0) {
        setTimeout(get_send_func, 1000, (void *)arg);
        printf("waiting\n");
    }
    waitForAllTimeoutsAndMessagesThenExit();
}
