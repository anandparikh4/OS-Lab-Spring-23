sed -r "/\b($2)\b/s/([a-zA-Z][^a-zA-Z]*)([a-zA-Z][^a-zA-Z]*)?/\u\1\l\2/g" <$1
