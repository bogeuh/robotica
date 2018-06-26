#!/usr/bin/env python

import colorsys
import math
import time

import pantilthat



pantilthat.light_mode(pantilthat.WS2812)
pantilthat.light_type(pantilthat.GRB)

import atexit
def lightout():
	pantilthat.clear()
	pantilthat.show()
atexit.register(lightout)


color_values = [51, 102, 153, 204, 255]
reverse_cv = [204, 153, 102, 51, 0]
#color_values = [153]
#print (color_values[3])
pixels = 8


while True:

	for y in color_values:
			b = y
			pantilthat.set_all(255, 0, b)
			pantilthat.show()
			time.sleep(0.3)
	for x in reverse_cv:
		r = x
		pantilthat.set_all(r, 0, 255)
		pantilthat.show()
		time.sleep(0.3)
	for z in color_values:
		g = z
		pantilthat.set_all(0, z, 255)
		pantilthat.show()
		time.sleep(0.3)
	for y in reverse_cv:
			b = y
			pantilthat.set_all(0, 255, b)
			pantilthat.show()
			time.sleep(0.3)
	for x in color_values:
		r = x
		pantilthat.set_all(r, 255, 0)
		pantilthat.show()
		time.sleep(0.3)
	for z in reverse_cv:
		g = z
		pantilthat.set_all(255, z, 0)
		pantilthat.show()
		time.sleep(0.3)
	
				
#while True:
	#pantilthat.set_all(204, 102, 0)
	#pantilthat.show()
	#time.sleep(0.15)
	#pantilthat.set_all(102, 102, 102)
	
	#pantilthat.show()
	#time.sleep(0.15)



		##pantilthat.set_pixel(0, 51, 204, 255)
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



