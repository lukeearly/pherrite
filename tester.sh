#!/usr/bin/env bash
set -euo pipefail

FAILURES=""

for OEXT in ir parse
do
    for EXT in ph
    do
        for SOURCE in samples/*.$EXT
        do
            echo -e "\n==== Testing $SOURCE ====\n-> Source code"
            cat $SOURCE
            echo -e "-> Output"
            OUT=`./pherrite $OEXT $SOURCE`
            echo $OUT
            echo -e "-> Expected result"
            EXPECTED=`cat $SOURCE.$OEXT`
            echo $EXPECTED
            if [[ "$OUT" != "$EXPECTED" ]]
            then
                FAILURES="$FAILURES $SOURCE.$OEXT"
            fi
        done
    done
done

echo
if [[ -z "$FAILURES" ]]
then
    echo All tests passed!
else
    echo Failures: "$FAILURES"
fi
