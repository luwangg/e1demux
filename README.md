e1demux
=======

Command-line utility for de-multiplexing of E1 (2.048 Mbit/s, ITU-T
G.704) stream.

Program receives E1 stream on stdin, catches frame alignment signal and puts
timeslots' data to specified files.  User is able to pass a file for each
timeslot.  If not file specified then this timeslot is skipped,

How it works
============

At first program achieves byte alignment. It is required because
program input can be bit-shifted. Byte aligned data is checked for 
frame alignment and then data is ready for de-multiplexing.

Number of received frames can be limited or continuous. 

Usage, options
==============

Usage:
 
    ./e1demux [OPTION]

Options:

  * -F <framing>, --framing=<framing> Sets E1 framing: double-frame or
      multi-frame. Values are: 2 for double-frame, 16 for multi-frame. 
      Default = 2
  * -c <on/off>, --crc4=<on/off> Enables of disables insertion of CRC-4
       bits into SYNC timeslot. Valid only with framing=16.
       Default = on.
  * -n <num-of-frames>, --num-of-frames=<integer> Sets number of frames
       to generate. Default value is 0 (unlimited). 
  * -t <tsnum:file>, --timeslot=<tsnum:file> Sets a file for a specified
       timeslot. File will be written each time timeslot data is arrived.
       Example: -t 1:/dev/null. If file write gives an error then 
       this file is skipped.
  * -v, --verbose print more
  * -V, --version print version
  * -h, --help print this message

Currently, options -F, -c, -n is not supported. They are reserved for
the future.

Build
=====

    $ make

Examples
========

Examples is base on usage of 'e1gen' program.
You can get it at https://github.com/yapavelkurochkin/e1gen

Passing 1st timeslot to the file (/tmp/ts1):

    $ e1gen -n 100 | ./e1demux -t 1:/tmp/ts1

Passing 1st timeslot data to the program (using bash process substition):

    $ e1gen -n 100 | ./e1demux -t 1:>(gzip -c > /tmp/1.gz) 

Transferring data with help of 'e1gen' and 'e1demux' using bash process
substitution:

    $ e1gen -t 1:<(cat some_stream) | ./e1demux -t 1:>(gzip -c > /tmp/stream.gz) 

