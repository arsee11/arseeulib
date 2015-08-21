#!/bin/bash

for f in `ls mvc*`
do
	echo "src:"$f" dest:"${f:3}
	mv $f ${f:3}
done

