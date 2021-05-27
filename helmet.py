import serial
import cv2 as cv
import numpy as np
import struct

WIDTH = 10
HEIGHT = 30

s = serial.Serial("COM5", baudrate=115200)

img = cv.imread("kek.png")
img = cv.resize(img, (WIDTH,  HEIGHT))

while True:
    for row in img:
        for pix in row:
            msg = struct.pack("BBB", pix[2], pix[1], pix[0])
            s.write(msg)


    cv.imshow("Image", img)
    cv.waitKey(1)

    img = np.roll(img, 1, axis=1)