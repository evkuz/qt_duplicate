#!/bin/bash


START_TIME="$(date)"

./qt_duplicate 2>&1 | tee outp_duplicate.txt

echo "Script started ad $START_TIME"
echo "Script  finished at $(date)"

