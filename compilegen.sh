# KMake - because a Makefile with small projects is for losers!
#!/bin/bash
cd "${0%/*}"

if [ -z "$1" ]; then
	GENERATORS=$(find examples -name '*.cpp')
else
	GENERATORS="examples/$1.cpp"
fi

for g in $GENERATORS; do
	CPP_FILES="$g"
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
	g++ $CPP_FILES -fPIC -shared -o build/$(basename -s .cpp $g).so \
		-std=c++1z -Iinclude \
		$(pkg-config eigen3 --cflags --libs) \
		-ldl -g -pipe
	if [ $? -eq 0 ]; then
		echo Created $(basename -s .cpp $g).so
	fi
done
