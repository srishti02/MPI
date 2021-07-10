#!/bin/bash
function get_dirs()
{
  find . -maxdepth 1 -type d -name '[[:digit:]][[:digit:]]-*' -print | cut -d/ -f2-
}

function clean_all()
{
  for d in $(get_dirs) ; do
    echo "Cleaning $d..."
    (cd "$d" && make clean TOOLCHAIN=gcc && make clean TOOLCHAIN=intel && cd ..) || (echo "Aborting..." >&2 && exit 2)
  done
}

clean_all
