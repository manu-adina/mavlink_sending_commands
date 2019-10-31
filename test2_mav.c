#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <fcntl.h>
#include <mavlink.h>

#include <unistd.h>

#define BUFFER_LENGTH 2041

int main(int argc, char **argv) {

    uint8_t buf[BUFFER_LENGTH];
    ssize_t recsize;
    unsigned int temp = 0;

    struct sockaddr_in locAddr;
    socklen_t fromlen = sizeof(locAddr);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    locAddr.sin_family = AF_INET;
    locAddr.sin_addr.s_addr = INADDR_ANY;
    locAddr.sin_port = htons(14550);

    if(-1 == bind(sock, (struct sockaddr *)&locAddr, sizeof(struct sockaddr))) {
        perror("Error Bind Failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    for(;;) {
        // Receive Message.
        
        memset(buf, 0, BUFFER_LENGTH);
        recsize = recvfrom(sock, (void *)buf, BUFFER_LENGTH, 0, (struct sockaddr *)&locAddr, &fromlen);
        if(recsize > 0) {
            mavlink_message_t msg;
            mavlink_status_t status;

            printf("Bytes Received: %d\nDatagram: ", (int)recsize);
            for(int i = 0; i < recsize; i++) {
                temp = buf[i];
                if(mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status)) {
                    printf("Received packet: SYS %d, COMP: %d, LEN: %d, MSGID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);

                    mavlink_test_message_t received_message;
                    mavlink_msg_test_message_decode(&msg, &received_message);
                    printf("%s\n", received_message.test_string);
                }
            }
        }
        memset(buf, 0, BUFFER_LENGTH);
        sleep(1);
    }
}
