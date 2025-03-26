#!/bin/zsh
# Inserts parameter values from a scan result file into a VME config XML file.
# Example:
#   ScanResultsInserter.sh MEX1_CFEBrx_2022-04-27_15-40-39.xml pc_config_1000902_2022-04-14_12-05-25_GMT.xml
# The (for example CFEB rx) scan result file should contain something like
# <scans>
#  <chamber label='ME+2/1/08' cfeb0delay='-1' cfeb0posneg='0' cfeb1delay='-1' cfeb1posneg='0' cfeb2delay='-1' cfeb2posneg='0' cfeb3delay='-1' cfeb3posneg='0' cfeb4delay='-1' cfeb4posneg='0'/>
# </scans>


if [[ $# -ne 2 ]]; then
    print "*** Error: Need exactly two arguments. Exiting."
    print "    Usage:"
    print "    $0 <scan_results_file> <VME_config_XML>"
    exit 1
fi

print "xsltproc --stringparam SCANRESULTSFILE $1 ${0:h}/ScanResultsInserter.xsl $2 > ${2:r}_modified.xml"
xsltproc --stringparam SCANRESULTSFILE $1 ${0:h}/ScanResultsInserter.xsl $2 > ${2:r}_modified.xml
