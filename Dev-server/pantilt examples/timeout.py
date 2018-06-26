#!/usr/bin/env python

import time

import pantilthat
import atexit
def lightout():
	pantilthat.clear()
	pantilthat.show()
	pantilthat.pan(0)
	pantilthat.tilt(0)
	pantilthat.servo_enable(1, 0)
	pantilthat.servo_enable(2, 0)
atexit.register(lightout)




def panleft():
	for x in range(90):
		pantilthat.pan(x)
		pantilthat.idle_timeout(0.5)
#    pantilthat.tilt(15)
#time.sleep(3)
def panright():
	for x in range(-90):
		pantilthat.pan(x)
		pantilthat.idle_timeout(0.5)
pantilthat.pan(0)
pantilthat.tilt(0)		
panright()
panleft()




