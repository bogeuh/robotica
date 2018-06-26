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
#can't set brightness
#pantilthat.brightness(0)
def neorider():
	while True:
		index = 0
		# t = round(time.time() * 1000) / 1000
		# r, g, b = [int(x*255) for x in  colorsys.hsv_to_rgb(((t*100) % 360) / 360.0, 1.0, 1.0)]
		while (index == 0):
			t = round(time.time() * 1000) / 1000
			r, g, b = [int(x*255) for x in  colorsys.hsv_to_rgb(((t*100) % 360) / 360.0, 1.0, 1.0)]
			pantilthat.set_pixel(index, r, g, b)
		 	pantilthat.show()
		 	time.sleep(0.05)
		 	index = index + 1
		 	pantilthat.clear()
			pantilthat.show()
		while (index < pixels):
			t = round(time.time() * 1000) / 1000
			r, g, b = [int(x*255) for x in  colorsys.hsv_to_rgb(((t*100) % 360) / 360.0, 1.0, 1.0)]
			pantilthat.set_pixel(index, r, g, b)
			pantilthat.set_pixel(index + 1, r, g, b)
		 	pantilthat.show()
		 	time.sleep(0.05)
		 	index = index + 1
		 	pantilthat.clear()
			pantilthat.show()
			#index is now 8
		#while (index == 8):
			#pantilthat.clear()
			#pantilthat.show()	
		while (index > 0):
			t = round(time.time() * 1000) / 1000
			r, g, b = [int(x*255) for x in  colorsys.hsv_to_rgb(((t*100) % 360) / 360.0, 1.0, 1.0)]
			pantilthat.set_pixel(index, r, g, b)
			pantilthat.set_pixel(index - 1, r, g, b)
		 	pantilthat.show()
		 	time.sleep(0.05)
		 	index = index - 1
		 	pantilthat.clear()
			pantilthat.show()
neorider()

