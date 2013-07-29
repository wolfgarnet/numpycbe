#We must add up the sums and divide by s*k
#We must add up the individual times.
#We should do the total time benchmark in a seperate program,
#where time is not wasted starting and stopping the timer.

#Also, we must free memory after each "run".

import time
import numpycbe

def MC_int_old1(s,k):

    #timeRandom = 0
    #timeMult = 0
    #timeAdd = 0
    #timeLessEqual = 0
    #timeSum = 0
    Z = 0
    
    #start=time.time()
    x=numpycbe._initVector()
    #stop=time.time()
    #print 'time for initVector: ', (stop-start)
    y=numpycbe._initVector()
    
    for i in range(0, k):

        #print 'Starting random'
        #start=time.time()
        x=numpycbe.random(x)
        #stop=time.time()
        #timeRandom = timeRandom + (stop-start)
        #print 'Ending random'
        
        y=numpycbe.random(y)

        #print 'Starting mult'
        #start=time.time()
        x=numpycbe.multiply(x,x)
        #stop=time.time()
        #timeMult = timeMult + (stop-start)
        #print 'Ending mult'
        
        y=numpycbe.multiply(y,y)

        #print 'Starting add'
        #start=time.time()
        x=numpycbe.add(x,y)
        #stop=time.time()
        #timeAdd = timeAdd + (stop-start)
        #print 'Starting add'

        #print 'Starting lessequal'
        #start=time.time()
        Y=numpycbe.lessequal(x, 1.0)
        #stop=time.time()
        #timeLessEqual = timeLessEqual + (stop-start)
        #print 'Ending lessequal'

        #print 'Starting sum'
        #start=time.time()
        Z = Z + numpycbe.sum(Y)
        #stop=time.time()
        #timeSum = timeSum + (stop-start)
        #print 'Starting sum'

        #It should not be necessary to free anything!
        
    else:

        #Print times
        #print 'Total time for random: ' + str(timeRandom)
        #print 'Total time for mult: ' + str(timeMult)
        #print 'Total time for add: ' + str(timeAdd)
        #print 'Total time for lessequal: ' + str(timeLessEqual)
        #print 'Total time for sum: ' + str(timeSum)
        
        return Z*4.0/(s*k)
    
    
#First argument must currently match the size of the array we initialize - this
#is only so that the last division with s gives the correct result.
#Second argument is the number of times we execute the MC routine using the same
#data space.
start = time.time()
result = MC_int_old1(64*64*64*64,1)
stop = time.time()
print 'Total time for MC: ' + str(stop-start)
print result