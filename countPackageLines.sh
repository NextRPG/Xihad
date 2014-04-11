output() {
	for dir in *
	do
		if test -d $dir 
		then
			out=$(find $dir -type f | xargs wc -l | grep total)
			echo -n $out
			echo -n '	'
			echo $dir
		fi
	done
}

output | sort -nr
