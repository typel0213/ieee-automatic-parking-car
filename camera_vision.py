import cv2 as cv
import numpy as np

def readVideo(path):
    capture = cv.VideoCapture(path)
    print("Video opened:", capture.isOpened())
    return capture

def resized(frame, scale):
    width = int(frame.shape[1]*scale)
    height = int(frame.shape[0]*scale)
    dimensions = (width, height)

    return cv.resize(frame, dimensions, interpolation=cv.INTER_AREA)

def drawLine(frame, startPt, endPt, color=(0, 0, 255), thickness=2):
    return cv.line(frame, startPt, endPt, color, thickness)

def drawRect(frame, corner1, corner3, color=(0, 0, 255), thickness=2):
    return cv.rectangle(frame, corner1, corner3, color, thickness) #thickness=-1 --> cv.FILLED

def drawCircle(frame, originPt, radius, color=(0, 0, 255), thickness=2):
    return cv.circle(frame, originPt, radius, color, thickness)

def inputText(frame, text, originPt, fontFace=cv.FONT_HERSHEY_TRIPLEX, scale=1.0, color=(0, 0, 255)):
    return cv.putText(frame, text, originPt, fontFace, scale, color)

def grayscale(frame):
    return cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
     
def toHSV(frame):
    return cv.cvtColor(frame, cv.COLOR_BGR2HSV)

def toLAB(frame):
    return cv.cvtColor(frame, cv.COLOR_BGR2LAB)

def toRGB(frame):
    #OpenCV takes BGR, so RGB colors will be inverted, other programs use RGB
    return cv.cvtColor(frame, cv.COLOR_BGR2RGB)

def splitColorChannels(frame, colorInitial): 
    #Possible Color Initials: "B", "G", "R" - the lighter the grey, the more of that color (white in image b = lots of blue)
    b, g, r = cv.split(frame)
    if colorInitial == "B":
        return b
    if colorInitial == "G":
        return g
    if colorInitial == "R":
        return r

def gaussianBlur(frame, xKernel=5, yKernel=5, standardDeviationX=0, standardDeviationY=0, borderType=cv.BORDER_DEFAULT):
  return cv.GaussianBlur(frame, (xKernel, yKernel), standardDeviationX, standardDeviationY, borderType) 

def BITWISEand(frame1, frame2): #intersecting regions
    return cv.bitwise_and(frame1, frame2)

def BITWISEor(frame1, frame2): #non-intersecting and intersecting regions
    return cv.bitwise_or(frame1, frame2)

def BITWISExor(frame1, frame2): #non-intersecting regions
    return cv.bitwise_xor(frame1, frame2)

def BITWISEnot(frame): #whatever is not in the region
    return cv.bitwise_not(frame)

def masking(frame, P1, P2, P3, P4):
    h, w = frame.shape[:2]
    mask = np.zeros((h, w), dtype = 'uint8')
    keepVertices = np.array([P1, P2, P3, P4], dtype=np.int32)
    mask = cv.fillPoly(mask, keepVertices, 255)
    masked_image = cv.bitwise_and(frame, frame, mask = mask)
    return masked_image

def simpleThresholding(grayFrame, thresh, inv=False):
    #thresh is an integer from 0-255; the higher the integer, the more spaces will be black for normal and less will be black for inverse
    if inv == True:
        return cv.threshold(grayFrame, thresh, 255, cv.THRESH_BINARY_INV)[1] #cv.threshold returns both the threshold value used, and the resulting image, but I think we only need the image
    return cv.threshold(grayFrame, thresh, 255, cv.THRESH_BINARY)[1]

def adaptiveThresholding(grayFrame, boxSize, c, inv=False, gaus=False): 
    #calculates a different threshhold value for each kernel(small box on the image)
    if inv==True:
        if gaus==True:
            return cv.adaptiveThreshold(grayFrame, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY_INV, boxSize, c)
        return cv.adaptiveThreshold(grayFrame, 255, cv.ADAPTIVE_THRESH_MEAN_C, cv.THRESH_BINARY_INV, boxSize, c)
    if gaus==True:
        return cv.adaptiveThreshold(grayFrame, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, boxSize, c)
    return cv.adaptiveThreshold(grayFrame, 255, cv.ADAPTIVE_THRESH_MEAN_C, cv.THRESH_BINARY, boxSize, c)

def cannyEdgeDetection(grayBlurFrame, lowerThresh, upperThresh):
    #input a frame that is both grayscale & with gaussian blur
    return cv.Canny(grayBlurFrame, lowerThresh, upperThresh)

def detectContours(cannyOrThreshFrame, showOriginal=False):
    blankBlack=np.zeros_like(cannyOrThreshFrame)
    contours, hierarchy = cv.findContours(cannyOrThreshFrame, cv.RETR_LIST, cv.CHAIN_APPROX_NONE)
    if showOriginal==True:
        return cv.drawContours(cannyOrThreshFrame, contours, -1, (0,0,255), 2) #contourIDX=-1 means draw all the contours
    else:
        return cv.drawContours(blankBlack, contours, -1, (255,255,255), 1)

#hough line transform

#contour geometry

#morphological transformations

def displayFrame(frame):
    cv.imshow("Video", frame)
    cv.waitKey(40)

def main():
    path = (r"")#int arg (0,1,2,3) or a path to a vidoe file if using webcam, we need to webcam: integer (0) if multiple cameras, 1 for camera, etc (you just need to connect them))
    video = readVideo(path) 
    
    while True: #capture frames forever until we tell it to break    
        isTrue, frame = video.read()
        print("Frame captured:", isTrue)

        if not isTrue: ##or press a key to break
            print("Video ended or frame could not be read!")
            break

        displayFrame(frame)
                
    video.release()
    cv.destroyAllWindows()


if __name__ == "__main__":
    main()
