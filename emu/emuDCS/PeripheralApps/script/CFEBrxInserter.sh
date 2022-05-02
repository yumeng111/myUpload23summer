#!/bin/zsh
# Inserts CFEB posneg and delay from an rx scan result file into a VME config XML file.
# Example:
#   CFEBrxInserter.sh MEX1_CFEBrx_2022-04-27_15-40-39.xml pc_config_1000902_2022-04-14_12-05-25_GMT.xml
# The CFEB rx scan result file should contain something like
# <scans>
#  <chamber label='ME+2/1/08' cfeb0delay='-1' cfeb0posneg='0' cfeb1delay='-1' cfeb1posneg='0' cfeb2delay='-1' cfeb2posneg='0' cfeb3delay='-1' cfeb3posneg='0' cfeb4delay='-1' cfeb4posneg='0'/>
# </scans>


if [[ $# -ne 2 ]]; then
    print "*** Error: Need exactly two arguments. Exiting."
    print "    Usage:"
    print "    $0 <CFEB_rx_scan_results> <VME_config_XML>"
    exit 1
fi

print "xsltproc --stringparam CFEBRXFILE $1 ${0:h}/CFEBrxInserter.xsl $2 > ${2:r}_modified.xml"
xsltproc --stringparam CFEBRXFILE $1 ${0:h}/CFEBrxInserter.xsl $2 > ${2:r}_modified.xml
