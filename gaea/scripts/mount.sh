#!/bin/bash

[ -z "${SCRIPTDIR}" ] && export SCRIPTDIR=/etc/gaea/scripts

. ${SCRIPTDIR}/gaea.conf
. ${SCRIPTDIR}/autopart.sh

###选择分区挂载点

[ -d "${HD_1_MOUNT_DIR}" ]|| mkdir -p ${HD_1_MOUNT_DIR}

        #挂载第一分区
       mount_part ${HD_1}  ${HD_1_MOUNT_DIR} 
#        echo "mount_part ${HD_1}  ${HD_1_MOUNT_DIR}"

 mkdir -p ${HD_1_MOUNT_DIR}/${HD_2_MOUNT_DIR}
 mkdir -p ${HD_1_MOUNT_DIR}/${HD_3_MOUNT_DIR}

        #挂载第二分区
#      mount_part ${HD_2}  ${HD_1_MOUNT_DIR}/${HD_2_MOUNT_DIR} 
#        echo "mount_part ${HD_2}  ${HD_1_MOUNT_DIR}/${HD_2_MOUNT_DIR}"

        #挂载第三分区
#       mount_part ${HD_3}  ${HD_1_MOUNT_DIR}/${HD_3_MOUNT_DIR} 
#        echo "mount_part ${HD_3}  ${HD_1_MOUNT_DIR}/${HD_3_MOUNT_DIR}"
#
###挂载文件系统到相应目录

#echo "mount ${SYSTEM_FILE_DIR}/${SYSTEM_FILE} ${SYSTEM_FILE_MOUNT_DIR} -o loop"
[ -d "${SYSTEM_FILE_MOUNT_DIR}" ] || mkdir -p ${SYSTEM_FILE_MOUNT_DIR}
#mount ${SYSTEM_FILE_DIR}/${SYSTEM_FILE} ${SYSTEM_FILE_MOUNT_DIR} -o loop

