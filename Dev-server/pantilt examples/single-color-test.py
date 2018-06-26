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


r = 255
g = 0
b = 0
while True:
	pantilthat.set_all(r, g, b)
	pantilthat.show()

