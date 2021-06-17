# Copyright 2019 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#
# This component was generated for the 'hello_world' TF Micro example.
#

# Make sure that the IDF Path environment variable is defined
if(NOT DEFINED ENV{IDF_PATH})
  message(FATAL_ERROR "The IDF_PATH environment variable must point to the location of the ESP-IDF.")
endif()


#idf_component_register(SRCS "tensorflow/lite/micro/gesture_model_tflite.cc"
idf_component_register(SRC_DIRS     "."
                    INCLUDE_DIRS    "."
                    REQUIRES log)

