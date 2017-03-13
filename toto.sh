for f in $(find . -type f -name "*.[ch]");
do
expand -t4 $f >! /tmp/toto ; cat /tmp/toto >! $f;
done
