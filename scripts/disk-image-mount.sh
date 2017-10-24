#!/bin/sh

pushd `dirname $0` > /dev/null
SCRIPTPATH=`pwd -P`
. ./settings.cfg
popd > /dev/null

if [[ $# -ne 2 ]]; then
    echo -e "Usage: $(basename $0) <disk_image> <directory>"
    exit 1
fi

if [[ $UID -ne 0 ]]; then
    echo "Permission dennied!"
    exit 1
fi

tmp=$(kpartx -a -v $1)

if [[ $? != 0 ]]; then
    echo "$tmp" >&2
    exit 1
fi

partition=$(echo $tmp | cut -d " " -f 3)

sleep 1

mount -t ext2 /dev/mapper/$partition $2
a=$?

if [[ $a -ne 0 ]]; then
    kpartx -d $1
fi

exit $a
