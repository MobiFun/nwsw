#!/bin/bash
# Purpose: Get Component status
#
# 28Sep2011, Zarnappa Shivaraya: Created

export PICOBS_HOME=$PWD/..

#Script runs in inst_A
INST_PATH=$PICOBS_HOME/rel/inst_A

#Get the Component  status
$INST_PATH/bin/cih.exe --rootDir $INST_PATH -q -c "get 105"
