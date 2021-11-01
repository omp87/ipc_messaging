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
    int msgid_data_host_remote;
 
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

    //first byte of message must be non negative long
    uint8_t* data_message = (uint8_t*) malloc(in_image_size + sizeof(long));
    long message_type = 1;
    memcpy(data_message, &message_type, sizeof(long));
    memcpy(&(data_message[sizeof(long)]), (uint8_t*) &(in_image.data), 10);
    memcpy(data_message + sizeof(long) , (uint8_t*) in_image.data, in_image_size);
    msgid_data_host_remote = msgget(key_data_host_remote, 0666 | IPC_CREAT);
    printf("Write Image Data");
    int msg_send_result = msgsnd(msgid_data_host_remote, data_message, in_image_size + sizeof(long), 0);
    printf("Mesage Send value%d\n", msg_send_result);

    printf("size of message %ld\n", sizeof(message));
    msgid_ctl_host_remote = msgget(key_ctl_host_remote, 0666 | IPC_CREAT);
    message_ctl_host_remote.message_flags = 1;
    message_ctl_host_remote.data_message_size = in_image_size + sizeof(long);
    msg_send_result = msgsnd(msgid_ctl_host_remote, &message_ctl_host_remote, sizeof(message), 0);
    printf("Mesage Send value%d\n", msg_send_result);
    
    free(data_message);



    // display the message
    printf("Data send is : %d \n", message_ctl_host_remote.data_message_size);

    return 0;
}