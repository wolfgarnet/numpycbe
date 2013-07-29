for(i=0; i<n; i++, i1+=is1, i2+=is2, op+=os) {
    *((Bool *)op)=*((@typ@ *)i1) @OP@ *((@typ@ *)i2);