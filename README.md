tok8x
=====

NAME
----

tok8x - convert between plaintext files and tokenised files for the TI8x series
of calculators

## SYNOPSIS

`tok8x` [<var>OPTION</var>]... [-i <var>FILE</var>] [-o <var>FILE</var>]

## DESCRIPTION

**tok8x** is a utility for two-way conversion between human-readable text files
and TI's .8xp file format for storing TI-BASIC calculator programs. This format
is also used by several user-created calculator languages, so support for some
of them (currently Axe and Grammer) is included as well.

## OPTIONS
  * `-h`, `--help`:
    display help dialogue

  * `-t`, `--token_set`=(<var>axe</var>|<var>basic</var>|<var>grammer</var>):
    define token set to be used

  * `-i`, `--input`=<var>FILE</var>:
    define file to be read (defaults to stdin)

  * `-o`, `--output`=<var>FILE</var>:
    define file to be written (defaults to stdout)

  * `-n`, `--name`=<var>NAME</var>:
    define on-calc name (defaults to 'A'. valid only with 8xp output)

  * `-a`, `--archived`:
    generate archived program (valid only with 8xp output)

  * `-p`, `--pretty`:
    use "pretty" output, i.e. unicode approximations that more closely resemble
    the true appearances (valid only with 8xp input)

  * `-s`, `--strip`:
    strip excess data (comments, spaces, empty lines, etc. valid only with 8xp
    output)

## BUGS

Please report any bugs to https://github.com/shmibs/tok8x/issues

## AUTHOR

Katherine Martin <shmibs@shmibbles.me>

