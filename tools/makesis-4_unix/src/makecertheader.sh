#!/bin/sh

echo "static const char selfsignedCer[] = " > selfsigned.cer.h
cat selfsigned.cer | sed 's/^/"/' | sed 's/$/\\n"/' >> selfsigned.cer.h
echo ";" >> selfsigned.cer.h
echo "static const char selfsignedKey[] = " > selfsigned.key.h
cat selfsigned.key | sed 's/^/"/' | sed 's/$/\\n"/' >> selfsigned.key.h
echo ";" >> selfsigned.key.h

