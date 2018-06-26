#def get_new_id(old):
    #return "YES"

#def get_new_project(old):
    #return ("YEP" + myid)

#if __name__ == '__main__': 
    #myid = ''
    #project = ("Yep"+myid) 

    #print "myid", myid
    #print "project ", project
    #print

    #myid = get_new_id(myid)

    #print "myid", myid
    #print "project ", project
    #print

    #project = get_new_project(project)

    #print "myid", myid
    #print "project ", project
    #print
# This is a global variable
#a = 0

#if a == 0:
    ## This is still a global variable
    #b = 1

#def my_function(c):
    ## this is a local variable
    #d = 3
    #print(c)
    #print(d)

## Now we call the function, passing the value 7 as the first and only parameter
#my_function(7)

## a and b still exist
#print(a)
#print(b)

## c and d don't exist anymore -- these statements will give us name errors!
#print(c)
#print(d)

# We define some options
#LOWER, UPPER, CAPITAL = 1, 2, 3

#name = "jane"
## We use our constants when assigning these values...
#print_style = UPPER

## ...and when checking them:
#if print_style == LOWER:
    #print(name.lower())
#elif print_style == UPPER:
    #print(name.upper())
#elif print_style == CAPITAL:
    #print(name.capitalize())
#else:
    ## Nothing prevents us from accidentally setting print_style to 4, 90 or
    ## "spoon", so we put in this fallback just in case:
    #print("Unknown style option!")
r = 0
g = 0
b = 0
def change():
	r,g,b = 255,0,0
	print (r)
change()
print (r)
print (r)
