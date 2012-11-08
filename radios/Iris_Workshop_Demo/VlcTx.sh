#!/bin/bash

vlc $1 --loop --sout '#duplicate{dst=std{access=udp,mux=ts,dst=127.0.0.1:1234},dst=display}'
