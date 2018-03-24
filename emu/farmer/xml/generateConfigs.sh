#!/bin/zsh

echo "Executing"
echo $0 $@

if [[ $#@ -ne 2 ]]; then
    echo "Need exactly two arguments. Exiting."
    echo "Usage:"
    echo "$0 RUI-to-computer_mapping.xml /directory/for/the/output"
    exit 1
fi

RUIMAP=$1
# Create a dir for the output if it doesn't exist yet
OUTDIR=$2
[ -d $OUTDIR ] || mkdir $OUTDIR

typeset -A sideName
sideName=( \
"P" "_sideP" \
"M" "_sideM" \
"B" "" \
)

#
# First we generate the XDAQ configuration (.xml) files (common to DAQ and DQM), 
# and for each we generate the corresponding FunctionManager configurtion (.duck) file:
#

# Loop over plus, minus and both sides:
for SIDE in ${(k)sideName}; do
    # Loop over all parameter combinations for DAQ:
    for WRITE in Y N; do
	for BUILD in Y N; do
	    NAME=DAQ${sideName[$SIDE]}_write${WRITE}_build${BUILD}
	    # Generate the XDAQ configuration (.xml) file (common to DAQ and DQM):
	    print "xsltproc --stringparam WRITE ${WRITE} --stringparam SIDE ${SIDE} ${0:h}/lDAQConfigGenerator.xsl ${RUIMAP} > ${OUTDIR}/${NAME}.xml"
	    xsltproc --stringparam WRITE ${WRITE} --stringparam BUILD ${BUILD} --stringparam SIDE ${SIDE} ${0:h}/lDAQConfigGenerator.xsl ${RUIMAP} > ${OUTDIR}/${NAME}.xml
	    # Generate the corresponding FunctionManager configuration (.duck) file for DAQ:
	    print "xsltproc --stringparam SIDE ${SIDE} --stringparam DIR $OUTDIR --stringparam NAME ${NAME} --stringparam FARM DAQ ${0:h}/lDAQDuckGenerator.xsl ${OUTDIR}/${NAME}.xml > ${OUTDIR}/${NAME}.duck"
	    xsltproc --stringparam SIDE ${SIDE} --stringparam DIR $OUTDIR --stringparam NAME ${NAME} --stringparam FARM DAQ ${0:h}/lDAQDuckGenerator.xsl ${OUTDIR}/${NAME}.xml > ${OUTDIR}/${NAME}.duck
	done
    done
    # Generate the corresponding FunctionManager configuration (.duck) file for DQM:
    NAME=DAQ${sideName[$SIDE]}_writeY_buildN
    print "xsltproc --stringparam SIDE ${SIDE} --stringparam DIR $OUTDIR --stringparam NAME ${NAME} --stringparam FARM DQM ${0:h}/lDAQDuckGenerator.xsl ${OUTDIR}/${NAME}.xml > ${OUTDIR}/DQM${sideName[$SIDE]}.duck"
    xsltproc --stringparam SIDE ${SIDE} --stringparam DIR $OUTDIR --stringparam NAME ${NAME} --stringparam FARM DQM ${0:h}/lDAQDuckGenerator.xsl ${OUTDIR}/${NAME}.xml > ${OUTDIR}/DQM${sideName[$SIDE]}.duck
done

# Generate the FunctionManager configuration (.duck) file for DQM display:
NAME=DAQ_writeY_buildN
print "xsltproc --stringparam DIR $OUTDIR --stringparam NAME ${NAME} --stringparam FARM DQM_Display ${0:h}/lDAQDuckGenerator.xsl ${OUTDIR}/${NAME}.xml > ${OUTDIR}/DQM_Display.duck"
xsltproc --stringparam DIR $OUTDIR --stringparam NAME ${NAME} --stringparam FARM DQM_Display ${0:h}/lDAQDuckGenerator.xsl ${OUTDIR}/${NAME}.xml > ${OUTDIR}/DQM_Display.duck
