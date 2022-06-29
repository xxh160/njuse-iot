#! /bin/bash

HERE=$(dirname $(readlink -f "$0"))
CAMERA=$HERE/camera
OUTPUT=out
EXEC=main
RES=$1

# env variables
export LD_LIBRARY_PATH=/usr/lib64
export LD_LIBRARY_PATH=/home/HwHiAiUser/ascend_ddk/arm/lib:/home/HwHiAiUser/Ascend/acllib/lib64:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=/home/HwHiAiUser/Ascend/acllib/lib64:${LD_LIBRARY_PATH}
export PYTHONPATH=/home/HwHiAiUser/Ascend/pyACL/python/site-packages/acl:$PYTHONPATH
export ASCEND_AICPU_PATH=/home/HwHiAiUser/Ascend
export PKG_CONFIG_PATH=/usr/share/pkgconfig/
export PYTHONPATH=$HOME/ascend_ddk/:$PYTHONPATH
export PATH=$PATH:/home/HwHiAiUser/ascend_ddk/arm/bin

# enter dir
cd $CAMERA/$OUTPUT 

# run camera, ignore output
./$EXEC -i -c 0 -o ./$RES --overwrite

# cp result to here
cp $RES $HERE 

