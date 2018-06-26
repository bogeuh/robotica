#!/usr/bin/env python

import math
import time
import colorsys
import atexit
import pantilthat
import knightrider
pantilthat.light_mode(pantilthat.WS2812)
pantilthat.light_type(pantilthat.GRB)

def lightout():
	pantilthat.clear()
	pantilthat.show()
	#pan-tilt don't do much
	pantilthat.pan(0)
	pantilthat.tilt(0)
	pantilthat.servo_enable(1, 0)
	pantilthat.servo_enable(2, 0)
atexit.register(lightout)
maxpan = 120
maxtilt = 120
pana = 0
tilta = 0
#pana, tilta = 0, 0
# pantilthat.pan(pana)
# pantilthat.tilt(tilta)
# def panup(x, maxpan):
#     while -maxpan < x < maxpan:
#         x = x - 1
#      x  -maxpan:
#         x = x + 1
#     # t = time.time()
#     # t = round(time.time() * 1000) / 1000
#     # #t*2 = faster movement
#     # a = math.sin(t * 2) * 90
#     # #Cast a to int for v0.0.2
#     # a = int(a)/2
#     return x

def panleft(pan_angle_left, maxpana):
    while (-maxpana-1) < pan_angle_left < maxpana:
        #print(pan_angle_left)
        #knightrider.neorider()
        #neorider creates an infinite loop blocking the whole thing
        pan_angle_left = pan_angle_left + 1
        pantilthat.pan(pan_angle_left/2)
        time.sleep(0.005)
    return pan_angle_left
def panright(pan_angle_right, maxpana):
    while -maxpana < pan_angle_right < (maxpana+1):
        #print(pan_angle_right)
        pan_angle_right = pan_angle_right - 1
        pantilthat.pan(pan_angle_right/2)
        time.sleep(0.005)
    return pan_angle_right
def tiltup(tilt_angle_up, maxpana):
    while (-maxpana-1) < tilt_angle_up < maxpana:
        #print(tilt_angle_up)
        tilt_angle_up = tilt_angle_up + 1
        pantilthat.tilt(tilt_angle_up/2)
        time.sleep(0.005)
    return tilt_angle_up
def tiltdown(tilt_angle_down, maxpana):
    while -maxpana < tilt_angle_down < (maxpana+1):
        #print(tilt_angle_down)
        tilt_angle_down = tilt_angle_down - 1
        pantilthat.tilt(tilt_angle_down/2)
        time.sleep(0.005)
    return tilt_angle_down
pantilthat.pan(0)
pantilthat.tilt(0)    
while True: 
        # print(pana)
        # panleft(pana, maxpan)
        
        # print(panleft(pana, maxpan))
        pana = panleft(pana, maxpan)
      
        pana = panright(pana, maxpan)

        tilta = tiltup(tilta, maxtilt)

        tilta = tiltdown(tilta, maxtilt)
       
        
        # pana = panright(pana, maxpan)
    # panangle = panleft(panangle, maxpan)
        # pana = panright(pana, maxpan)
        
    # panangle = panright(panangle, maxpan)
    # # print(panangle, maxpan))
    # time.sleep(0.5)

# This is what you want:
# def defineAList():
#     local_list = ['1','2','3']
#     print "For checking purposes: in defineAList, list is", local_list 
#     return local_list 

# def useTheList(passed_list):
#     print "For checking purposes: in useTheList, list is", passed_list

# def main():
#     # returned list is ignored
#     returned_list = defineAList()   

#     # passed_list inside useTheList is set to what is returned from defineAList
#     useTheList(returned_list) 

# main()
# You can even skip the temporary returned_list
# and pass the returned value directly to useTheList:

# def main():
#     # passed_list inside useTheList is set to what is returned from defineAList
#     useTheList(defineAList())
        




    # Get the time in seconds
    # t = time.time()

    # G enerate an angle using a sine wave (-1 to 1) multiplied by 90 (-90 to 90)

    
    # b = (math.sin(t * 2) + 1) / 2
    # b = int(b * 255.0)
    
    # t = round(time.time() * 1000) / 1000
    # #a = round(math.sin(t) * 90)
    # c = round(math.tan(t) * 90)

    # # t*2 = faster movement
    # a = math.sin(t * 2) * 90
    # # Cast a to int for v0.0.2
    # a = int(a)
    # c = int(c)

    #pantilthat.tilt(a/2)
    #generate rgb colorwheel
    #r, g, b = [int(x*255) for x in  colorsys.hsv_to_rgb(((t*100) % 360) / 360.0, 1.0, 1.0)]
    #pantilthat.set_all(r, g, b)
    #pantilthat.show()

    # Two decimal places is quite enough!
   # print(round(r,2),round(g,2),round(b,2))
    # print(round(a,2))
    # print(c) 

    # Sleep for a bit so we're not hammering the HAT with updates
    
