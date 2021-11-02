#include "messaging.h"

int main()
{
    key_t key_ctl_host_remote;
    key_ctl_host_remote = ftok(".", 60);
    int msgid_ctl_host_remote = msgget(key_ctl_host_remote, 0666 | IPC_CREAT);
    message message_ctl_host_remote;
    printf("%xd\n", key_ctl_host_remote);

    key_t key_data_host_remote;
    key_data_host_remote = ftok(".", 61);
    int shmid_data_host_remote = shmget(key_data_host_remote,SHARED_IMAGE_BUFFER_SIZE,0666|IPC_CREAT);
    uint8_t *image_buffer = (uint8_t*) shmat(shmid_data_host_remote,(void*)0,0);
    printf("shared memory %d\n", shmid_data_host_remote);

    key_t key_ctl_remote_host;
    key_ctl_remote_host = ftok(".", 62);
    int msgid_ctl_remote_host = msgget(key_ctl_remote_host, 0666 | IPC_CREAT);;
    message message_ctl_remote_host;

    printf("size of message %ld\n", sizeof(message));
    msgrcv(msgid_ctl_host_remote, &message_ctl_host_remote,sizeof(message) , 1, 0);
    printf("Data Received is : %d \n", message_ctl_host_remote.data_image_size);

    cv::Mat* in_image = new cv::Mat(message_ctl_host_remote.data_rows, message_ctl_host_remote.data_cols, message_ctl_host_remote.data_type);
    memcpy((uint8_t*) &(in_image->data[0]), image_buffer, message_ctl_host_remote.data_image_size);

    char* json_string_char = new char[message_ctl_host_remote.data_json_size];
    memcpy(&(json_string_char[0]), &(image_buffer[message_ctl_host_remote.data_image_size]), message_ctl_host_remote.data_json_size);
    std::string json_string(json_string_char);
    std::cout << json_string << std::endl;


    cv::inRange(*in_image, cv::Scalar(0,0,0), cv::Scalar(255,255,255), *in_image);
    //Manipulate Image
    cv::imshow("image", *in_image);
    if(cv::waitKey(-1))
    { }


    //Copy image back into shared memory
    size_t in_image_size = in_image->total() * in_image->elemSize();
    memcpy(&(image_buffer[0]), &(in_image->data[0]), in_image_size);

    //Send Response back to host
    message_ctl_remote_host.message_type = 1;
    message_ctl_remote_host.data_rows = in_image->rows;
    message_ctl_remote_host.data_cols = in_image->cols;
    message_ctl_remote_host.data_type = in_image->type();
    message_ctl_remote_host.data_image_size = in_image_size;
    message_ctl_remote_host.data_json_size = 0;
    int msg_send_result = msgsnd(msgid_ctl_remote_host, &message_ctl_host_remote, sizeof(message), 0);


    delete in_image;
    delete json_string_char;

    // to destroy the message queue
    msgctl(msgid_ctl_host_remote, IPC_RMID, NULL);
    shmdt(image_buffer);
    shmctl(shmid_data_host_remote,IPC_RMID,NULL);
   return 0;
}