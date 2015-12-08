#!/bin/bash
ronn --roff tok8x.1.ronn
ronn --markdown tok8x.1.ronn

{
cat << EOF
tok8x
=====

NAME
----

tok8x - convert between plaintext files and tokenised files for the TI8x series
of calculators
EOF
tail -n +3 tok8x.1.markdown | head -n -9
} > ../README.md

rm tok8x.1.markdown
