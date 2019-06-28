
DIR=`pwd`
echo $DIR
FILE="$DIR/src $DIR/tests" 

find -L  $FILE -name "*.h" -o -name "*.cpp" -o -name "*.c"  -o -name "*.cc"  > cscope_my.files

cscope -bkq -i cscope_my.files  -f cscope_my.out

for i in $FILE 
do
	cp .vimrc $i
done
