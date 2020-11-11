#!/usr/bin/env bash
set -euo pipefail

FAILURES=""

for EXT in ph expr
do
    for SOURCE in samples/*.$EXT
    do
        echo -e "\n==== Testing $SOURCE ====\n-> Source code"
        cat $SOURCE
        echo -e "-> Output"
        OUT=`./pherrite parse $SOURCE $EXT`
        echo $OUT
        echo -e "-> Expected result"
        EXPECTED=`cat $SOURCE.out`
        echo $EXPECTED
        if [[ "$OUT" != "$EXPECTED" ]]
        then
            FAILURES="$FAILURES $SOURCE"
        fi
    done
done

echo
if [[ -z "$FAILURES" ]]
then
    echo All tests passed!
else
    echo Failures: "$FAILURES"
fi
