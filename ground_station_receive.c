#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <fcntl.h>
#include <mavlink.h>

#include <unistd.h>

#define BUFFER_LENGTH = 2041

int main(int argc, char **argv) {

    uint8_t[BUFFER_LENGTH];
    ssize_t recsize;

    unsigned int temp = 0;
    int sock;

    struct sockaddr_int ground_station_addr;
    socklen_t fromlen = sizeof(ground_station_addr);

    if((sock = socket(AF_INET, SOCK_DRGAM, 0)) == 0) {
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }

    ground_station_addr.sin_family = AF_INET;
    ground_station_addr.s_addr = INADDR_ANY;
    ground_station_addr.sin_port = htons(14550);

    if(bind(sock, (struct sockaddr*)&ground_station_addr, sizeof(struct sockaddr)) < 0) {
        perror("Error Bind Failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    for(;;) {
        recsize = recvfrom(sock, (void *)buf, BUFFER_LENGTH, 0, (struct sockaddr *)&ground_station_addr, &fromlen);
        if(recsize > 0) {
            mavlink_message_t msg;
            mavlink_status_t status;

            printf("Bytes Received: %d\n", (int)recsize);
            for(int i = 0; i < recsize; i++) {
                temp = buf[i];
                if(mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status)) {
                    print("Received packet - SYS: %d, COMP: %d, LEN: %d, MSGID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
                    switch(msg.msgid) {
                        case MAVLINK_MSG_ID_SENSOR_WAVELENGTHS:
                            {
                                mavlink_sensor_wavelengths_t sensor_wavelengths_msg;
                                mavlink_msg_sensor_wavelengths_decode(&msg, &sensor_wavelengths_msg);
                                printf("Wavelength_1: %d, Wavelength_2: %d, Wavelength_3: %d
                                        Wavelength_4: %d, Wavelength 5: %d", sensor_wavelengths_msg)
                            }
                            break;
                        case MAVLINK_MSG_ID_LASER_RANGEFINDER:
                            {
                                mavlink_laser_rangefinder_t laser_rangefinder_msg;
                                mavlink_msg_laser_rangefinder_decode(&msg, &laser_rangefinder_msg);
                            }
                            break;
                        case MAVLINK_MSG_ID_CAMERA_MODE:
                            {
                                mavlink_camera_mode_t camera_mode_msg;
                                mavlink_msg_camera_mode_decode(&msg, &camera_mode_msg);
                            }
                            break;
                    }

                }
            }
        }
    }



    
}

