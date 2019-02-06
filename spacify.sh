find . -name "*.c" | while read line; do expand -t 4 $line > $line.new; mv $line.new $line; done
