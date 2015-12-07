tok8x
=====

NAME
----

tok8x - convert between plaintext files and tokenised files for the TI8x series
of calculators

SYNOPSIS
--------

`tok8x` [*OPTION*]... [*-i=FILE*] [*-o=FILE*]

DESCRIPTION
-----------

`tok8x` is a utility for two-way conversion between human-readable text files
and TI's .8xp file format for storing TI-BASIC calculator programs. This format
is also used by several user-created calculator languages, so support for some
of them (currently Axe and Grammer) is included as well.

OPTIONS
-------

`-h`, `--help`
  display help dialogue

`-t`, `--token_set`=(*axe*|*basic*|*grammer*)
  define token set to be used

`-o`, `--output`=*FILENAME*
  define file to be written (defaults to stdout)

`-n`, `--name`=*NAME*
  define on-calc name (defaults to 'A')

`-a`, `--archived`
  generate archived program. valid only with 8xp output

`-p`, `--pretty`
  use "pretty" output, i.e. unicode approximations that more closely resemble
  the true appearances. valid only with 8xp input

`-s`, `--strip`
  strip excess data (comments, spaces, empty lines, etc). valid only with 8xp
  output

BUGS
----

Please report any bugs to https://github.com/shmibs/tok8x/issues

AUTHOR
------

Katherine Martin <shmibs@shmibbles.me>
