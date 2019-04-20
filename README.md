# Example of UDP packet trasmission with libev 
[![Build Status](https://travis-ci.org/silent94/UDP_Scoket_C.svg?branch=master)](https://travis-ci.org/silent94/UDP_Scoket_C)
[![License: MIT](https://img.shields.io/github/license/silent94/UDP_Scoket_C.svg?style=popout)](https://opensource.org/licenses/MIT)

## Install dependency
> For *Ubuntu* :
```Bash
$ sudo apt-get install libev-dev
```

> For *macOS* :
```Bash
$ brew install libev
```


## Build and Make
1. Clone this repository to your workspace :
```Bash
$ git clone https://github.com/silent94/UDP_Scoket_C.git
```

2. Get into the project :
```Bash
$ cd UDP_Socket_C
```

3. Build the project :
```Bash
$ make clean
$ make all 
```

## Run
1. First, run the *receiver* program to start receiving packet :
```Bash
$ ./receiver
```

2. Open a new terminal, run *sender* program to start sending packet:
```Bash
$ ./sender
```