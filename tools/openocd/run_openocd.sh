#!/bin/bash
function showHelp {
    cat << EOF
Helper program to run openocd
 -h -- help (this)
 -o -- openocd path , where openocd is compiled, just the path
 -t -- target (e.g rp2040)
 -i -- interface (e.g picoprobe)
 -e -- Elf file to be added to the target, using the interface
EOF

}

function with_elf {

    (
        echo "${OPENOCD_BIN} \
-s ${OPENOCD_TCL} \
-f ${OPENOCD_TCL}/interface/${INTERFACE}.cfg \
-f ${OPENOCD_TCL}/target/${TARGET}.cfg \
-c \"program ${ELF} verify reset init exit\"
"
        ${OPENOCD_BIN} \
            -s ${OPENOCD_TCL} \
            -f ${OPENOCD_TCL}/interface/${INTERFACE}.cfg \
            -f ${OPENOCD_TCL}/target/${TARGET}.cfg \
            -c "program ${ELF} reset  exit"

    )


}

function without_elf {

    (
        cd ${OPENOCD_PATH}
        echo "${OPENOCD_BIN} \
-s ${OPENOCD_TCL} \
-f ${OPENOCD_TCL}/interface/${INTERFACE}.cfg \
-f ${OPENOCD_TCL}/target/${TARGET}.cfg"
        out=$(${OPENOCD_BIN} -s ${OPENOCD_TCL} -f ${OPENOCD_TCL}/interface/${INTERFACE}.cfg -f ${OPENOCD_TCL}/target/${TARGET}.cfg)
    )
}

function exec_openocd {

    if [[ -z "${ELF}" ]]; then
        without_elf
    else
        with_elf
    fi



}


INTERFACE="picoprobe"
TARGET="rp2040"
ELF=
OPENOCD_BIN=

while getopts "ho:t:i:e:" args
do
    case $args in
        h)
            showHelp
            echo "FSISI"
            exit 0
            ;;
        o)
            echo "Setting ocdpath=${OPTARG}"
            export OPENOCD_PATH=${OPTARG}
            ;;
        i)
            echo "Setting inteface=${OPTARG}"
            INTERFACE=${OPTARG%\.cfg}
            echo ${INTERFACE}
            ;;
        t)
            echo "Setting target=${OPTARG}"
            TARGET=${OPTARG%\.cfg}
            ;;
        e)
            echo "Flashing elf=${OPTARG}"
            ELF=${OPTARG}
            ;;
    esac

done



if [[ -z "${ELF}" ]]; then

    echo "Not using elf"
else
    echo "Using ${ELF}"
fi
#Setting the openOCD binary path
OPENOCD_SRC=${OPENOCD_PATH}/src
OPENOCD_BIN=${OPENOCD_SRC}/openocd
OPENOCD_TCL=${OPENOCD_PATH}/tcl



exec_openocd
