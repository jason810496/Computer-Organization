import configparser
import subprocess
import os
import sys

if __name__ == "__main__":
    config = configparser.ConfigParser()
    config.read('config.conf')
    cache_set =  config['cache']['Set']
    cache_way =  config['cache']['Way']
    cache_block_size = config['cache']['BlockSize']
    policy = config['cache']['Policy']
    
    if (sys.argv[1] == "build"):
        os.system("make " + policy)
    elif (sys.argv[1] != "test"):
        print("wrong argument")
        exit(0)

    output = subprocess.check_output("find \"./benchmark\" -name *.c -printf \"%f\n\"", shell=True, text=True)
    benchmarks = output.split("\n")
    benchmarks.pop()
    
    avg_miss_rate = 0

    for benchmark in benchmarks:
        os.system("make compile FILE_NAME=./benchmark/" + benchmark)
        output = subprocess.run(["make", "run", "CACHE_SET=" + cache_set, "CACHE_WAY=" + cache_way, "CACHE_BLOCKSIZE=" + cache_block_size], capture_output=True, text=True)
        lines = output.stdout.split("\n")
        avg_miss_rate += float(lines[-2].split()[3].split('%')[0])

    avg_miss_rate /= len(benchmarks)
    os.system("make clean")

    print("\n\n=======================================================================")
    if (sys.argv[1] == "build"):
        print("Policy: " + policy)
    print("Data Cache Setting with: " + str(cache_set) + ":" + str(cache_way) + ':' + str(cache_block_size))
    print("Miss Rate: " + str(round(avg_miss_rate, 4)) + " %")
        
