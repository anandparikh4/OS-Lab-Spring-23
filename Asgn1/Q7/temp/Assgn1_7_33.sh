#!/bin/bash
mkdir -p "$2"

echo "a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.q.r.s.t.u.v.w.x.y.z" | gawk "
BEGIN{
    FS = \".\"
}
{
    for(i=1; i<=NF; i++){
        file_name = \"$2\" \"/\" \$i \".txt\"
        printf(\"\") > file_name
    }
}
"

for file in "$1"/*.txt
do
    gawk "
    {
        x = tolower(substr(\$1,1,1))
        file_name = \"$2\" \"/\" x \".txt\"
        printf(\"%s\\n\",\$1) >> file_name
    }
    " "$file"
done

for file in "$2"/*.txt
do
    sort "$file" -o "$file"
done
