#!/usr/bin/env python

import colorsys
import math
import time

import pantilthat
import atexit


pantilthat.light_mode(pantilthat.WS2812)
pantilthat.light_type(pantilthat.GRB)
def lightout():
	pantilthat.clear()
	pantilthat.show()
atexit.register(lightout)	
pixels = 8
r = 0
g = 0
b = 50


while True:
		pantilthat.set_all(51, 204, 255)
		pantilthat.show()
	 	time.sleep(0.15)
	 	pantilthat.clear()
		pantilthat.show()
		time.sleep(0.15)
		pantilthat.set_all(255, 255, 255)
		pantilthat.show()
		time.sleep(0.15)
		#pantilthat.set_pixel(0, 51, 204, 255)
		#pantilthat.set_pixel(1, 255, 255, 255)
		#pantilthat.set_pixel(2, 51, 204, 255)
	 	#pantilthat.show()
	 	#time.sleep(0.15)
	 	#pantilthat.clear()
		#pantilthat.show()
		#time.sleep(0.15)
	 	#pantilthat.set_pixel(1, 51, 204, 255)
		#pantilthat.set_pixel(0, 255, 255, 255)
		#pantilthat.set_pixel(2, 255, 255, 255)
		#pantilthat.show()
		#time.sleep(0.15)
	 	#pantilthat.clear()
		#pantilthat.show()
		#time.sleep(0.15)
	 	#index = index + 1
	 	#pantilthat.clear()
		#pantilthat.show()
	#while (index < pixels):
		#pantilthat.set_pixel(index, r, g, b)
		#pantilthat.set_pixel(index + 1, r, g, b)
	 	#pantilthat.show()
	 	#time.sleep(0.15)
	 	#index = index + 1
	 	#pantilthat.clear()
		#pantilthat.show()
		##index is now 8
	##while (index == 8):
		##pantilthat.clear()
		##pantilthat.show()	
	#while (index > 0):
		#pantilthat.set_pixel(index, r, g, b)
		#pantilthat.set_pixel(index - 1, r, g, b)
	 	#pantilthat.show()
	 	#time.sleep(0.15)
	 	#index = index - 1
	 	#pantilthat.clear()
		#pantilthat.show()



