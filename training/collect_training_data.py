"""

Collect training data for funchuk.
Funchuk must be connected via serial port and in training mode.

Modified from https://github.com/jakkra/Gesture-Detecting-Macro-Keyboard/blob/main/training/collect_train_data.py
"""

import signal
import sys
import numpy as np
import serial
from matplotlib import pyplot as plt
import argparse
import threading
from pathlib import Path
import os

INPUT_SHAPE_X = 16
INPUT_SHAPE_Y = 16


def signal_handler(sig, frame):
    print('Storing dataset...')
    cwd = os.path.dirname(os.path.realpath(__file__)) + "/data/"
    Path(cwd).mkdir(parents=True, exist_ok=True)
    data_cutoff = (int)(len(training_data)*pct_test_data)
    with open('{0}.train'.format(cwd + gesture_name), "w") as f:
        for sample in training_data[data_cutoff:]:
            f.write('{0}\n'.format(sample))
    with open('{0}.test'.format(cwd + gesture_name), "w") as f:
        for sample in training_data[:data_cutoff]:
            f.write('{0}\n'.format(sample))
    print('Stored, exiting.')
    sys.exit(0)


def console_input_thread():
    while(True):
        try:
            input()
            print('Deleted last sample')
            if (len(training_data) > 0):
                training_data.pop()
                sample_number = len(training_data)
        except KeyboardInterrupt:
            sys.exit(0)
        except EOFError:
            sys.exit(0)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Create and store training data')
    parser.add_argument('--port', type=str, required=True, help='COM port')
    parser.add_argument('--gesture_name', type=str, required=True, help='Name of gesture must be unique')
    parser.add_argument('--pct_test_data', type=float, required=False, help='Percentage of collected data to use as test data', default=0.25)
    args = parser.parse_args()

    print('Press CTRL+C when you are tired of collecting training data and the data will be stored.')
    print('Press ENTER in the console to remove the last sample in case you mess up.\n')

    global training_data
    training_data = []
    global pct_test_data
    pct_test_data = args.pct_test_data
    global gesture_name
    gesture_name = args.gesture_name
    global sample_number
    sample_number = 0

    ser = serial.Serial(args.port, 115200, timeout=1)
    signal.signal(signal.SIGINT, signal_handler)
    console_thread = threading.Thread(target=console_input_thread, daemon=True)
    console_thread.start()

    while True:
        try:
            # receive/check data
            data = ser.readline()
            if not data:
                continue
            data = data.decode("utf-8")
            data = data.split('START:')
            if not (len(data) > 1):
                continue
            data = data[1][:-3].split(',')
            if len(data) < 20:
                print("data too short; skipping")
                continue
            # plot
            img = np.zeros((INPUT_SHAPE_X, INPUT_SHAPE_Y))
            for i in range(0, len(data), 2):
                img[int(data[i + 1]), int(data[i])] = 1
            plt.imshow(img, interpolation='nearest')
            plt.gca().invert_yaxis()
            plt.show(block=False)
            plt.pause(0.1)
            plt.close("all")
            img = img.reshape(1, INPUT_SHAPE_X, INPUT_SHAPE_Y, 1)
            img = img.astype('float32')
            # add
            training_data.append(data)
            sample_number = sample_number + 1
            print('Sample {0} added'.format(sample_number))
        except KeyboardInterrupt:
            print('EXIT')
            ser.close()
            sys.exit()
