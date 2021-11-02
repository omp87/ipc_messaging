#include "messaging.h"
  
int main(int argc, char* argv[])
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
    printf("shared memory %d\n", shmid_data_host_remote);

 
    key_t key_ctl_remote_host;
    key_ctl_remote_host = ftok(".", 62);
    int msgid_ctl_remote_host;
    message message_ctl_remote_host;

    key_t key_data_remote_host;
    key_data_remote_host = ftok(".", 63);
    int msgid_data_remote_host;

    cv::Mat in_image = cv::imread("../images/intel_part.png");
    size_t in_image_size = in_image.total() * in_image.elemSize();
    message_ctl_host_remote.message_type = 1;
    message_ctl_host_remote.data_rows = in_image.rows;
    message_ctl_host_remote.data_cols = in_image.cols;
    message_ctl_host_remote.data_type = in_image.type();

    memcpy(image_buffer, (uint8_t*) in_image.data, in_image_size);
    printf("Write Image Data");

    printf("size of message %ld\n", sizeof(message));
    msgid_ctl_host_remote = msgget(key_ctl_host_remote, 0666 | IPC_CREAT);
    message_ctl_host_remote.message_flags = 1;
    message_ctl_host_remote.data_message_size = in_image_size;
    int msg_send_result = msgsnd(msgid_ctl_host_remote, &message_ctl_host_remote, sizeof(message), 0);
    printf("Mesage Send value%d\n", msg_send_result);

    // display the message
    printf("Data send is : %d \n", message_ctl_host_remote.data_message_size);
    shmdt(image_buffer);


    return 0;
}