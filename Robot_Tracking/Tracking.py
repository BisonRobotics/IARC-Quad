import cv2
import numpy as np

robot_types = 2
camera = cv2.VideoCapture(0)
robot_minimum_hue = [38, 160]
robot_maximum_hue = [75, 179]
camera.set(cv2.CV_CAP_PROP_EXPOSURE, 0.5)
camera.set(cv2.CV_CAP_PROP_BRIGHTNESS, 0.5)
camera.set(cv2.CV_CAP_PROP_CONTRAST, 0.5)
opened = camera.isOpened()
if opened == False:
    print("Error opening camera!")

def blob(original_image, color_filtered):
    # Setup SimpleBlobDetector parameters.
    params = cv2.SimpleBlobDetector_Params()


    params.filterByColor = False

    # Filter by Area.
    params.filterByArea = True
    params.minArea = 1500

    # Filter by Circularity
    params.filterByCircularity = True
    params.minCircularity = 0.1

    # Filter by Convexity
    params.filterByConvexity = True
    params.minConvexity = 0.70

    # Filter by Inertia
    params.filterByInertia = True
    params.minInertiaRatio = 0.01

    blobsDetector = cv2.SimpleBlobDetector_create(params)
    keypoints = []
    blobsDetector.detect(color_filtered, keypoints)
    return keypoints

class detected_robot:
    robot_type = 0
    x_position = 0.0
    y_position = 0.0

    def __init__(self, r, x, y):
        self.robot_type = r
        self.x_position = x
        self.y_position = y

def get_robot_positions():

    detections = []
    original_image = cv2.Mat()
    img_HSV = cv2.Mat()

    camera.read(original_image)
    cv2.medianBlur(original_image, img_HSV, 3)
    cv2.cvtColor(img_HSV, img_HSV, cv2.COLOR_BGR2HSV)

    for i in range(robot_types):

        minimum_hue = robot_minimum_hue[i]
        maximum_hue = robot_maximum_hue[i]
        color_filtered = cv2.Mat()

        cv2.inRange(img_HSV, cv2.Scalar(minimum_hue, 50, 50), cv2.Scalar(maximum_hue, 255, 255), color_filtered)

        cv2.erode(color_filtered, color_filtered, cv2.getStructuringElement(cv2.MORPH_ELLIPSE, cv2.Size(5, 5)) )
        cv2.dilate( color_filtered, color_filtered, cv2.getStructuringElement(cv2.MORPH_ELLIPSE, cv2.Size(5, 5)) )

        cv2.dilate( color_filtered, color_filtered, cv2.getStructuringElement(cv2.MORPH_ELLIPSE, cv2.Size(5, 5)) )
        cv2.erode(color_filtered, color_filtered, cv2.getStructuringElement(cv2.MORPH_ELLIPSE, cv2.Size(5, 5)) )

        keypoints = blob(original_image, color_filtered)
        for t in keypoints:
            cv2.circle(original_image, keypoints[t].pt, 4, cv2.Scalar(255, 0, 255), -1)
            robot = detected_robot(i, keypoints[t].pt.x, keypoints[t].pt.y)
            detections.append(robot)

    cv2.imshow("detections", original_image)
    cv2.waitKey()
    return detections
