
     __  __         _____  _     _      ____                  _       _____                _                  
    |  \/  |       |  __ \(_)   | |    |  _ \                | |     |  __ \              | |                 
    | \  / |_   _  | |  | |_ ___| | __ | |_) | ___ _ __   ___| |__   | |__) |___  __ _  __| |_ __ ___   ___   
    | |\/| | | | | | |  | | / __| |/ / |  _ < / _ \ '_ \ / __| '_ \  |  _  // _ \/ _` |/ _` | '_ ` _ \ / _ \  
    | |  | | |_| | | |__| | \__ \   <  | |_) |  __/ | | | (__| | | | | | \ \  __/ (_| | (_| | | | | | |  __/  
    |_|  |_|\__, | |_____/|_|___/_|\_\ |____/ \___|_| |_|\___|_| |_| |_|  \_\___|\__,_|\__,_|_| |_| |_|\___|  
            __/ |                                                                                            
           |___/                                                                                             


# MyDiskBench
### This is a benchmarking program to calculate R/W speed of a given drive HDD/SSD. It uses POSIX threads for measuring throughput.

####  Its pretty simple to use it

## Pre-Requisite:
### g++ compiler
### pthread library

## Steps to Run
#### execute command 
#### `make`  
##### This will clean and compile the cpp code

### To run the program use following options:

##### `./benchmark <W/WR/R/RR> <# of Threads> <RecordSize in bytes>`

### Executing the code:

#### 1. Write sequential with 2 threads and record size of 4KB
##### `./benchmark W 2 4096`

#### 2. Write random with 48 threads and record size of 16MB
##### `./benchmark WR 48 16777216`

#### 3. Read sequential with 2 threads and record size of 4KB
##### `./benchmark R 2 4096`

#### 4. Read random with 48 threads and record size of 16MB
##### `./benchmark RR 48 16777216`


### Note:
--
1. Benchmark program will read/write 10g of data on to disk.
2. Before performing any READ operations you must first execute WRITE Sequential operation of similar configuration.
3. Record size should be multiple of 512 otherwise there will be Segmentation Fault because default block size is 512 bytes.
4. As O_DIRECT flag is used NOTE 3 is very important because strict memory alignment is prerequisite for using O_DIRECT flag.
5. Number of files created will be equal to the number of threads and named from 0 to N-1 where N: # of threads.