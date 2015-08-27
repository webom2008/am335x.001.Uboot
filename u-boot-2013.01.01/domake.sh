#!/bin/bash
#
# Copyright (c) 2013-2014, SeeCare.CVTE
# All rights reserved.
#
##############################################
# Author	: QiuWeibo
# Version	: V1.0.0
# Date		: 2014-6-10
# Comment	: script for make uboot project for aPM-335x
##############################################
# Author	: QiuWeibo
# Version	: V1.0.1
# Date		: 2014-6-12
# Comment	: Add MLO or u-boot.img build independently scripts.

# 1st Method : complied objects in the same path with source code
make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm distclean
# 1.1. Both MLO and u-boot.img may be built simultaneously using
make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm aPM_MPU
#make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm aPM_MPU_SDCard

# 1.2. whether MLO or u-boot.img may be built
#make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm aPM_MPU_config
# 1.2.1 MLO(U-boot_min) Build
#make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm MLO
# 1.2.2 u-boot.img Build
#make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm u-boot.img

cp -f ./MLO* ./u-boot.img ~/tftproot/

# 2nd Method : Complied objects in the specify paths by user defined
#rm -rf ./aPM_MPU
#make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=aPM_MPU aPM_MPU
#cp -f ./aPM_MPU/MLO* ./aPM_MPU/u-boot.img /home/qiuweibo/tftproot/
ls -l ~/tftproot/MLO*
ls -l ~/tftproot/u-boot.img
