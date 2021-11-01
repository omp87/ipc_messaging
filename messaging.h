#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <opencv2/opencv.hpp>

#define MAX_SIZE 10

// structure for message queue
typedef struct mesg_buffer {
    long message_type;
    unsigned int data_rows;
    unsigned int data_cols;
    unsigned int data_type;
    unsigned int data_message_size;
    unsigned int message_flags;
} message;
