#!/bin/bash -ex
# first argument -> full path to the static app
# second argument -> full path to the shared app
# third argument -> full path to the mbox file

# sequential
$1 "$3" dev@openoffice.apache.org "01 Jan 2011 00:18:49" "15 Jan 2029 00:00:00" 1 > sequential.txt
# threaded
$2 "$3" dev@openoffice.apache.org "01 Jan 2011 00:18:49" "15 Jan 2029 00:00:00" -1 > threaded.txt
cmp sequential.txt threaded.txt
retVal=$?
rm sequential.txt threaded.txt
if [ $retVal -ne 0 ]; then
  echo "Stress test was failed"
  exit 1
fi
exit $retVal
