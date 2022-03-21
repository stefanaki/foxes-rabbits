#!/bin/bash
MAIN=out/foxes-rabbits
OUTPUT_DIR=out/output
DIFF_DIR=out/diff
INITIAL_INSTANCES_DIR=initial-instances/


make clean
make

mkdir -p ${OUTPUT_DIR}
mkdir -p ${DIFF_DIR}

array=()
while IFS= read -r -d $'\0'; do
  array+=("$REPLY")
done < <(find "${INITIAL_INSTANCES_DIR}" -type f -name "*.out" -print0)


for file in "${array[@]}"; do
  CURRENT_FILE="${file##*/}"
  FILE_NO_EXTENSION="${CURRENT_FILE%.*}"
  
  PRE_COMMAND="${CURRENT_FILE//-/ }"
  R_COMMAND="${PRE_COMMAND%.*}"
  COMMAND="${R_COMMAND:1}"

  # run the main
  ${MAIN} ${COMMAND} > "${OUTPUT_DIR}"/"${CURRENT_FILE}" 

  # generate diff
  diff --color "${OUTPUT_DIR}"/"${CURRENT_FILE}" "${INITIAL_INSTANCES_DIR}" > "${DIFF_DIR}"/"${FILE_NO_EXTENSION}".diff
  
done
