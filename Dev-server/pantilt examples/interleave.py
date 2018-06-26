# ALTERNATING = INTERLEAVE
# import time
# def infiniteloop():
#     while True:
#         print('Loop 1')
#         time.sleep(1)
#         print('Loop 2')
#         time.sleep(1)

# infiniteloop()
#
#YIELD interleave
# import time
# def a():
#     while True:  # infinite loop nr. 1 (kind of)
#         print('Loop 1')
#         time.sleep(1)
#         yield

# def b():
#     for _ in a():    # infinite loop nr. 2
#         print('Loop 2')
#         time.sleep(1)
# b()
import threading
import time

def infiniteloop1():
    while True:
        print('Loop 1')
        time.sleep(1)

def infiniteloop2():
    while True:
        print('Loop 2')
        time.sleep(1)

# thread1 = threading.Thread(target=infiniteloop1)
# thread1.start()

# thread2 = threading.Thread(target=infiniteloop2)
# thread2.start()

# Ctrl+C terminates the main thread, but because your threads 
# aren't in daemon mode, they keep running, and that keeps the process alive.
#  We can make them daemons:

f = infiniteloop1()
f.daemon = True
f.start()
s = infiniteloop2()
s.daemon = True
s.start()
# But then there's another problem - once the main thread has started your threads, 
# there's nothing else for it to do. So it exits, and the threads are destroyed instantly.
#  So let's keep the main thread alive:

# while True:
#     time.sleep(1)
# Now it will keep print 'first' and 'second' until you hit Ctrl+C.

# Edit: as commenters have pointed out,
#  the daemon threads may not get a chance to clean up things
#   like temporary files. If you need that, then catch the 
#   KeyboardInterrupt on the main thread and have it 
#   co-ordinate cleanup and shutdown. But in many cases, 
#   letting daemon threads die suddenly is probably good enough.

#  is a little bit better.
while threading.active_count() > 1:
	time.sleep(1)