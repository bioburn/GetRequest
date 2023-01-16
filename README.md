# Aptitude Project

Requirements

Write a command-line program that takes one argument (a URL) and prints the resource at
that URL to standard output, in the following way:
● If the resource is text, print one line of that text per second, preceded by line numbers,
then exit.
● If the resource is not text, print 16 bytes of the resource per line, in hexadecimal with a
space between each byte, preceded by file offset in hex, one line per second (similar to
the output of "od -t x1" on Linux)
● While the lines are printing, if the user presses the "+" key, speed up the display. If he
presses "-", slow it down. If he presses the spacebar, stop the display, and restart it when
he presses the spacebar again. Do not echo these key presses to the output.
● The program must respond immediately to key presses while printing.
● Do not use threads. Otherwise, you may use whatever libraries are available and useful on
your system, but bonus points using fewer libraries.


run with ./GetRequest <UrlHere>
if no argument is given, example.com is used

checks whether or not a resource is text by the content-type header
text/html, utf-8 /utf-16
not all resources will present this information however, so it may fail


prints 1 line per second as default
pressing + will increase the lines printed per second by 1
pressing - will decrease the lines printed per second to a minimum of 1 line per second

program exits upon completion of printing the contents

Uses the CURL C library
Developed in visual studio

