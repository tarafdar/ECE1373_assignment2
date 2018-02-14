#!/bin/bash
echo $1
echo $2
./reg_rw /dev/xdma0_user 0x10 w $1
./reg_rw /dev/xdma0_user 0x18 w $2
./reg_rw /dev/xdma0_user 0x0 w 0x1
./reg_rw /dev/xdma0_user 0x0 r

