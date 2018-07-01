import sys
import re
import matplotlib.pyplot as plt
import subprocess
from joblib import delayed, Parallel


isCycle = re.compile("\[([0-9]+)\]")
isNewPacket = re.compile("<IN[0-9]+>")
isOldPacket = re.compile("<OUT[0-9]+>")
isReq = re.compile("<REQ>")
isAnsw = re.compile("<ANSW>")

figure_count = 0;

def treatLogFile(logFileSrc):
    global figure_count
    average_latency=-1;
    average_throughput=-1;
    average_req_per_packet=-1
    average_answ_per_packet=-1
    latencies = list();
    throughput = list();
    packet_queue = list();
    req_per_packet = list();
    answ_per_packet = list();
    cycle = 0;

    with open(logFileSrc, "r+") as logFile:
        lines = logFile.readlines();
        i = 0;

        packets_this_cycle = 0;
        req_count = 0;
        answ_count = 0;

        while (cycle < 100000) and (i < len(lines)):

            line = lines[i];
            
            c = isCycle.match(line)
            np = isNewPacket.match(line)
            op = isOldPacket.match(line)
            re = isReq.match(line)
            an = isAnsw.match(line)
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
                req_per_packet.append(req_count)
                req_count = 0;
                answ_per_packet.append(answ_count)
                answ_count = 0;
            elif re != None:
                req_count+=1;
            elif an != None:
                answ_count+=1;

            i = i+1;

        cut_latency = len(latencies);
        cut_throughput = len(throughput);
        cut_req = len(req_per_packet)
        cut_answ = len(answ_per_packet)

        while (i < len(lines)):

            line = lines[i];
            
            c = isCycle.match(line)
            np = isNewPacket.match(line)
            op = isOldPacket.match(line)
            re = isReq.match(line)
            an = isAnsw.match(line)
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
                req_per_packet.append(req_count)
                req_count = 0;
                answ_per_packet.append(answ_count)
                answ_count = 0;
            elif re != None:
                req_count+=1;
            elif an != None:
                answ_count+=1;

            i = i+1;

        average_latency = sum(latencies[cut_latency:])/len(latencies[cut_latency:])
        print("Average latency: " + str(average_latency))
        average_throughput = sum(throughput[cut_throughput:])/len(throughput[cut_throughput:])
        print("Average throughput: " + str(average_throughput))
        average_req_per_packet= sum(req_per_packet[cut_req:])/len(req_per_packet[cut_req:])
        print("Average Req per packet: " + str(average_req_per_packet))
        average_answ_per_packet= sum(answ_per_packet[cut_answ:])/len(answ_per_packet[cut_answ:])
        print("Average Req per packet: " + str(average_answ_per_packet))

        #return (average_throughput, average_latency,) 
        '''
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

        plt.figure(figure_count)
        figure_count+=1
        plt.scatter(range(len(req_per_packet)), req_per_packet, s=1)
        plt.axvline(x=cut_req , color="r", linestyle="--")
        plt.title('REQ per packet')

        plt.figure(figure_count)
        figure_count+=1
        plt.scatter(range(len(answ_per_packet)), answ_per_packet, s=1)
        plt.axvline(x=cut_answ, color="r", linestyle="--")
        plt.title('ANSW per packet')
        '''
    return (average_throughput, average_latency, average_req_per_packet, average_answ_per_packet,) 


def createGraphFile(destPath, inRate, inSize, outRate, outSize, outQSize, latency):
    with open(destPath, "+w") as dest:
        [ dest.write("P: IN" + str(i) + " " + str(inRate) + " " + str(inRate/5) + "\n") for i in range(inSize) ]
        '''
        [ dest.write("P: OUT" + str(i) + " " + str(outRate) + " " + str(outRate/5) + "\n") for i in range(outSize) ]
        [ dest.write("Q: Q" + str(i) + " " + str(outQSize) + "\n") for i in range(outSize) ]
        '''
        [ dest.write("C: OUT" + str(i) + " " + str(outRate) + " " + str(outRate/5) + " " + str(outQSize) + "\n") for i in range(outSize) ]

        dest.write("\n")

        [ [ dest.write("IN" + str(i) + ": OUT" + str(o) + " " + str(latency) + "\n") for o in range(outSize) ] for i in range(inSize) ] 
        '''
        [ [ dest.write("IN" + str(i) + ": Q" + str(o) + " " + str(latency) + "\n") for o in range(outSize) ] for i in range(inSize) ] 
        [ dest.write("Q" + str(o) + ": OUT" + str(o) + " 0\n") for o in range(outSize) ] 
        '''
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

#iterInRate(190, "scredit")
#iterInRate(100,"ack")
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
plt.figure(figure_count);
figure_count+=1;
plt.title("Average REQ per packet");
plt.xlabel("Input throughput");
plt.ylabel("REQ per packet");
for i in range(len(modes)):
    plt.loglog(idx, [ v[2] for v in values[i::len(modes)]], label=modes[i])
plt.legend()
plt.figure(figure_count);
figure_count+=1;
plt.title("Average ANSW per packet");
plt.xlabel("Input throughput");
plt.ylabel("ANSW per packet");
for i in range(len(modes)):
    plt.loglog(idx, [ v[3] for v in values[i::len(modes)]], label=modes[i])
plt.legend()
plt.figure(figure_count)
plt.title("Average latency over average throughput")
plt.xlabel("Output throughput")
plt.ylabel("Latency")
for i in range(len(modes)):
    plt.scatter([v[0] for v in values[i::len(modes)]], [v[1] for v in values[i::len(modes)]], s=1, label=modes[i]) 
plt.axvline(x=1/outRate, label="Output ideal throughput", linestyle="--")
plt.legend()
plt.show()

