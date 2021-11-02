#include "messaging.h"
  
int main(int argc, char* argv[])
{
    //create IPC structures for communicating between processes
    key_t key_ctl_host_remote;
    key_ctl_host_remote = ftok(".", 60);
    int msgid_ctl_host_remote = msgget(key_ctl_host_remote, 0666 | IPC_CREAT);
    message message_ctl_host_remote;

    key_t key_data_host_remote;
    key_data_host_remote = ftok(".", 61);
    int shmid_data_host_remote = shmget(key_data_host_remote,SHARED_IMAGE_BUFFER_SIZE,0666|IPC_CREAT);
    uint8_t *image_buffer = (uint8_t*) shmat(shmid_data_host_remote,(void*)0,0);

    key_t key_ctl_remote_host;
    key_ctl_remote_host = ftok(".", 62);
    int msgid_ctl_remote_host = msgget(key_ctl_remote_host, 0666 | IPC_CREAT);
    message message_ctl_remote_host;

    //Read image from file and obtain image information to calculate size
    cv::Mat in_image = cv::imread("../images/intel_part.png");
    size_t in_image_size = in_image.total() * in_image.elemSize();
    message_ctl_host_remote.message_type = 1;
    message_ctl_host_remote.data_rows = in_image.rows;
    message_ctl_host_remote.data_cols = in_image.cols;
    message_ctl_host_remote.data_type = in_image.type();
    message_ctl_host_remote.data_image_size = in_image_size;

    //Copy image data into shared memory
    memcpy((uint8_t*) &(image_buffer[0]), (uint8_t*) &(in_image.data[0]), in_image_size);

    std::string* json_string = new std::string("{\"a\" : 5}");
    message_ctl_host_remote.data_json_size = json_string->size();
    message_ctl_host_remote.data_json_size = 0; //debug
    
    //image size corresponds with first byte after the image
    memcpy(&(image_buffer[in_image_size]), json_string->c_str(), json_string->size());
    int msg_send_result = msgsnd(msgid_ctl_host_remote, &message_ctl_host_remote, sizeof(message), 0);
    if(msg_send_result < 0)
    {}

    int msg_recv_result = msgrcv(msgid_ctl_remote_host, &message_ctl_remote_host, sizeof(message), 1, 0);
    if(msg_recv_result < 0)
    {}

    //Grab data back from shared memory
    cv::Mat* out_image = new cv::Mat(message_ctl_remote_host.data_rows, message_ctl_remote_host.data_cols, message_ctl_remote_host.data_type);
    memcpy(&(out_image->data[0]), &(image_buffer[0]), message_ctl_remote_host.data_image_size);

    //Free allocated memory
    delete out_image;
    delete json_string;

    //Free shared IPC components
    shmdt(image_buffer);
    msgctl(msgid_ctl_remote_host, IPC_RMID, NULL);

    return 0;
}