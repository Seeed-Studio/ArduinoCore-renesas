#!/bin/bash

CORE_PATH=/tmp/aaa
TARGET=UNO_R4

LIBRARY=`find . | grep "\.a$"`

echo Copying ${LIBRARY} to ${CORE_PATH}/variants/${TARGET}/libs/libfsp.a
cp ${LIBRARY} ${CORE_PATH}/variants/${TARGET}/libs/libfsp.a

LINKER_SCRIPTS=`find . | grep "\.ld$"`
cp ${LINKER_SCRIPTS} ${CORE_PATH}/variants/${TARGET}/

FILE_MK=`find . | grep subdir.mk | head -n1`

CCCOMMAND=`cat $FILE_MK | grep \$\(file | cut -f2 -d","`

echo $CCCOMMAND

DEFINES=()
INCLUDES=()
FLAGS=()

OIFS=$IFS
IFS=' '
tokens=$CCCOMMAND
for x in $tokens
do
    if [[ $x == -D* ]]; then DEFINES+=( $x ); fi
    if [[ $x == -I* ]]; then INCLUDES+=( $x ); fi
    if [[ $x == -m* ]]; then FLAGS+=( $x ); fi
    if [[ $x == -f* ]]; then FLAGS+=( $x ); fi
    if [[ $x == -W* ]]; then FLAGS+=( $x ); fi
done
IFS=$OIFS

for value in "${DEFINES[@]}"
do
    echo $value >> ${CORE_PATH}/variants/${TARGET}/defines.txt
done

for value in "${INCLUDES[@]}"
do
    INCLUDE_PATH=`echo $value | cut -f2 -d"\"" | cut -f1 -d"\""`
    echo $INCLUDE_PATH
    # temporarily, copy everything staring with "ra_" in variant/includes/ , everything with ra in core folder
    if [[ $INCLUDE_PATH == $PWD/ra_* ]]; then
        INCLUDE_PATH_REL=${INCLUDE_PATH#"$PWD/"}
        cp -r --parent $INCLUDE_PATH_REL ${CORE_PATH}/variants/${TARGET}/includes/
        echo "\"-I{build.variant.path}/$INCLUDE_PATH_REL\"" >> ${CORE_PATH}/variants/${TARGET}/includes.txt
    else
        if [[ $INCLUDE_PATH == $PWD/ra* ]]; then
            INCLUDE_PATH_REL=${INCLUDE_PATH#"$PWD/"}
            cp -r --parent $INCLUDE_PATH_REL ${CORE_PATH}/cores/arduino/fsp/
            echo "\"-I{build.core.path}/$INCLUDE_PATH_REL\"" >> ${CORE_PATH}/variants/${TARGET}/includes.txt

        fi
    fi
    #rel_path=`echo $value | sed -e "s#-I$PWD#-iwithprefixbefore/fsp#g"`
    #echo $rel_path >> ${CORE_PATH}/variants/${TARGET}/includes.txt

    # TODO: check how many include folders are generated and if it makes sense to track them manually
done

for value in "${FLAGS[@]}"
do
    echo $value >> ${CORE_PATH}/variants/${TARGET}/cflags.txt
    echo $value >> ${CORE_PATH}/variants/${TARGET}/cxxflags.txt
done