#!/bin/bash
MAIN=out/foxes-rabbits
OUTPUT_DIR=out/output
DIFF_DIR=out/diff
INSTANCES_DIR=larger-instances/
# larger-instances for larger runtime
# initial-instances for normal runtime


make clean
make

mkdir -p ${OUTPUT_DIR}
mkdir -p ${DIFF_DIR}

array=()
while IFS= read -r -d $'\0'; do
  array+=("$REPLY")
done < <(find "${INSTANCES_DIR}" -type f -name "*.out" -print0)


for file in "${array[@]}"; do
  CURRENT_FILE="${file##*/}"
  FILE_NO_EXTENSION="${CURRENT_FILE%.*}"
  
  PRE_COMMAND="${CURRENT_FILE//-/ }"
  R_COMMAND="${PRE_COMMAND%.*}"
  COMMAND="${R_COMMAND:1}"

  echo ${COMMAND} RESULT:

  # run the main
  ${MAIN} ${COMMAND} > "${OUTPUT_DIR}"/"${CURRENT_FILE}" 

  # generate diff
  diff "${OUTPUT_DIR}"/"${CURRENT_FILE}" "${file}" > "${DIFF_DIR}"/"${FILE_NO_EXTENSION}".diff
  
done
