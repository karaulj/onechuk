/*
This is a automatically generated TensorFlow Lite model by train_model.py, see README.md for more info.
It is converted into a C data array using xxd and is defined in gesture_model_tflite.cc
*/
#ifndef TENSORFLOW_LITE_GESTURE_MODEL_H_
#define TENSORFLOW_LITE_GESTURE_MODEL_H_
extern const unsigned char gesture_model_tflite_data[];

typedef enum gesture_label_t 
{
	LABEL_RGESTURE_CENTER_CLICK = 0,
	LABEL_RGESTURE_CIRCLE = 1,
	LABEL_RGESTURE_DOWN_FLICK = 2,
	LABEL_RGESTURE_LEFT_FLICK = 3,
	LABEL_RGESTURE_RIGHT_FLICK = 4,
	LABEL_RGESTURE_UP_FLICK = 5,
	LABEL_END
} gesture_label_t;

static inline const char* getNameOfPrediction(gesture_label_t prediction)
{
	switch (prediction) {
		case LABEL_RGESTURE_CENTER_CLICK: return "LABEL_RGESTURE_CENTER_CLICK";
		case LABEL_RGESTURE_CIRCLE: return "LABEL_RGESTURE_CIRCLE";
		case LABEL_RGESTURE_DOWN_FLICK: return "LABEL_RGESTURE_DOWN_FLICK";
		case LABEL_RGESTURE_LEFT_FLICK: return "LABEL_RGESTURE_LEFT_FLICK";
		case LABEL_RGESTURE_RIGHT_FLICK: return "LABEL_RGESTURE_RIGHT_FLICK";
		case LABEL_RGESTURE_UP_FLICK: return "LABEL_RGESTURE_UP_FLICK";
		default: return "UNKNOWN_PREDICTION";
	}
}

#endif  // TENSORFLOW_LITE_GESTURE_MODEL_H_
