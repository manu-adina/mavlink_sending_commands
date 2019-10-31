#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <mavlink.h>

#include <unistd.h>

#define BUFFER_LENGTH 2041

int main(int argc, char* argv[]) {

    char target_ip[] = "127.0.0.1";

    // PF_IENT is equivalent to AF_INET
    int sock = socket(AF_INET, SOCK_DGRAM, 0); // or can just use '0' for the last value.
    struct sockaddr_in gcAddr;

    uint8_t buf[BUFFER_LENGTH];
    int bytes_sent;
    mavlink_message_t msg;
    uint16_t len;

    int i = 0;
    unsigned int temp = 0;

    if(fcntl(sock, F_SETFL, O_NONBLOCK | O_ASYNC) < 0) {
        perror("Failed to set to nonblocking");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // For sending to socket.
    memset(&gcAddr, 0, sizeof(gcAddr));
    gcAddr.sin_family = AF_INET;
    gcAddr.sin_addr.s_addr = inet_addr(target_ip);
    gcAddr.sin_port = htons(14550);

    for(;;) {
        // Sending something I guess.
        mavlink_msg_test_message_pack(1, 200, &msg, "hello world!");
        len = mavlink_msg_to_send_buffer(buf, &msg);
        bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr *)&gcAddr, sizeof(struct sockaddr_in));

        printf("Sending messages\n");
        
        memset(buf, 0, BUFFER_LENGTH);
        sleep(1);
    }

    return 0;
}
