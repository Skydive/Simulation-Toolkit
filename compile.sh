# KMake - because a Makefile with small projects is for losers!
#!/bin/bash
cd "${0%/*}"

CPP_FILES="./src/Main.cpp"
files=$(find src -name '*.hpp')
pattern="#pragma implement "
for f in $files; do
	while read -r line; do
		if [[ $line =~ $pattern ]]; then
			line=$(echo $line | cut -d " " -f3-)
			for arg in $line; do
				CPP_FILES="$CPP_FILES $(dirname $f)/$arg.cpp"
			done
		fi
	done < $f
done
g++ $CPP_FILES -o build/main \
	-std=c++1z -Iinclude \
	$(pkg-config eigen3 sdl2 --cflags --libs) \
	-g -pipe \
	-ldl -Wl,--rpath=./
