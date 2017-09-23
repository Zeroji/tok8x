# CURRENTLY UNMAINTAINTED

and not being worked on. interest == lost since not using those calculators any
more. poke me if you want to take over or something; otherwise it'll just be
sitting here

tok8x
=====

NAME
----

tok8x - convert between plaintext files and tokenised files for the TI8x series
of calculators

## SYNOPSIS

`tok8x` [<var>OPTION</var>]... [<var>FILE</var>]...<br>
`tok8x` -i [-p] [-t=<var>token set</var>] <var>TOKEN</var>...

## DESCRIPTION

**tok8x** is a utility for two-way conversion between human-readable text files
and TI's .8xp file format for storing TI-BASIC calculator programs. This format
is also used by several user-created calculator languages, so support for some
of them (currently Axe and Grammer) is included as well.

By default, input is read for the designated files, or <var>stdin</var> if no file names
are provided, converted based on the type of the input data (if input is in 8xp
format, output will be plain textl if input is plaintext, output will be 8xp),
and written to the designated output file, or <var>stdout</var> if none is provided. If
options are passed which do not apply to the current input and output formats,
they will be ignored.

If the `-i` option is designated, arguments should be representations of tokens
provided either as their string names (i.e. ">DMS", "Asm(") or their hexadecimal
values (i.e. "01", "BB6A").

## OPTIONS
  * `-h`, `--help`:
    display help dialogue

  * `-i`, `--info`:
    rather than converting files, return information on one or more tokens
    provided either as their string names (i.e. ">DMS", "Asm(") or their
    hexadecimal values (i.e. "01", "BB6A").

  * `-t`, `--token_set`=(<var>axe</var>|<var>basic</var>|<var>grammer</var>):
    define token set to be used

  * `-o`, `--output`=<var>FILE</var>:
    define file to be written (defaults to stdout)

  * `-n`, `--name`=<var>NAME</var>:
    define on-calc name (defaults to 'A'. affects only 8xp output)

  * `-a`, `--archived`:
    generate archived program (affects only 8xp output)

  * `-p`, `--pretty`:
    use "pretty" output, i.e. unicode approximations that more closely resemble
    the true appearances (affects plaintext output or printing information with
    -i)

  * `-s`, `--safe`:
    when outputting to plain text, insert backslashes before single-character
    strings which could be misconstrued as the beginning of a token name to
    ensure they will remain distinct tokens if converted back to 8xp (affects
    only plaintext output)

  * `-c`, `--condense`:
    strip excess whitespace to reduce on-calc size (affects only 8xp output)

  * `-e`, `--expand`:
    insert whitespace to make output more easily human readable (affects only
    plaintext output)

## BUGS

Please report any bugs to https://github.com/shmibs/tok8x/issues

## AUTHOR

Katherine Martin <shmibs@shmibbles.me>

