#include <iostream>
#include "ai_detection.h"
#include <modal_pipe.h>
#include <unistd.h>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

using namespace mavsdk;
using namespace std;

static const string kProcessName = "voxl-tracking";

static void tflite_server_connect_cb(int ch, void *context)
{
    printf("Connected to tflite server\n");
}

static void tflite_server_disconnect_cb(int ch, void *context)
{
    fprintf(stderr, "Disonnected from tflite server\n");
}

static void tflite_server_cb(int ch, char *data, int bytes, void *context)
{
    for (int off = 0; off + (int)sizeof(ai_detection_t) <= bytes; off += sizeof(ai_detection_t)) {
        ai_detection_t *detection = reinterpret_cast<ai_detection_t *>(data + off);
        printf("class confidence: %f\n", detection->class_confidence);
    }
}

void setup_pipe() {
    int ch = pipe_client_get_next_available_channel();
    const string &input_pipe = "/run/mpa/tflite_data";

    main_running = 1;

    pipe_client_set_connect_cb(ch, tflite_server_connect_cb, nullptr);
    pipe_client_set_disconnect_cb(ch, tflite_server_disconnect_cb, nullptr);
    pipe_client_set_simple_helper_cb(ch, tflite_server_cb, nullptr);

    if (pipe_client_open(ch, input_pipe.c_str(), kProcessName.c_str(), EN_PIPE_CLIENT_SIMPLE_HELPER, 10 * sizeof(ai_detection_t)))
    {
        fprintf(stderr, "Failed to open pipe: %s\n", input_pipe.c_str());
        return;
    }
}

int main(int argc, char** argv)
{
    setup_pipe();
    while(main_running) {
        usleep(5000000);
    }

    return 0;
}
