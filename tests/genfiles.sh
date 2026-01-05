#!/usr/bin/env bash

function gentestfiles() {
outdir=$1

# test1.txt
mkdir -p $outdir
cat >$outdir/test1.txt << EOL
helo
EOL

# test2.txt
cat >$outdir/test2.txt << EOL
booger
face
toodaloo
EOL

# test3.txt
cat >$outdir/test3.txt << EOL
when are we going
to the malll
mist
er
EOL

# test4.txt (larger file)
cat >$outdir/test4.txt << EOL

$(for i in {1..500}; do
  printf -- '.%.0s' {1..100}; echo ""
done)
EOL
}

# Entry
gentestfiles $@
