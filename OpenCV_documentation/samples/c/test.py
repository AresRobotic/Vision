#!/usr/bin/python
# Import the CV module
import cv2 as cv
# Load the image
img = cv.LoadImage('baboon.jpg',cv.CV_LOAD_IMAGE_COLOR)
# Create a named window
cv.NamedWindow('BaboonImage',cv.CV_WINDOW_AUTOSIZE)
# Display the image
cv.ShowImage('BaboonImage',img)
# Wait for keystroke
cv.WaitKey(0)
#
cv.DestroyWindow('BaboonImage')
