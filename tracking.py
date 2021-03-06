#--------------------------
#   title          = object tracking | KRSRI
#   create         = agustus 2021
#   author         = Danu andrean
#   maintenance    = yusuf prasetyo

#---------------------------------------
#   main program 2021
#
#-----------------------------------

import cv2
import numpy as np
import serial
import time 


def callback(x):
    pass

cap = cv2.VideoCapture(0)
cv2.namedWindow('image',cv2.WINDOW_NORMAL)


ilowH = 0
ilowS =149
ilowV = 35
ihighH = 255
ihighS = 255
ihighV = 168

frame_w = 440
frame_h = 280

min_obj = 10*300
max_obj = frame_w*frame_h/1.5
limit_ = 12

#===================set variable ===================
count = 0

#===================================================
# create trackbars for color change

cv2.createTrackbar('lowH','image',ilowH,255,callback)
cv2.createTrackbar('lowS','image',ilowS,255,callback)
cv2.createTrackbar('lowV','image',ilowV,255,callback)
cv2.createTrackbar('highH','image',ihighH,255,callback)
cv2.createTrackbar('highS','image',ihighS,255,callback)
cv2.createTrackbar('highV','image',ihighV,255,callback)



kernelOpen=np.ones((10,10))
kernelClose=np.ones((20,20))
#font=cv2.cv.InitFont(cv2.cv.CV_FONT_HERSHEY_SIMPLEX,2,0.5,0,3,1)

while True:
    ser = serial.Serial('/dev/ttyACM0', 9600)
    ret, frame=cap.read()
    # frame = cv2.flip(frame,1)

    (height_, width_) = frame.shape[:2] #w:frame-width and h:frame-height
    cv2.circle(frame, (width_//2, height_//2), 7, (255, 255, 255), -1) 
    #frame=cv2.resize(frame,(340,220))

    ilowH = cv2.getTrackbarPos('lowH', 'image')
    ilowS = cv2.getTrackbarPos('lowS', 'image')
    ilowV = cv2.getTrackbarPos('lowV', 'image')
    ihighH = cv2.getTrackbarPos('highH', 'image')
    ihighS = cv2.getTrackbarPos('highS', 'image')
    ihighV = cv2.getTrackbarPos('highV', 'image')


    #convert BGR to HSV
    frameHSV= cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)
    #----------------------------------------------------
    lower_hsv = np.array([ilowH, ilowS, ilowV])
    higher_hsv = np.array([ihighH, ihighS, ihighV])
    # create the Mask
    mask=cv2.inRange(frameHSV,lower_hsv,higher_hsv)
    #morphology
    maskOpen=cv2.morphologyEx(mask,cv2.MORPH_OPEN,kernelOpen)
    maskClose=cv2.morphologyEx(maskOpen,cv2.MORPH_CLOSE,kernelClose)

    maskFinal=maskClose
    _,conts,h=cv2.findContours(maskFinal.copy(),cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    data ="0,0,0"
    #print conts
    for countour in conts:
        area = cv2.contourArea(countour)
        #print "area",area

    
    #cv2.drawContours(frame,conts,-1,(255,0,0),3)
        if ((area <max_obj) & (area >min_obj)):         
            objectFound = 1
            count +=1
            # print(count)
            cv2.drawContours(frame,conts,-1,(255,0,0),3)
            
            for i in range(len(conts)):
                x,y,w,h=cv2.boundingRect(conts[i])
                cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,0), 2)

                if(y > (height_/2)):
                    value_y = ((y-(height_/2))*-1)/5
                else:
                    value_y = ((height_/2)-y) /5
				
                if(x > (width_/2)):
                    value_x = (x-(width_/2))/5
                else:
                    value_x = (((width_/2)-x)*-1) /5
                value_x = value_x + limit_
                value_y = value_y + limit_

                cv2.putText(frame,str(round(value_x, 2))+","+str(round(value_y, 2)),(x, y - 20), cv2.FONT_HERSHEY_COMPLEX, 1 ,(0,0,255), 2)

                if (count>=1):
                    data ="*,1"+","+str(value_x)+","+str(value_y)+",#"
                    cv2.putText(frame,'Object Found',(50,50), cv2.FONT_HERSHEY_COMPLEX, 2 ,(0,255,0), 2)        
        else:
            count =0
            
            # for i in range(len(conts)):
            #     x,y,w,h=cv2.boundingRect(conts[i])
            #     cv2.rectangle(frame,(x,y),(x+w,y+h),(0,0,255), 2)

    ser.write(data.encode()) 
    time.sleep(0.1)
    cv2.imshow("mask",mask)
    cv2.imshow("cam",frame)
    cv2.waitKey(10)
    #print ilowH, ilowS, ilowV,ihighH,ihighS,ihighV
    if(cv2.waitKey(1) & 0xFF == ord('q')):
        break


cv2.destroyAllWindows()
cap.release()