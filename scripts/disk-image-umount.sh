#!/bin/sh

pushd `dirname $0` > /dev/null
SCRIPTPATH=`pwd -P`
. ./settings.cfg
popd > /dev/null


if [[ $# -ne 1 ]]; then
    echo -e "Usage: $(basename $0) <directory>"
    exit 1
fi

if [[ $UID -ne 0 ]]; then
    echo "Permission dennied!" >&2
    exit 1
fi

a=$(realpath $1)
b=$(mount | grep $a)

if [[ $a !=  $(echo $b | cut -d " " -f 3) ]]; then
    echo "Paths are not equal" >&2
    exit 1
fi

umount $a

a=$(basename $(echo $b | cut -d " " -f 1))
a=$(echo "${a::-2}")

if [[ ! -z $a && -e "/dev/$a" ]]; then
    a=$(echo "/dev/$a")
    kpartx -d $a
    exit 0
else
    echo "Device: $a does not exist" >&2
    exit 1
fi
