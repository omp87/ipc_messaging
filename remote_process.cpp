#include "messaging.h"

int main()
{
    key_t key_ctl_host_remote;
    key_ctl_host_remote = ftok(".", 60);
    int msgid_ctl_host_remote;
    message message_ctl_host_remote;
    printf("%xd\n", key_ctl_host_remote);

    key_t key_data_host_remote;
    key_data_host_remote = ftok(".", 61);
    int msgid_data_host_remote;

    key_t key_ctl_remote_host;
    key_ctl_remote_host = ftok(".", 62);
    int msgid_ctl_remote_host;
    message message_ctl_remote_host;

    key_t key_data_remote_host;
    key_data_remote_host = ftok(".", 63);
    int msgid_data_remote_host;

    printf("size of message %ld\n", sizeof(message));
    msgid_ctl_host_remote = msgget(key_ctl_host_remote, 0666 | IPC_CREAT);
    msgrcv(msgid_ctl_host_remote, &message_ctl_host_remote, sizeof(message), 1, 0);
    printf("Data Received is : %d \n", message_ctl_host_remote.data_message_size);

    msgid_data_host_remote = msgget(key_data_host_remote, 0666 | IPC_CREAT);
    cv::Mat* in_image = new cv::Mat(message_ctl_host_remote.data_rows, message_ctl_host_remote.data_cols, message_ctl_host_remote.data_type);
    uint8_t* data_message = (uint8_t*) malloc(message_ctl_host_remote.data_message_size);
    msgrcv(msgid_data_host_remote, (void*) data_message, message_ctl_host_remote.data_message_size, 1, 0);
    memcpy(&(in_image->data), &(data_message[sizeof(long)]), message_ctl_host_remote.data_message_size-sizeof(long));
    printf("Message Received\n");


    // to destroy the message queue
    msgctl(msgid_ctl_host_remote, IPC_RMID, NULL);
    msgctl(msgid_data_host_remote, IPC_RMID, NULL);
    return 0;
}