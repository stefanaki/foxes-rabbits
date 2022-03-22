#!/bin/bash
MAIN=./foxes-rabbits
DEBUG=1 # set the debug variable here [0 or 1] -> output will have or not the maps
OUTPUT_DIR=out/output
DIFF_DIR=out/diff
if [ "$1" != "large-only" ]; then INITIAL_INSTANCES_DIR=initial-instances/; fi
if [ "$1" = "all" -o "$1" = "large-only" ]; then LARGER_INSTANCES_DIR=larger-instances/; fi

make clean
make

mkdir -p ${OUTPUT_DIR}
mkdir -p ${DIFF_DIR}

array=()
while IFS= read -r -d $'\0'; do
  array+=("$REPLY")
done < <(find ${INITIAL_INSTANCES_DIR} ${LARGER_INSTANCES_DIR} -type f -name "*.out" -print0)


for file in "${array[@]}"; do
  CURRENT_FILE="${file##*/}"
  FILE_NO_EXTENSION="${CURRENT_FILE%.*}"
  
  PRE_COMMAND="${CURRENT_FILE//-/ }"
  R_COMMAND="${PRE_COMMAND%.*}"
  COMMAND="${R_COMMAND:1}"

  echo ${COMMAND} ${DEBUG} RESULT:

  # run the main with 1 for debug mode
  ${MAIN} ${COMMAND} ${DEBUG} > "${OUTPUT_DIR}"/"${CURRENT_FILE}" 

  # generate diff
  diff "${OUTPUT_DIR}"/"${CURRENT_FILE}" "${file}" > "${DIFF_DIR}"/"${FILE_NO_EXTENSION}".diff
  
done
