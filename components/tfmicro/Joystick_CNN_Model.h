/*
 * Joystick_CNN_Model.h
 * 
 *  Shallow class mainly to house public model functions and namespace.
 *
 *  Created on: Jun 21, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_TFMICRO_JOYSTICK_CNN_MODEL_
#define COMPONENTS_TFMICRO_JOYSTICK_CNN_MODEL_

#include "gesture_model_tflite.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"

#include "esp_err.h"


// 11136 bytes for 16x16 model
// from https://github.com/edgeimpulse/tflite-find-arena-size
#define TENSOR_ARENA_SZ     11*1024


typedef struct prediction_result_t {
    gesture_label_t label;
    float prob;
} prediction_result_t;


class Joystick_CNN_Model
{
public:
    Joystick_CNN_Model(uint32_t inputDimX, uint32_t inputDimY);
    esp_err_t makePrediction(float* inputData, uint32_t inputLen, prediction_result_t* labelProbs);
//protected:
//    void prepare
private:
    const uint32_t xDim, yDim;
    uint32_t inputLength;
    static const int kTensorArenaSize = 11 * 1024;
    //uint8_t tensorArena[kTensorArenaSize];

    tflite::ErrorReporter* error_reporter = nullptr;
    tflite::Model* model = nullptr;
    tflite::MicroInterpreter* interpreter = nullptr;

    float* input = nullptr;
    TfLiteTensor* output = nullptr;

    int inferenceCnt = 0;
};


#endif /* COMPONENTS_TFMICRO_JOYSTICK_CNN_MODEL_ */