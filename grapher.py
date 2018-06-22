import sys
import re
import matplotlib.pyplot as plt
import subprocess
from joblib import delayed, Parallel

isCycle = re.compile("\[([0-9]+)\]")
isNewPacket = re.compile("<IN[0-9]+>")
isOldPacket = re.compile("<OUT[0-9]+>")

figure_count = 0;

def treatLogFile(logFileSrc):
    global figure_count
    average_latency=-1;
    average_throughput=-1;
    latencies = list();
    throughput = list();
    packet_queue = list();
    cycle = 0;

    with open(logFileSrc, "r+") as logFile:
        lines = logFile.readlines();
        i = 0;

        packets_this_cycle = 0;

        while (cycle < 10000) and (i < len(lines)):

            line = lines[i];
            
            c = isCycle.match(line)
            np = isNewPacket.match(line)
            op = isOldPacket.match(line)
            if c != None:
                cycle = int(c.group(1));
                throughput.append(packets_this_cycle)
                packets_this_cycle = 0
            elif np != None:
                packet_queue.append(cycle)
            elif op != None:
                l =  packet_queue.pop(0)
                latencies.append(cycle - l)
                last_packet_cycle = cycle
                packets_this_cycle+=1;

            i = i+1;

        cut_latency = len(latencies);
        cut_throughput = len(throughput);

        while (i < len(lines)):

            line = lines[i];
            
            c = isCycle.match(line)
            np = isNewPacket.match(line)
            op = isOldPacket.match(line)
            if c != None:
                cycle = int(c.group(1));
                throughput.append(packets_this_cycle)
                packets_this_cycle = 0
            elif np != None:
                packet_queue.append(cycle)
            elif op != None:
                l =  packet_queue.pop(0)
                latencies.append(cycle - l)
                last_packet_cycle = cycle
                packets_this_cycle+=1;

            i = i+1;

        average_latency = sum(latencies[cut_latency:])/len(latencies[cut_latency:])
        print("Average latency: " + str(average_latency))
        average_throughput = sum(throughput[cut_throughput:])/len(throughput[cut_throughput:])
        print("Average throughput: " + str(average_throughput))

        #return (average_throughput, average_latency,) 
        plt.figure(figure_count)
        figure_count+=1
        plt.scatter(range(len(latencies)), latencies, s=1)
        plt.axvline(x=cut_latency, color="r", linestyle="--")
        plt.title('Packet latencies per packet')

        plt.figure(figure_count)
        figure_count+=1
        plt.scatter(range(len(throughput)), throughput, s=1)
        plt.axvline(x=cut_throughput, color="r", linestyle="--")
        plt.title('Throughput per packet')
    return (average_throughput, average_latency,) 


def createGraphFile(destPath, inRate, inSize, outRate, outSize, outQSize, latency):
    with open(destPath, "+w") as dest:
        [ dest.write("P: IN" + str(i) + " " + str(inRate) + " " + str(inRate/5) + "\n") for i in range(inSize) ]
        [ dest.write("P: OUT" + str(i) + " " + str(outRate) + " " + str(outRate/5) + "\n") for i in range(outSize) ]
        [ dest.write("Q: Q" + str(i) + " " + str(outQSize) + "\n") for i in range(outSize) ]

        dest.write("\n")

        [ [ dest.write("IN" + str(i) + ": Q" + str(o) + " " + str(latency) + "\n") for o in range(outSize) ] for i in range(inSize) ] 
        [ dest.write("Q" + str(o) + ": OUT" + str(o) + " 0\n") for o in range(outSize) ] 


        dest.close();

def treatCase(id, mode, inRate, inSize, outRate, outSize, outQSize, latency):
    createGraphFile(id+".graph", inRate, inSize, outRate, outSize, outQSize, latency)
    subprocess.call("./bin/sim ./"+ id + ".graph ./"+ id +".log --f " + mode, shell=True)
    v = treatLogFile(id+".log");
    subprocess.call("rm ./"+ id + ".graph ./"+ id +".log", shell=True)
    return v

inSize = 2;
outRate = 100;

outSize = 1;
outQSize = 50;
latency = 10;

modes=("ack", "scredit",)
#subprocess.call("make", shell=True)

def iterInRate(inRate, mode):
    return treatCase("inRate" + str(inRate) + "-" + mode, mode, inRate, inSize, outRate, outSize, outQSize, latency)

iterInRate(1000,"scredit")
'''
initialValue = 10
finalValue = 1000 
step=5

values = Parallel(n_jobs=8)(delayed(iterInRate)(i, j) for i  in range(initialValue, finalValue, step) for j in modes)

idx = [ 1/rate for rate in range(initialValue, finalValue, step)]

plt.figure(figure_count);
figure_count+=1;
plt.title("Average throughput");
plt.xlabel("Input throughput");
plt.ylabel("Output throughput");
for i in range(len(modes)):
    plt.loglog(idx, [ v[0] for v in values[i::len(modes)]], label=modes[i])
plt.axhline(y=1/outRate, label="Output ideal throughput", linestyle="--")
plt.legend()
plt.figure(figure_count);
figure_count+=1;
plt.title("Average latency")
plt.xlabel("Input throughput")
plt.ylabel("Latency")
for i in range(len(modes)):
    plt.loglog(idx, [ v[1] for v in values[i::len(modes)]], label=modes[i])
plt.axhline(y=outRate, label="Output average latency", linestyle="--")
plt.legend()
plt.figure(figure_count)
plt.title("Average latency over average throughput")
plt.xlabel("Output throughput")
plt.ylabel("Latency")
for i in range(len(modes)):
    plt.scatter([v[0] for v in values[i::len(modes)]], [v[1] for v in values[i::len(modes)]], s=1, label=modes[i]) 
plt.axvline(x=1/outRate, label="Output ideal throughput", linestyle="--")
plt.legend()
'''
plt.show()

