#!/usr/bin/fish
for file in in/*
  ./main < $file > res.out
  diff -q out/(basename $file .in).out res.out
  rm res.out
end
