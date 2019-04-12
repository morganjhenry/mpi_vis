import json
import numpy as np

def getTimes(js):
    
    times = []
    for line in js:
        line = line[0:len(line)-2]
        if len(line) != 0:
            j = json.loads(line)
            times.append(j["time"])
    
    return times

def getVariance(time1, time2, time3):
    ex = [];

    for i in range (0, len(time1)):
        temp = [time1[i], time2[i], time3[i]]
        ex.append(temp)

    variance = []

    for data in ex:
        variance.append(np.var(data))
    
    return variance

def main():
    one_js = open("pcap100_sleep_1.js")
    two_js = open("pcap100_sleep_2.js")
    three_js = open("pcap100_sleep_3.js")

    one_js = one_js.readlines();
    two_js = two_js.readlines();
    three_js = three_js.readlines();

    time1 = getTimes(one_js)
    time2 = getTimes(two_js)
    time3 = getTimes(three_js)

    variance = getVariance(time1, time2, time3)
    
    total = 0;
    for v in variance:
        total += v;
    
    avg_variance = total / len(variance)

    print("max variance: " + str(max(variance)))
    print("min variance: " + str(min(variance)))
    print("avg variance: " + str(avg_variance))


main()

