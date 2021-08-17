#!/bin/zsh
# Prepares a CSC-mapping db if necessary (for tast stands) and invokes the analyzer on the specified data files.
# Invoke it as
#   analysisScriptName analysisExeName 'dataFile1 dataFile2 ...' ['crateId1 dmbSlot1 chamberLabel1' 'crateId2 dmbSlot2 chamberLabel2' ...]

function convertToDDU(){
    # Adds DDU headers and trailers if necessary
    # Argument 1: data file name
    if [[ $(od -Ad -tx8z -w8 $1 | grep -c 8000ffff80008000) -eq 0 ]]; then
	print "No DDU trailers found in $1. Adding them and the headers."
	# First rename the original file.
	print "mv ${1} ${1:r}.odmb"
	mv ${1} ${1:r}.odmb
	# Add the DDU headers and trailers.
	print "Adding DDU headers and trailers:\n${ANALYZER:h}/add_ddu_hdr.exe ${1:r}.odmb ${1:r}.raw"
	${ANALYZER:h}/add_ddu_hdr.exe ${1:r}.odmb ${1:r}.raw
    fi
}

ulimit -c unlimited

print
print "Environment variables:"
print "    BUILD_HOME         = $BUILD_HOME"
print "    XDAQ_ROOT          = $XDAQ_ROOT"
print "    XDAQ_OS            = $XDAQ_OS"
print "    XDAQ_PLATFORM      = $XDAQ_PLATFORM"
print "    XDAQ_DOCUMENT_ROOT = $XDAQ_DOCUMENT_ROOT"
print "    ROOTSYS            = $ROOTSYS"
print "    PATH               = $PATH"
print "    LD_LIBRARY_PATH    = $LD_LIBRARY_PATH"
print "    CMSSW              = $CMSSW"
print "    DQMCONFIG          = $DQMCONFIG"
print

print "Invoked as"
print $0 "$@"


#
# Sanity check
#

[[ $# -lt 2 ]] && { print "*** Error: At least 2 arguments are needed. Exiting."; exit 1 }

ANALYZER=$1

if [[ $2 == *STEP_27* ]]; then
    DQMANALYZER=${1:h}/runEmuCSCAnalyzer.exe
    print "Looks like test 27 (high-statistics cosmics and gas gain). Will try to analyze it with $ANALYZER and $DQMANALYZER"
    [[ -x $DQMANALYZER ]] || { print "*** Error: DQM analyzer \"$DQMANALYZER\" not found or not executable. Exiting."; exit 1 }
elif [[ $2 == *STEP_40* ]]; then
    DQMANALYZER=${1:h}/runEmuCSCAnalyzer.exe
    print "Looks like test 40 (beam trigger). Will try to analyze it with $ANALYZER and $DQMANALYZER"
    [[ -x $DQMANALYZER ]] || { print "*** Error: DQM analyzer \"$DQMANALYZER\" not found or not executable. Exiting."; exit 1 }

fi

[[ -x $ANALYZER ]] || { print "*** Error: Analyzer \"$ANALYZER\" not found or not executable. Exiting."; exit 1 }

[[ -d $DQMCONFIG ]] || { print "*** Error: DQM config dir \"$DQMCONFIG\" not found. Exiting."; exit 1 }


#
# Prepare CSC-mapping database
#

print "Using sqlite3 at $(which sqlite3) of version $(sqlite3 --version)"

MAP_P5_DB=csc_map_P5.db
MAP_DB=csc_map.db
# runEmuCSCAnalyzer.exe uses the .txt map file instead of the .db file. We need to change that, too.
MAP_P5_TXT=csc_map_P5.txt
MAP_TXT=csc_map.txt


# If we have more than two arguments, those beyond the second one are mappings of the form 'crateId dmbSlot chamberLabel'
if [[ $# -gt 2 ]]; then

    cd $DQMCONFIG

    [[ -e $MAP_P5_DB ]] || { print "*** Error: P5 mapping db file $MAP_P5_DB not found. Exiting."; exit 1 }
    [[ -e $MAP_DB    ]] && { print "Removing old $DQMCONFIG/$MAP_DB"; rm $MAP_DB }

    [[ -e $MAP_P5_TXT ]] || { print "*** Error: P5 mapping txt file $MAP_P5_TXT not found. Exiting."; exit 1 }
    [[ -e $MAP_TXT    ]] && { print "Removing old $DQMCONFIG/$MAP_TXT"; rm $MAP_TXT }

    # Copy csc_map table definition from P5 db into test stand db
    # LIMIT 0 prevents the records from being copied in sqlite3 3.7.13. It prevents the table from being created altogether in sqlite3 v3.3.6 ...
    # sqlite3 -line $MAP_DB "ATTACH ${(qq)MAP_P5_DB} AS P5; CREATE TABLE main.csc_map AS SELECT * FROM P5.csc_map LIMIT 0;"
    sqlite3 -line $MAP_DB "ATTACH ${(qq)MAP_P5_DB} AS P5; CREATE TABLE main.csc_map AS SELECT * FROM P5.csc_map WHERE chamberLabel='nonexistent';"

    # Copy the .txt mapping, too
    [[ -e $DQMCONFIG/$MAP_P5_TXT ]] && cp $DQMCONFIG/$MAP_P5_TXT $DQMCONFIG/$MAP_TXT

    # DMB slot --> DMB id associative array
    typeset -A dmbSlotToId 
    dmbSlotToId=(
	3 1
	5 2
	7 3
	9 4
	11 5
	15 7
	17 8
	19 9
	21 10
    )

    # Create a chamber list
    CHAMBERARRAY=()

    # Loop over the arguments beyond the second one
    for I in {3..$#}; do
	MAPPING=( $(print ${(P)I}) )
	CRATEID=${MAPPING[1]}
	DMB=${dmbSlotToId[${MAPPING[2]}]}
	(( $DMB<6 )) && (( CSCID=$DMB )) || (( CSCID=$DMB-1 ))
	CHAMBER=${MAPPING[3]}
	CHAMBERARRAY+=$CHAMBER
	print $CRATEID $DMB $CHAMBER
	case $CHAMBER in

	    ( ME[+-]1/<1-3>/<01-36> | ME[+-]<2-4>/1/<01-18> | ME[+-]<2-4>/2/<01-36> )

	    print "Canonical chamber label ${(qq)CHAMBER} is added to test stand mapping db."
	    # Copy this chamber's row from the P5 CSC mapping db
	    sqlite3 -line $MAP_DB "ATTACH ${(qq)MAP_P5_DB} AS P5; INSERT INTO main.csc_map SELECT * FROM P5.csc_map WHERE chamberLabel=${(qq)CHAMBER};"
	    # Change the crate id, csc id, csc index and DMB id to those of the test stand in order for the analysis program to know to what chamber the data containing these ids belong to
	    sqlite3 -line $MAP_DB "UPDATE csc_map SET crateid=${(qq)CRATEID}, cscid=${(qq)CSCID}, dmb=${(qq)DMB}, cscIndex=${(qq)$(( 10*$CRATEID+$CSCID ))} WHERE chamberLabel=${(qq)CHAMBER};"

	    # Modify the .txt mapping file, too. Replace endcap, station, ring, chamber number with $CHAMBER for $CRATEID (i.e. vme) $DMB
	    # Its entries look like this:
	    #   endcap  station ring  chamber     vme    dmb     tmb    sector   cscid   ddu   input     dcc
	    #       1       1       1       1       1       2      -1       1       2      17       8     752
	    typeset -A ENDCAP
	    ENDCAP=( '+' 1 '-' 2 )
	    EC=${ENDCAP[${CHAMBER[3]}]}
	    ST=${CHAMBER[4]}
	    RG=${CHAMBER[6]}
	    CH=${CHAMBER[8,-1]}
	    print "${(l:8:)EC}${(l:8:)ST}${(l:8:)RG}${(l:8:)$((CH))}"
	    sed -i -e 's/^\([ ]\+[0-9]\+\)\{4\}\([ ]\+'${CRATEID}'[ ]\+'${DMB}'[ ]\+\)\([[:print:]]\+\)/'${(l:8:)EC}${(l:8:)ST}${(l:8:)RG}${(l:8:)$((CH))}'\2\3/g' $DQMCONFIG/$MAP_TXT
	    ;;


	    ( ME[+-]1/<1-3>/* | ME[+-]<2-4>/1/* | ME[+-]<2-4>/2/* )

	    print "Non-canonical chamber label ${(qq)CHAMBER} is added to test stand mapping db."
	    # Copy from the P5 CSC mapping db a row that corresponds to an existing chamber in this ring (say, chamber 01)
	    EXISTINGCHAMBER="${CHAMBER[1,7]}01"
	    sqlite3 -line $MAP_DB "ATTACH ${(qq)MAP_P5_DB} AS P5; INSERT INTO main.csc_map SELECT * FROM P5.csc_map WHERE chamberLabel=${(qq)EXISTINGCHAMBER};"
	    # Change the crate id, csc id, csc index and DMB id to those of the test stand in order for the analysis program to know to what chamber the data containing these ids belong to. Change the chamber label, too (to the dummy one).
	    sqlite3 -line $MAP_DB "UPDATE csc_map SET crateid=${(qq)CRATEID}, cscid=${(qq)CSCID}, dmb=${(qq)DMB}, cscIndex=${(qq)$(( 10*$CRATEID+$CSCID ))}, chamberLabel=${(qq)CHAMBER} WHERE chamberLabel=${(qq)EXISTINGCHAMBER};"

	    # Modify the .txt mapping file, too. Replace endcap, station, ring, chamber number with $EXISTINGCHAMBER for $CRATEID (i.e. vme) $DMB
	    # Its entries look like this:
	    #   endcap  station ring  chamber     vme    dmb     tmb    sector   cscid   ddu   input     dcc
	    #       1       1       1       1       1       2      -1       1       2      17       8     752
	    typeset -A ENDCAP
	    ENDCAP=( '+' 1 '-' 2 )
	    EC=${ENDCAP[${CHAMBER[3]}]}
	    ST=${CHAMBER[4]}
	    RG=${CHAMBER[6]}
	    CH=${EXISTINGCHAMBER[8,-1]}
	    print "${(l:8:)EC}${(l:8:)ST}${(l:8:)RG}${(l:8:)$((CH))}"
	    sed -i -e 's/^\([ ]\+[0-9]\+\)\{4\}\([ ]\+'${CRATEID}'[ ]\+'${DMB}'[ ]\+\)\([[:print:]]\+\)/'${(l:8:)EC}${(l:8:)ST}${(l:8:)RG}${(l:8:)$((CH))}'\2\3/g' $DQMCONFIG/$MAP_TXT
	    ;;


	    ( * )

	    print "*** Error: Malformed chamber label ${(qq)CHAMBER}. Exiting."
	    print '           Note that all chamber labels must be canonical up to the ring. A well-formed non-canonical chamber label example: ME+4/2/_239_'
	    exit 1
	    ;;

	esac
    done

else
    # No explicit chamber mappings specified. Use the canonical one of P5.
    [[ -e $DQMCONFIG/$MAP_P5_DB  ]] && cp $DQMCONFIG/$MAP_P5_DB  $DQMCONFIG/$MAP_DB
    [[ -e $DQMCONFIG/$MAP_P5_TXT ]] && cp $DQMCONFIG/$MAP_P5_TXT $DQMCONFIG/$MAP_TXT
fi


#
# Run analyzer
#

# The first argument is a space-separated list of data files given in the format host:file
DATAFILES=( $(print $2) )
for DATAFILE in $DATAFILES; do
    DATAFQHOSTNAME=${DATAFILE%:*}
    DATAHOSTNAME=${DATAFQHOSTNAME%.*}
    DATANETNAME=${DATAFQHOSTNAME#*.}
    DATAPATHNAME=${DATAFILE#*:}
    print "DATAFQHOSTNAME =$DATAFQHOSTNAME"
    print "DATAHOSTNAME   =$DATAHOSTNAME"
    print "DATANETNAME    =$DATANETNAME"
    print "DATAPATHNAME   =$DATAPATHNAME"
    if [[ $DATANETNAME == "cms" ]]; then
	# In the .cms network, convert 
	# csc-daq03.cms:/data/csc_00000001_EmuRUI09_Test_13_000_130924_171005_UTC.raw to
	# /cmscsc/csc-daq03/csc_00000001_EmuRUI09_Test_13_000_130924_171005_UTC.raw
	DATAPATHNAME=/cmscsc/${DATAHOSTNAME}/${DATAPATHNAME#/data/}
    fi
    print "DATAPATHNAME   =$DATAPATHNAME"

    convertToDDU $DATAPATHNAME

    RESULTSTOPDIR=${DATAPATHNAME:h}/Tests_results
    [[ -d $RESULTSTOPDIR ]] || mkdir -p $RESULTSTOPDIR

    if [[ $DATAPATHNAME == *STEP_27* ]]; then
	# For test 27, the high-stat cosmics, first produce the .root file, then the plots. Finally, list the chambers in chambers.txt for linkToChambers.sh to know which chambers' data these results contain.
	print "cd $RESULTSTOPDIR && mkdir -p Test_27_Cosmics && cd Test_27_Cosmics && $DQMANALYZER $DATAPATHNAME && $DQMANALYZER ${DATAPATHNAME:t:r}.root && cd ${DATAPATHNAME:t:r}.plots && { print $CHAMBERARRAY > chambers.txt }"
	cd $RESULTSTOPDIR && mkdir -p Test_27_Cosmics && cd Test_27_Cosmics && $DQMANALYZER $DATAPATHNAME && $DQMANALYZER ${DATAPATHNAME:t:r}.root && cd ${DATAPATHNAME:t:r}.plots && { print $CHAMBERARRAY > chambers.txt }
    elif [[ $DATAPATHNAME == *STEP_40* ]]; then
	# For test 40, the beam trigger, first produce the .root file, then the plots. Finally, list the chambers in chambers.txt for linkToChambers.sh to know which chambers' data these results contain.
	print "cd $RESULTSTOPDIR && mkdir -p Test_40_Beam && cd Test_40_Beam && $DQMANALYZER $DATAPATHNAME && $DQMANALYZER ${DATAPATHNAME:t:r}.root && cd ${DATAPATHNAME:t:r}.plots && { print $CHAMBERARRAY > chambers.txt }"
	cd $RESULTSTOPDIR && mkdir -p Test_40_Beam && cd Test_40_Beam && $DQMANALYZER $DATAPATHNAME && $DQMANALYZER ${DATAPATHNAME:t:r}.root && cd ${DATAPATHNAME:t:r}.plots && { print $CHAMBERARRAY > chambers.txt }
    fi

    # Run the calibration analysis (i.e. $ANALYZER). 
    # For STEP_*, this will do the gas gain analysis, which is only meaningful for normal cosmics (STEP_27), not for single layer cosmics (STEP_27s) or beam-triggered runs (STEP_40).
    if [[ $DATAPATHNAME != *STEP_27s* && $DATAPATHNAME != *STEP_40* ]]; then
	print "cd $RESULTSTOPDIR && $ANALYZER $DATAPATHNAME"
	cd $RESULTSTOPDIR && $ANALYZER $DATAPATHNAME
    fi

    # Generate web page for easier browsing. Not for tests 27 and 40, though, for those the DQM analyzer does it already:
    print "Results' dir:"
    print $RESULTSTOPDIR/Test_*/${DATAPATHNAME:t:r}.plots(/Nom[1])
    RESULTSDIR=$( print $RESULTSTOPDIR/Test_*/${DATAPATHNAME:t:r}.plots(/Nom[1]) )
    if [[ ${#RESULTSDIR} -gt 0 && $RESULTSDIR != *Test_27* && $RESULTSDIR != *Test_40* ]]; then
	if [[ -x ${0:h}/generateIndexHTML.sh ]]; then
	    print "Generating web page with ${0:h}/generateIndexHTML.sh $RESULTSDIR"
	    ${0:h}/generateIndexHTML.sh $RESULTSDIR
	else
	    print "** Warning: Web page generator script ${0:h}/generateIndexHTML.sh not found."
	fi
    fi

    if [[ -x ${0:h}/linkToChambers.sh ]]; then
	print "Generating chamber-oriented directory structure in $RESULTSTOPDIR"
	${0:h}/linkToChambers.sh $RESULTSTOPDIR $RESULTSTOPDIR
    else
	print "** Warning: Chamber-oriented directory generator script ${0:h}/linkToChambers.sh not found."
    fi
done
