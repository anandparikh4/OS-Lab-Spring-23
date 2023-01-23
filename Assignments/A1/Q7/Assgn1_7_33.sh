mkdir -p "$2"
for alphabet in {a..z};do
    (echo "$(grep -ih "^$alphabet" $1/*.txt)" | sort) > $2/$alphabet.txt
done