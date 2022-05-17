PATH_PATTERNS=./data/patts
PATH_TEXT=./../english
FILE_PATH=./data2
TEXT_PATTERN="mdate"

# Create index
# Search
for filename in `ls $FILE_PATH | grep -v '\.idx'`; do
  echo $filename
  start_time=`date +%s`;
  time ./bin/ipmt index "$FILE_PATH/$filename"
  end_time=`date +%s`
  exec_time=$((end_time-start_time))
  echo $exec_time
done

# Search
for filename in `ls $FILE_PATH | grep '\.idx'`; do
  echo $filename
  start_time=`date +%s`;
  # time ./bin/ipmt search $TEXT_PATTERN "$FILE_PATH/$filename" -c
  end_time=`date +%s`
  exec_time=$((end_time-start_time))
  echo $exec_time
done
