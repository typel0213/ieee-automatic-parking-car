import cv2 as cv
import numpy as np

#resize frames
def resized(frame, scale):
    width = int(frame.shape[1]*scale)
    height = int(frame.shape[0]*scale)
    dimensions = (width, height)

    return cv.resize(frame, dimensions, interpolation=cv.INTER_AREA)

#drawing shapes
def drawLine(frame, startPt, endPt, color=(0, 0, 255), thickness=2):
    return cv.line(frame, startPt, endPt, color, thickness)

def drawRect(frame, corner1, corner3, color=(0, 0, 255), thickness=2):
    return cv.rectangle(frame, corner1, corner3, color, thickness)

#adding text
def inputText(frame, text, originPt, fontFace=cv.FONT_HERSHEY_TRIPLEX, scale=1.0, color=(0, 0, 255)):
    return cv.putText(frame, text, originPt, fontFace, scale, color)

#colorspaces
def grayscale(frame):
    #cv.imshow("Grayscale Video", gray)
    return cv.cvtColor(frame, cv.COLOR_BGR2GRAY)

def toRGB(frame):
    #OpenCV takes BGR, so RGB colors will be inverted.
    return cv.cvtColor(frame, cv.COLOR_BGR2RGB)

#blur
def averageBlur(frame, xKernel, yKernel):
    return cv.blur(frame, (xKernel, yKernel)) 

def gaussianBlur(frame, xKernel=5, yKernel=5, standardDeviationX=0, standardDeviationY=0, borderType=cv.BORDER_DEFAULT):
    return cv.GaussianBlur(frame, (xKernel, yKernel), standardDeviationX, standardDeviationY, borderType) 

#masking
def masking(frame, P1, P2, P3, P4):
    h, w = frame.shape[:2]
    mask = np.zeros((h, w), dtype = 'uint8') 
    keepVertices = np.array([[P1, P2, P3, P4]], dtype=np.int32)
    mask = cv.fillPoly(mask, keepVertices, 255) 
    masked_image = cv.bitwise_and(frame, frame, mask = mask)
    return masked_image

#threshold
def simpleThresholding(grayFrame, thresh, inv=False):
    if inv == True:
        return cv.threshold(grayFrame, thresh, 255, cv.THRESH_BINARY_INV)[1]
    return cv.threshold(grayFrame, thresh, 255, cv.THRESH_BINARY)[1]

def adaptiveThresholding(grayFrame, boxSize, c, inv=False, gaus=False):
    if inv==True:
        if gaus==True:
            return cv.adaptiveThreshold(grayFrame, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY_INV, boxSize, c)
        return cv.adaptiveThreshold(grayFrame, 255, cv.ADAPTIVE_THRESH_MEAN_C, cv.THRESH_BINARY_INV, boxSize, c)
    if gaus==True:
        return cv.adaptiveThreshold(grayFrame, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, boxSize, c)
    return cv.adaptiveThreshold(grayFrame, 255, cv.ADAPTIVE_THRESH_MEAN_C, cv.THRESH_BINARY, boxSize, c)

#contour geometry
class Contour:
    def __init__(self, contour):
        self.contour = contour
    def area(self):
        return cv.contourArea(self.contour)
    def perimeter(self, closed=True):
        return cv.arcLength(self.contour, closed)
    def boundRect(self):
        return cv.boundingRect(self.contour)
    def centroid(self):
        M = cv.moments(self.contour)
        if M["m00"] != 0:
            cx = int(M["m10"] / M["m00"])
            cy = int(M["m01"] / M["m00"])
            return (cx, cy)
        return None
    def contourApprox(self, closed=True):
        epsilon = 0.02*cv.arcLength(self.contour, closed)
        return cv.approxPolyDP(self.contour, epsilon, closed)
    def minAreaRect(self):
        return cv.minAreaRect(self.contour)
    def convexHull(self):
        return cv.convexHull(self.contour)
    def draw(self, frame, color=(0, 0, 255), thickness=2):
        cv.drawContours(frame, [self.contour], -1, color, thickness)
        return frame

#contour detection
def detectContours(cannyOrThreshFrame):
    contours, hierarchy = cv.findContours(cannyOrThreshFrame, cv.RETR_LIST, cv.CHAIN_APPROX_NONE)
    return contours

#sliding windows - add check if boundary not detected?
def slidingWindows(grayThreshMaskFrame, numberOfWindows = 10, margin = 50, minPixels = 20):
    height, width = grayThreshMaskFrame.shape[:2]
    windowHeight = height // numberOfWindows
    
    nonzero = grayThreshMaskFrame.nonzero() 
    nonzeroY = np.array(nonzero[0])
    nonzeroX = np.array(nonzero[1])
    
    bottomHalf = grayThreshMaskFrame[height//2: , : ]
    histogram = np.sum(bottomHalf == 255, axis=0) 
    midpoint = width//2
    leftStart =  np.argmax(histogram[:midpoint]) 
    rightStart =  np.argmax(histogram[midpoint:])+midpoint

    leftIndices = [] 
    rightIndices = []
    yLow = height-windowHeight
    yHigh = height
    leftCurrent = leftStart
    rightCurrent = rightStart

    for i in range(numberOfWindows):
        leftLeft = leftCurrent-margin
        leftRight = leftCurrent+margin
        rightLeft = rightCurrent-margin
        rightRight = rightCurrent+margin

        inLeftWindow = ((nonzeroX>=leftLeft) & (nonzeroX<leftRight)) & ((nonzeroY>=yLow) & (nonzeroY<yHigh))
        leftPixelCount = 0
        leftWindowIndices = []
        for j in range(inLeftWindow.size):
            if (inLeftWindow[j]==True):
                leftIndices.append(j)
                leftWindowIndices.append(j)
                leftPixelCount = leftPixelCount+1
        leftIsEnough = leftPixelCount>=minPixels
        if (leftIsEnough):
            leftCurrent = np.average(nonzeroX[leftWindowIndices])

        inRightWindow = ((nonzeroX>=rightLeft) & (nonzeroX<rightRight)) & ((nonzeroY>=yLow) & (nonzeroY<yHigh))
        rightPixelCount = 0
        rightWindowIndices = []
        for j in range(inRightWindow.size):
            if (inRightWindow[j]==True):
                rightIndices.append(j)
                rightWindowIndices.append(j)
                rightPixelCount = rightPixelCount+1
        rightIsEnough = rightPixelCount>=minPixels
        if (rightIsEnough):
            rightCurrent = np.average(nonzeroX[rightWindowIndices])

        yLow = yLow-windowHeight
        yHigh = yHigh-windowHeight

    leftX = nonzeroX[leftIndices]
    leftY = nonzeroY[leftIndices]
    rightX = nonzeroX[rightIndices]
    rightY = nonzeroY[rightIndices]

    return leftX, leftY, rightX, rightY

#fit lines
def fitLines(leftX, leftY, rightX, rightY):
    leftLane = np.polyfit(leftY, leftX, 2)
    rightLane = np.polyfit(rightY, rightX, 2)
    return leftLane, rightLane

#draw the lines
def drawLaneLines(frame, leftLane, rightLane):
    height, width = frame.shape[:2]
    
    aL, bL, cL = leftLane
    aR, bR, cR = rightLane

    leftPoints = []
    rightPoints = []
    
    plotY = np.linspace(height-1, 0, 50)
    
    for y in plotY:
        xSPLeft = (aL*(y**2)) + (bL*(y)) + cL
        leftPoints.append((xSPLeft, y))
        xSPRight = (aR*(y**2)) + (bR*(y)) + cR
        rightPoints.append((xSPRight, y))

    leftPoints = np.array(leftPoints, dtype=np.int32)
    rightPoints = np.array(rightPoints, dtype=np.int32)

    cv.polylines(frame, [leftPoints], False, (0, 0, 255), 2)
    cv.polylines(frame, [rightPoints], False, (0, 0, 255), 2)

    return frame

def calculateError(frame, leftLane, rightLane):
    height, width = frame.shape[:2]
        
    aL, bL, cL = leftLane
    aR, bR, cR = rightLane

    yAhead = int(height*0.75)
    xAheadL = (aL*(yAhead**2)) + (bL*(yAhead)) + cL
    xAheadR = (aR*(yAhead**2)) + (bR*(yAhead)) + cR


    cameraMid = width/2
    laneMid = (xAheadR+xAheadL)/2
    error = laneMid-cameraMid

    return error

#parking-space identification
def maskParkingSpot(threshFrame, rightLane, offset=0):
    height, width = threshFrame.shape[:2]
    aR, bR, cR = rightLane
    
    bottomY = height-1
    topY = 0
    bottomX = int((aR*(bottomY**2)) + (bR*(bottomY)) + cR)
    topX = int((aR*(topY**2)) + (bR*(topY)) + cR)

    maskedPark = masking(threshFrame, ((bottomX+offset, bottomY)), ((topX+offset, topY)), ((width-1, bottomY)), ((width-1, topY)))
    return maskedPark

def detectParkingSpot(parkMaskedFrame):
    detected = False
    contours = detectContours(parkMaskedFrame)
    boxes = []
    for contour in contours:
        shape = Contour(contour)
        area = shape.area()
        x, y, w, h = shape.boundRect()
        approx = shape.contourApprox()
        if area > 400 and len(approx) >= 3:
            detected = True
            boxes.append((x, y, w, h))
    return detected, boxes

def drawParkingSpot(frame, detected, boxes):
    if detected:
        inputText(frame, "PARKING_SPOT_DETECTED", (50, 50))
        for x, y, w, h in boxes:
            drawRect(frame,  (x, y), (x+w, y+h))
    return frame

#read the video
def readVideo(path):
    capture = cv.VideoCapture(path)
    print("Video opened:", capture.isOpened())
    return capture

#display one frame
def displayFrame(frame):
    cv.imshow("Video", frame)
    cv.waitKey(40)

#main :)
def main():
    path = (r"path")#int arg (0,1,2,3) or a path to a vidoe file
    video = readVideo(path) 
    frameCount=0

    while True:
        isTrue, frame = video.read()
        print("Frame captured:", isTrue)

        if not isTrue:
            print("Video ended or frame could not be read!")
            break

        frameCount = frameCount+1
        if frameCount%3 != 0:
            continue 

        resized = resized(frame, 0.75)
        gray = grayscale(resized)
        withThresh = adaptiveThresholding(gray, ...)

        #lane detection
        maskedLane = masking(withThresh, (0, 500), (300, 300), (1000, 300), (1100, 700))
        leftX, leftY, rightX, rightY = slidingWindows(maskedLane)
        leftLane, rightLane = fitLines(leftX, leftY, rightX, rightY) 
        withLanes = drawLaneLines(resized, leftLane, rightLane)
        error = calculateError(resized, leftLane, rightLane)

        #parking detection
        maskedParking = maskParkingSpot(withThresh, rightLane)
        detected, boxes = detectParkingSpot(maskedParking)
        withLanesAndParking = drawParkingSpot(withLanes, detected, boxes)

        #display results
        displayFrame(withLanesAndParking)
        print(error)
                
    video.release()
    cv.destroyAllWindows()

if __name__ == "__main__":
    main()
