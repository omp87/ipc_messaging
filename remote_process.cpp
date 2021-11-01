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
    int shmid_data_host_remote = shmget(key_data_host_remote,SHARED_IMAGE_BUFFER_SIZE,0666|IPC_CREAT);
    uint8_t *image_buffer = (uint8_t*) shmat(shmid_data_host_remote,(void*)0,0);

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

    cv::Mat* in_image = new cv::Mat(message_ctl_host_remote.data_rows, message_ctl_host_remote.data_cols, message_ctl_host_remote.data_type);
    memcpy(&(in_image->data), image_buffer, message_ctl_host_remote.data_message_size);
    printf("Message Received\n");


    // to destroy the message queue
    msgctl(msgid_ctl_host_remote, IPC_RMID, NULL);
    shmdt(image_buffer);
    shmctl(shmid_data_host_remote,IPC_RMID,NULL);
   return 0;
}