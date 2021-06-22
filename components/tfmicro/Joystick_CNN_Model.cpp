/*
 * Joystick_CNN_Model.cpp
 *
 *  Created on: Jun 21, 2021
 *      Author: Jacob
 */

#include "Joystick_CNN_Model.h"

// tflite
#include "gesture_model_tflite.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
//#include "tensorflow/lite/schema/schema_generated.h"
// logging
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "Joystick_CNN_Model";

static uint8_t tensorArena[TENSOR_ARENA_SZ];

/*
// Globals, used for compatibility with Arduino-style sketches.
namespace {
    tflite::ErrorReporter* error_reporter = nullptr;
    const tflite::Model* model = nullptr;
    tflite::MicroInterpreter* interpreter = nullptr;
    TfLiteTensor* input = nullptr;
    TfLiteTensor* output = nullptr;
    int inference_count = 0;
    // arena size should be 11136 bytes for 16x16 model
    // from https://github.com/edgeimpulse/tflite-find-arena-size
    constexpr int kTensorArenaSize = 11 * 1024;
    uint8_t tensor_arena[kTensorArenaSize];
}  // namespace
*/

Joystick_CNN_Model::Joystick_CNN_Model(uint32_t inputDimX, uint32_t inputDimY)
    : xDim(inputDimX), yDim(inputDimY)
{
    tflite::InitializeTarget();

    // Set up logging. Google style is to avoid globals or statics because of
    // lifetime uncertainty, but since this has a trivial destructor it's okay.
    // NOLINTNEXTLINE(runtime-global-variables)
    static tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;

    // Map the model into a usable data structure. This doesn't involve any
    // copying or parsing, it's a very lightweight operation.
    model = (tflite::Model*)tflite::GetModel(gesture_model_tflite_data);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        TF_LITE_REPORT_ERROR(error_reporter,
                            "Model provided is schema version %d not equal "
                            "to supported version %d.",
                            model->version(), TFLITE_SCHEMA_VERSION);
        return;
    }

    // This pulls in all the operation implementations we need.
    // NOLINTNEXTLINE(runtime-global-variables)
    static tflite::AllOpsResolver resolver;

    // Build an interpreter to run the model with.
    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensorArena, TENSOR_ARENA_SZ, error_reporter);
    interpreter = &static_interpreter;

    // Allocate memory from the tensor_arena for the model's tensors.
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
        return;
    }

    // Obtain pointers to the model's input and output tensors.
    input = interpreter->input(0)->data.f;
    output = interpreter->output(0);

    // Keep track of how many inferences we have performed.
    inferenceCnt = 0;

    inputLength = interpreter->input(0)->bytes;
    // logging
    ESP_LOGI(TAG, "START model input info");
    ESP_LOGI(TAG, "size (element):  %d", interpreter->input(0)->dims->size);
    ESP_LOGI(TAG, "matrix size:     %dx%dx%d",
        interpreter->input(0)->dims->data[0],
        interpreter->input(0)->dims->data[1],
        interpreter->input(0)->dims->data[2]
    );
    ESP_LOGI(TAG, "dimensions:      (%d,%d)", xDim, yDim);
    ESP_LOGI(TAG, "length (bytes):  %d", interpreter->input(0)->bytes);
    ESP_LOGI(TAG, "END model input info");
    ESP_LOGI(TAG, "START model output info");
    ESP_LOGI(TAG, "size (element):  %d", interpreter->output(0)->dims->size);
    ESP_LOGI(TAG, "# of gestures:   %d", interpreter->output(0)->dims->data[1]);
    ESP_LOGI(TAG, "END model output info");
}


esp_err_t Joystick_CNN_Model::makePrediction(float* inputData, uint32_t inputLen, prediction_result_t* labelProbs)
{
    TfLiteStatus invokeStatus;
    gesture_label_t label;
    float maxProb = 0;

    assert(inputLen == inputLength);

    memcpy(input, inputData, inputLen);

    invokeStatus = interpreter->Invoke();
    if (invokeStatus != kTfLiteOk) {
        ESP_LOGE(TAG, "Invoke ERROR %d\n", invokeStatus);
        return ESP_FAIL;
    }
    inferenceCnt++; 

    //labelProbs = &interpreter->output(0)->data.f;
    
    float* allProbs = interpreter->output(0)->data.f;
    for (int i=0; i<LABEL_END; i++) {
        if (allProbs[i] > maxProb) {
            label = (gesture_label_t)i;
            maxProb = allProbs[i];
        }
    }

    labelProbs->label = label;
    labelProbs->prob = maxProb;
    //ESP_LOGI(TAG, "label %d, result %.3f", (int)label, maxProb);
    

    return ESP_OK;
}
