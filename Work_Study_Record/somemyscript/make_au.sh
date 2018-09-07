#!/bin/sh
cd dl
if [ ! -x "arcadyan-utility-0.5" ] ; then 
	echo "arcadyan-utility-0.5 not exit ,mkdir it"
	mkdir -p arcadyan-utility-0.5
fi
cp -r ../../../arcadyan-utility_2013/* arcadyan-utility-0.5
tar zcf arcadyan-utility-0.5.tar.gz arcadyan-utility-0.5
cd ../
make package/arcadyan-utility/clean V=99
make package/arcadyan-utility/compile V=99
 
