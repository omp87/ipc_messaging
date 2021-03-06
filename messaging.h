#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#include <opencv2/opencv.hpp>

#define SHARED_IMAGE_BUFFER_SIZE 134217728

// structure for message queue
//first byte of message must be non negative long
typedef struct mesg_buffer {
    long message_type;
    unsigned int data_rows;
    unsigned int data_cols;
    unsigned int data_type;
    unsigned int data_image_size;
    unsigned int data_json_size;
} message;

