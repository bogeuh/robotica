#!/usr/bin/env python

import colorsys
import math
import time
import atexit
import pantilthat


pantilthat.light_mode(pantilthat.WS2812)
pantilthat.light_type(pantilthat.GRB)
def lightout():
	pantilthat.clear()
	pantilthat.show()
atexit.register(lightout)

r, g, b = 0, 0, 0

while True:
    for x in range(18):
        pantilthat.set_pixel(x, r, g, b)

    pantilthat.show()

    p = int(math.sin(time.time()) * 60)
    t = int(math.sin(time.time()) * 60)

    pantilthat.pan(p)
    pantilthat.tilt(t)
