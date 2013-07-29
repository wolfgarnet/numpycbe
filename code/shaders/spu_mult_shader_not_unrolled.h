uint32_t setNumberOfBlockJobs[] __attribute__((aligned(128))) = 
{
    0x42607e10, // ila $16,c0fc <main+0xadec> 
    0x34004208, // lqd $8,16($4) 
    0x40800e0d, // il $13,28 # 1c 
    0x35900000, // hbrp c <setNumberOfBlockJobs+0xc>,$0 
    0x3400428b, // lqd $11,16($5) 
    0x4260100c, // ila $12,c020 <main+0xad10> 
    0x4089c402, // il $2,5000 # 1388 
    0x3f820411, // rotqbyi $17,$8,8 
    0x4020007f, // nop $127 
    0x3f830586, // rotqbyi $6,$11,12 
    0x1804088f, // a $15,$17,$16 
    0x3884088e, // lqx $14,$17,$16 
    0x04000888, // ori $8,$17,0 
    0x38834889, // lqx $9,$17,$13 
    0x3883088a, // lqx $10,$17,$12 
    0x3ac30887, // cwx $7,$17,$12 
    0x3ec3088b, // cwd $11,12($17) 
    0x35900000, // hbrp 44 <setNumberOfBlockJobs+0x44>,$0 
    0x3b83c704, // rotqby $4,$14,$15 
    0xb1428107, // shufb $10,$2,$10,$7 
    0x7f000300, // heqi $0,$6,0 
    0x12000492, // hbrr 9c <setNumberOfBlockJobs+0x9c>,78 <setNumberOfBlockJobs+0x78> # 78 
    0x54a00311, // clz $17,$6 
    0x54a0020e, // clz $14,$4 
    0x4080008f, // il $15,1 
    0x32800013, // fsmbi $19,0 
    0x08044711, // sf $17,$14,$17 
    0x04000205, // ori $5,$4,0 
    0x0b64478f, // shl $15,$15,$17 
    0x0b64430e, // shl $14,$6,$17 
    0x0823c992, // or $18,$19,$15 
    0x3f3fc78f, // rotqmbii $15,$15,-1 
    0x58014710, // clgt $16,$14,$5 
    0x00200000, // lnop 
    0x08014711, // sf $17,$14,$5 
    0x3f3fc70e, // rotqmbii $14,$14,-1 
    0x8264c910, // selb $19,$18,$19,$16 
    0x00200000, // lnop 
    0x80a14890, // selb $5,$17,$5,$16 
    0x217ffb8f, // brnz $15,78 <setNumberOfBlockJobs+0x78> # 78 
    0x5800c282, // clgt $2,$5,$3 
    0xb122498b, // shufb $9,$19,$9,$11 
    0x28834409, // stqx $9,$8,$13 
    0x20000285, // brz $5,c0 <setNumberOfBlockJobs+0xc0> # c0 
    0x1c004984, // ai $4,$19,1 
    0x20000182, // brz $2,c0 <setNumberOfBlockJobs+0xc0> # c0 
    0xb062420b, // shufb $3,$4,$9,$11 
    0x28834403, // stqx $3,$8,$13 
    0x2883040a, // stqx $10,$8,$12 
    0x35000000, // bi $0 
};

#define SETNUMBEROFBLOCKJOBS_SIZE 200


uint32_t resetValuesInResultBuffer[] __attribute__((aligned(128))) = 
{
    0x7c00c204, // ceqi $4,$4,3 
    0x21000b84, // brnz $4,128 <resetValuesInResultBuffer+0x60> # 128 
    0x40800a14, // il $20,20 # 14 
    0x34004196, // lqd $22,16($3) 
    0x40800005, // il $5,0 
    0x12000490, // hbrr 11c <resetValuesInResultBuffer+0x54>,100 <resetValuesInResultBuffer+0x38> # 100 
    0x40800007, // il $7,0 
    0x40880006, // il $6,4096 # 1000 
    0x4020007f, // nop $127 
    0x3f820b15, // rotqbyi $21,$22,8 
    0x1c050a92, // ai $18,$21,20 # 14 
    0x38850a91, // lqx $17,$21,$20 
    0x3b848890, // rotqby $16,$17,$18 
    0x04000804, // ori $4,$16,0 
    0x1cffc306, // ai $6,$6,-1 
    0x38814218, // lqx $24,$4,$5 
    0x3ac14219, // cwx $25,$4,$5 
    0xb2e60399, // shufb $23,$7,$24,$25 
    0x4020007f, // nop $127 
    0x28814217, // stqx $23,$4,$5 
    0x1c010285, // ai $5,$5,4 
    0x217ffc86, // brnz $6,100 <resetValuesInResultBuffer+0x38> # 100 
    0x4020007f, // nop $127 
    0x35000000, // bi $0 
    0x4080040a, // il $10,8 
    0x3400418c, // lqd $12,16($3) 
    0x40800005, // il $5,0 
    0x12000498, // hbrr 194 <resetValuesInResultBuffer+0xcc>,158 <resetValuesInResultBuffer+0x90> # 158 
    0x40800007, // il $7,0 
    0x40880006, // il $6,4096 # 1000 
    0x4020007f, // nop $127 
    0x3f82060b, // rotqbyi $11,$12,8 
    0x1c020583, // ai $3,$11,8 
    0x38828588, // lqx $8,$11,$10 
    0x3b80c402, // rotqby $2,$8,$3 
    0x04000104, // ori $4,$2,0 
    0x1cffc306, // ai $6,$6,-1 
    0x3881420e, // lqx $14,$4,$5 
    0x3ac1420f, // cwx $15,$4,$5 
    0xb1a3838f, // shufb $13,$7,$14,$15 
    0x4020007f, // nop $127 
    0x2881420d, // stqx $13,$4,$5 
    0x1c010285, // ai $5,$5,4 
    0x25000006, // biz $6,$0 
    0x1cffc306, // ai $6,$6,-1 
    0x3881420e, // lqx $14,$4,$5 
    0x3ac1420f, // cwx $15,$4,$5 
    0xb1a3838f, // shufb $13,$7,$14,$15 
    0x4020007f, // nop $127 
    0x2881420d, // stqx $13,$4,$5 
    0x1c010285, // ai $5,$5,4 
    0x217ff886, // brnz $6,158 <resetValuesInResultBuffer+0x90> # 158 
    0x35000000, // bi $0 
    0x00200000, // lnop 
};

#define RESETVALUESINRESULTBUFFER_SIZE 216


uint32_t printValues[] __attribute__((aligned(128))) = 
{
    0x40800240, // il $64,4 
    0x24ffc0d0, // stqd $80,-16($1) 
    0x1c01023f, // ai $63,$4,4 
    0x24ff80d1, // stqd $81,-32($1) 
    0x040001d1, // ori $81,$3,0 
    0x3890023e, // lqx $62,$4,$64 
    0x40801083, // il $3,33 # 21 
    0x24004080, // stqd $0,16($1) 
    0x24ff0081, // stqd $1,-64($1) 
    0x1cf00081, // ai $1,$1,-64 
    0x4020007f, // nop $127 
    0x3b8fdf50, // rotqby $80,$62,$63 
    0x35202800, // bisl $0,$80 
    0x3580280a, // hbr 1fc <printValues+0x5c>,$80 
    0x4260343c, // ila $60,c068 <main+0xad58> 
    0x340068bd, // lqd $61,16($81) 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x3f821ebb, // rotqbyi $59,$61,8 
    0x180f1dba, // a $58,$59,$60 
    0x388f1db9, // lqx $57,$59,$60 
    0x3b8e9c83, // rotqby $3,$57,$58 
    0x35202800, // bisl $0,$80 
    0x4020007f, // nop $127 
    0x3580280a, // hbr 22c <printValues+0x8c>,$80 
    0x42602e37, // ila $55,c05c <main+0xad4c> 
    0x340068b8, // lqd $56,16($81) 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x3f821c36, // rotqbyi $54,$56,8 
    0x180ddb35, // a $53,$54,$55 
    0x388ddb34, // lqx $52,$54,$55 
    0x3b8d5a03, // rotqby $3,$52,$53 
    0x35202800, // bisl $0,$80 
    0x4020007f, // nop $127 
    0x3580280a, // hbr 25c <printValues+0xbc>,$80 
    0x42603032, // ila $50,c060 <main+0xad50> 
    0x340068b3, // lqd $51,16($81) 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x3f8219b1, // rotqbyi $49,$51,8 
    0x180c98b0, // a $48,$49,$50 
    0x388c98af, // lqx $47,$49,$50 
    0x3b8c1783, // rotqby $3,$47,$48 
    0x35202800, // bisl $0,$80 
    0x4020007f, // nop $127 
    0x3580280a, // hbr 28c <printValues+0xec>,$80 
    0x4260322d, // ila $45,c064 <main+0xad54> 
    0x340068ae, // lqd $46,16($81) 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x3f82172c, // rotqbyi $44,$46,8 
    0x180b562b, // a $43,$44,$45 
    0x388b562a, // lqx $42,$44,$45 
    0x3b8ad503, // rotqby $3,$42,$43 
    0x35202800, // bisl $0,$80 
    0x4020007f, // nop $127 
    0x3580280a, // hbr 2bc <printValues+0x11c>,$80 
    0x42605c28, // ila $40,c0b8 <main+0xada8> 
    0x340068a9, // lqd $41,16($81) 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x3f8214a7, // rotqbyi $39,$41,8 
    0x180a13a6, // a $38,$39,$40 
    0x388a13a5, // lqx $37,$39,$40 
    0x3b899283, // rotqby $3,$37,$38 
    0x35202800, // bisl $0,$80 
    0x4020007f, // nop $127 
    0x3580280a, // hbr 2ec <printValues+0x14c>,$80 
    0x42605623, // ila $35,c0ac <main+0xad9c> 
    0x340068a4, // lqd $36,16($81) 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x3f821222, // rotqbyi $34,$36,8 
    0x1808d121, // a $33,$34,$35 
    0x3888d120, // lqx $32,$34,$35 
    0x3b885003, // rotqby $3,$32,$33 
    0x35202800, // bisl $0,$80 
    0x4020007f, // nop $127 
    0x3580280a, // hbr 31c <printValues+0x17c>,$80 
    0x4260581e, // ila $30,c0b0 <main+0xada0> 
    0x3400689f, // lqd $31,16($81) 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x3f820f9d, // rotqbyi $29,$31,8 
    0x18078e9c, // a $28,$29,$30 
    0x38878e9b, // lqx $27,$29,$30 
    0x3b870d83, // rotqby $3,$27,$28 
    0x35202800, // bisl $0,$80 
    0x4020007f, // nop $127 
    0x3580280a, // hbr 34c <printValues+0x1ac>,$80 
    0x42605a19, // ila $25,c0b4 <main+0xada4> 
    0x3400689a, // lqd $26,16($81) 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x3f820d18, // rotqbyi $24,$26,8 
    0x18064c17, // a $23,$24,$25 
    0x38864c16, // lqx $22,$24,$25 
    0x3b85cb03, // rotqby $3,$22,$23 
    0x35202800, // bisl $0,$80 
    0x4020007f, // nop $127 
    0x3580280a, // hbr 37c <printValues+0x1dc>,$80 
    0x42608414, // ila $20,c108 <main+0xadf8> 
    0x34006895, // lqd $21,16($81) 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x3f820a93, // rotqbyi $19,$21,8 
    0x18050992, // a $18,$19,$20 
    0x38850991, // lqx $17,$19,$20 
    0x3b848883, // rotqby $3,$17,$18 
    0x35202800, // bisl $0,$80 
    0x4020007f, // nop $127 
    0x3580280a, // hbr 3ac <printValues+0x20c>,$80 
    0x42607e0f, // ila $15,c0fc <main+0xadec> 
    0x34006890, // lqd $16,16($81) 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x3f82080e, // rotqbyi $14,$16,8 
    0x1803c70d, // a $13,$14,$15 
    0x3883c70c, // lqx $12,$14,$15 
    0x3b834603, // rotqby $3,$12,$13 
    0x35202800, // bisl $0,$80 
    0x4020007f, // nop $127 
    0x3580280a, // hbr 3dc <printValues+0x23c>,$80 
    0x4260800a, // ila $10,c100 <main+0xadf0> 
    0x3400688b, // lqd $11,16($81) 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x3f820589, // rotqbyi $9,$11,8 
    0x18028488, // a $8,$9,$10 
    0x38828487, // lqx $7,$9,$10 
    0x3b820383, // rotqby $3,$7,$8 
    0x35202800, // bisl $0,$80 
    0x4020007f, // nop $127 
    0x3580280a, // hbr 40c <printValues+0x26c>,$80 
    0x42608204, // ila $4,c104 <main+0xadf4> 
    0x34006886, // lqd $6,16($81) 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x3f820305, // rotqbyi $5,$6,8 
    0x18010282, // a $2,$5,$4 
    0x38810283, // lqx $3,$5,$4 
    0x3b808183, // rotqby $3,$3,$2 
    0x35202800, // bisl $0,$80 
    0x40801083, // il $3,33 # 21 
    0x35202800, // bisl $0,$80 
    0x1c100081, // ai $1,$1,64 # 40 
    0x34004080, // lqd $0,16($1) 
    0x34ffc0d0, // lqd $80,-16($1) 
    0x34ff80d1, // lqd $81,-32($1) 
    0x35000000, // bi $0 
    0x00200000, // lnop 
};

#define PRINTVALUES_SIZE 656


uint32_t fetchMetaData[] __attribute__((aligned(128))) = 
{
    0x4080008a, // il $10,1 
    0x3580003c, // hbr 524 <fetchMetaData+0xf4>,$0 
    0x04000527, // ori $39,$10,0 
    0x35900000, // hbrp 43c <fetchMetaData+0xc>,$0 
    0x21a00b27, // wrch $ch22,$39 
    0x42602c25, // ila $37,c058 <main+0xad48> 
    0x340041a6, // lqd $38,16($3) 
    0x3f821324, // rotqbyi $36,$38,8 
    0x18095223, // a $35,$36,$37 
    0x38895222, // lqx $34,$36,$37 
    0x1c081220, // ai $32,$36,32 # 20 
    0x3b88d121, // rotqby $33,$34,$35 
    0x1808421f, // a $31,$4,$33 
    0x21a00820, // wrch $ch16,$32 
    0x21a0091f, // wrch $ch18,$31 
    0x40a00009, // il $9,16384 # 4000 
    0x35900000, // hbrp 470 <fetchMetaData+0x40>,$0 
    0x0400049e, // ori $30,$9,0 
    0x21a0099e, // wrch $ch19,$30 
    0x0400051d, // ori $29,$10,0 
    0x21a00a1d, // wrch $ch20,$29 
    0x40802008, // il $8,64 # 40 
    0x0400041c, // ori $28,$8,0 
    0x21a00a9c, // wrch $ch21,$28 
    0x4260541a, // ila $26,c0a8 <main+0xad98> 
    0x40a01019, // il $25,16416 # 4020 
    0x3400419b, // lqd $27,16($3) 
    0x3f820d98, // rotqbyi $24,$27,8 
    0x18068c17, // a $23,$24,$26 
    0x38868c16, // lqx $22,$24,$26 
    0x18064c14, // a $20,$24,$25 
    0x3b85cb15, // rotqby $21,$22,$23 
    0x18054213, // a $19,$4,$21 
    0x21a00814, // wrch $ch16,$20 
    0x21a00913, // wrch $ch18,$19 
    0x04000492, // ori $18,$9,0 
    0x21a00992, // wrch $ch19,$18 
    0x04000511, // ori $17,$10,0 
    0x21a00a11, // wrch $ch20,$17 
    0x04000410, // ori $16,$8,0 
    0x21a00a90, // wrch $ch21,$16 
    0x42607c0e, // ila $14,c0f8 <main+0xade8> 
    0x4240100d, // ila $13,8020 <main+0x6d10> 
    0x3400418f, // lqd $15,16($3) 
    0x4020007f, // nop $127 
    0x3f82078c, // rotqbyi $12,$15,8 
    0x18038606, // a $6,$12,$14 
    0x3883860b, // lqx $11,$12,$14 
    0x18034607, // a $7,$12,$13 
    0x3b818585, // rotqby $5,$11,$6 
    0x18014204, // a $4,$4,$5 
    0x21a00807, // wrch $ch16,$7 
    0x21a00904, // wrch $ch18,$4 
    0x21a00989, // wrch $ch19,$9 
    0x21a00a0a, // wrch $ch20,$10 
    0x21a00a88, // wrch $ch21,$8 
    0x40800102, // il $2,2 
    0x04000103, // ori $3,$2,0 
    0x21a00b03, // wrch $ch22,$3 
    0x21a00b82, // wrch $ch23,$2 
    0x01a00c03, // rdch $3,$ch24 
    0x35000000, // bi $0 
};

#define FETCHMETADATA_SIZE 248


uint32_t doCalculationsWithinTheBlocks[] __attribute__((aligned(128))) = 
{
    0x42603408, // ila $8,c068 <main+0xad58> 
    0x3400418a, // lqd $10,16($3) 
    0x3f820509, // rotqbyi $9,$10,8 
    0x35900000, // hbrp 534 <doCalculationsWithinTheBlocks+0xc>,$0 
    0x18020485, // a $5,$9,$8 
    0x38820482, // lqx $2,$9,$8 
    0x04000490, // ori $16,$9,0 
    0x3b814103, // rotqby $3,$2,$5 
    0x04000191, // ori $17,$3,0 
    0x21001d84, // brnz $4,638 <doCalculationsWithinTheBlocks+0x110> # 638 
    0x4080020d, // il $13,4 
    0x32800016, // fsmbi $22,0 
    0x1c01048c, // ai $12,$9,4 
    0x32800015, // fsmbi $21,0 
    0x0f3f8192, // rotmi $18,$3,-2 
    0x3883448b, // lqx $11,$9,$13 
    0x4020007f, // nop $127 
    0x35900000, // hbrp 56c <doCalculationsWithinTheBlocks+0x44>,$0 
    0x0f608197, // shli $23,$3,2 
    0x32800013, // fsmbi $19,0 
    0x4020007f, // nop $127 
    0x3b830584, // rotqby $4,$11,$12 
    0x04000214, // ori $20,$4,0 
    0x00200000, // lnop 
    0x20003083, // brz $3,70c <doCalculationsWithinTheBlocks+0x1e4> # 70c 
    0x00200000, // lnop 
    0x0f610b22, // shli $34,$22,4 
    0x3280000f, // fsmbi $15,0 
    0x0f61090d, // shli $13,$18,4 
    0x00200000, // lnop 
    0x1808830c, // a $12,$6,$34 
    0x00200000, // lnop 
    0x20000f92, // brz $18,624 <doCalculationsWithinTheBlocks+0xfc> # 624 
    0x00200000, // lnop 
    0x0f61079a, // shli $26,$15,4 
    0x34000819, // lqd $25,0($16) 
    0x04000985, // ori $5,$19,0 
    0x12000393, // hbrr 608 <doCalculationsWithinTheBlocks+0xe0>,5d8 <doCalculationsWithinTheBlocks+0xb0> # 5d8 
    0x4281018e, // ila $14,10203 <main+0xeef3> 
    0x3400060a, // lqd $10,0($12) 
    0x04000889, // ori $9,$17,0 
    0x18068a08, // a $8,$20,$26 
    0x3b840c98, // rotqby $24,$25,$16 
    0x04000c0b, // ori $11,$24,0 
    0x1802c2a1, // a $33,$5,$11 
    0x3882c2a0, // lqx $32,$5,$11 
    0x1cffc489, // ai $9,$9,-1 
    0x3400041d, // lqd $29,0($8) 
    0x1c010285, // ai $5,$5,4 
    0x18034408, // a $8,$8,$13 
    0x3b88501f, // rotqby $31,$32,$33 
    0x58c7c39e, // fm $30,$7,$31 
    0xb3878f0e, // shufb $28,$30,$30,$14 
    0xe3674e0a, // fma $27,$28,$29,$10 
    0x04000d8a, // ori $10,$27,0 
    0x2400061b, // stqd $27,0($12) 
    0x217ffa09, // brnz $9,5d8 <doCalculationsWithinTheBlocks+0xb0> # 5d8 
    0x1c00478f, // ai $15,$15,1 
    0x7804878e, // ceq $14,$15,$18 
    0x2100018e, // brnz $14,620 <doCalculationsWithinTheBlocks+0xf8> # 620 
    0x1c04060c, // ai $12,$12,16 
    0x327ff280, // br 5b0 <doCalculationsWithinTheBlocks+0x88> # 5b0 
    0x18048b16, // a $22,$22,$18 
    0x1c004a95, // ai $21,$21,1 
    0x7805488f, // ceq $15,$17,$21 
    0x2520000f, // binz $15,$0 
    0x1805c993, // a $19,$19,$23 
    0x327feb80, // br 590 <doCalculationsWithinTheBlocks+0x68> # 590 
    0x0f3f8192, // rotmi $18,$3,-2 
    0x34004494, // lqd $20,16($9) 
    0x0f608197, // shli $23,$3,2 
    0x32800016, // fsmbi $22,0 
    0x40800015, // il $21,0 
    0x32800013, // fsmbi $19,0 
    0x4020007f, // nop $127 
    0x3b824a23, // rotqby $35,$20,$9 
    0x04001194, // ori $20,$35,0 
    0x20001583, // brz $3,708 <doCalculationsWithinTheBlocks+0x1e0> # 708 
    0x0f610b32, // shli $50,$22,4 
    0x3280000f, // fsmbi $15,0 
    0x0f61090d, // shli $13,$18,4 
    0x00200000, // lnop 
    0x1801990c, // a $12,$50,$6 
    0x20001012, // brz $18,6f4 <doCalculationsWithinTheBlocks+0x1cc> # 6f4 
    0x40800628, // il $40,12 
    0x12000597, // hbrr 6d8 <doCalculationsWithinTheBlocks+0x1b0>,6a8 <doCalculationsWithinTheBlocks+0x180> # 6a8 
    0x1c030826, // ai $38,$16,12 
    0x3400060b, // lqd $11,0($12) 
    0x0f6107a7, // shli $39,$15,4 
    0x388a0825, // lqx $37,$16,$40 
    0x04000988, // ori $8,$19,0 
    0x3fe00889, // shlqbyi $9,$17,0 
    0x4281018e, // ila $14,10203 <main+0xeef3> 
    0x1809ca0a, // a $10,$20,$39 
    0x3b8992a4, // rotqby $36,$37,$38 
    0x04001205, // ori $5,$36,0 
    0x1801442f, // a $47,$8,$5 
    0x3881442e, // lqx $46,$8,$5 
    0x1cffc489, // ai $9,$9,-1 
    0x3400052b, // lqd $43,0($10) 
    0x1c010408, // ai $8,$8,4 
    0x1803450a, // a $10,$10,$13 
    0x3b8bd72d, // rotqby $45,$46,$47 
    0x58cb43ac, // fm $44,$7,$45 
    0xb54b160e, // shufb $42,$44,$44,$14 
    0xe52ad50b, // fma $41,$42,$43,$11 
    0x0400148b, // ori $11,$41,0 
    0x24000629, // stqd $41,0($12) 
    0x217ffa09, // brnz $9,6a8 <doCalculationsWithinTheBlocks+0x180> # 6a8 
    0x1c00478f, // ai $15,$15,1 
    0x7803c930, // ceq $48,$18,$15 
    0x210001b0, // brnz $48,6f0 <doCalculationsWithinTheBlocks+0x1c8> # 6f0 
    0x1c04060c, // ai $12,$12,16 
    0x327ff180, // br 678 <doCalculationsWithinTheBlocks+0x150> # 678 
    0x18048b16, // a $22,$22,$18 
    0x1c004a95, // ai $21,$21,1 
    0x780548b1, // ceq $49,$17,$21 
    0x25200031, // binz $49,$0 
    0x1805c993, // a $19,$19,$23 
    0x327feb80, // br 660 <doCalculationsWithinTheBlocks+0x138> # 660 
    0x35000000, // bi $0 
    0x35000000, // bi $0 
};

#define DOCALCULATIONSWITHINTHEBLOCKS_SIZE 488


uint32_t fetchMetaDataForResult[] __attribute__((aligned(128))) = 
{
    0x40800087, // il $7,1 
    0x35800018, // hbr 774 <fetchMetaDataForResult+0x64>,$0 
    0x04000390, // ori $16,$7,0 
    0x21a00b10, // wrch $ch22,$16 
    0x42607c0e, // ila $14,c0f8 <main+0xade8> 
    0x4240100d, // ila $13,8020 <main+0x6d10> 
    0x3400418f, // lqd $15,16($3) 
    0x3f82078c, // rotqbyi $12,$15,8 
    0x1803860b, // a $11,$12,$14 
    0x3883860a, // lqx $10,$12,$14 
    0x18034608, // a $8,$12,$13 
    0x3b82c509, // rotqby $9,$10,$11 
    0x18024206, // a $6,$4,$9 
    0x21a00808, // wrch $ch16,$8 
    0x21a00906, // wrch $ch18,$6 
    0x40a00005, // il $5,16384 # 4000 
    0x21a00985, // wrch $ch19,$5 
    0x21a00a07, // wrch $ch20,$7 
    0x40802004, // il $4,64 # 40 
    0x21a00a84, // wrch $ch21,$4 
    0x40800103, // il $3,2 
    0x04000182, // ori $2,$3,0 
    0x21a00b02, // wrch $ch22,$2 
    0x21a00b83, // wrch $ch23,$3 
    0x01a00c02, // rdch $2,$ch24 
    0x35000000, // bi $0 
};

#define FETCHMETADATAFORRESULT_SIZE 104


uint32_t doTheMultThingy[] __attribute__((aligned(128))) = 
{
    0x42607e12, // ila $18,c0fc <main+0xadec> 
    0x3400420f, // lqd $15,16($4) 
    0x40800e08, // il $8,28 # 1c 
    0x35900000, // hbrp 784 <doTheMultThingy+0xc>,$0 
    0x4020007f, // nop $127 
    0x34004287, // lqd $7,16($5) 
    0x04000223, // ori $35,$4,0 
    0x3fe001a2, // shlqbyi $34,$3,0 
    0x040002a4, // ori $36,$5,0 
    0x3f820793, // rotqbyi $19,$15,8 
    0x4020007f, // nop $127 
    0x3f830390, // rotqbyi $16,$7,12 
    0x18048986, // a $6,$19,$18 
    0x3884898d, // lqx $13,$19,$18 
    0x0400098f, // ori $15,$19,0 
    0x38820984, // lqx $4,$19,$8 
    0x3ec30987, // cwd $7,12($19) 
    0x3b818691, // rotqby $17,$13,$6 
    0x35900000, // hbrp 7c0 <doTheMultThingy+0x48>,$0 
    0x7f000800, // heqi $0,$16,0 
    0x12000492, // hbrr 810 <doTheMultThingy+0x98>,7ec <doTheMultThingy+0x74> # 7ec 
    0x54a0080c, // clz $12,$16 
    0x54a00889, // clz $9,$17 
    0x4080008a, // il $10,1 
    0x32800006, // fsmbi $6,0 
    0x0803048c, // sf $12,$9,$12 
    0x0400088e, // ori $14,$17,0 
    0x0b63050a, // shl $10,$10,$12 
    0x0b630809, // shl $9,$16,$12 
    0x0822830d, // or $13,$6,$10 
    0x3f3fc50a, // rotqmbii $10,$10,-1 
    0x5803848b, // clgt $11,$9,$14 
    0x00200000, // lnop 
    0x0803848c, // sf $12,$9,$14 
    0x3f3fc489, // rotqmbii $9,$9,-1 
    0x80c1868b, // selb $6,$13,$6,$11 
    0x00200000, // lnop 
    0x81c3860b, // selb $14,$12,$14,$11 
    0x217ffb8a, // brnz $10,7ec <doTheMultThingy+0x74> # 7ec 
    0x1200070a, // hbrr 83c <doTheMultThingy+0xc4>,84c <doTheMultThingy+0xd4> # 84c 
    0x5800c70b, // clgt $11,$14,$3 
    0xb0810307, // shufb $4,$6,$4,$7 
    0x7c00070a, // ceqi $10,$14,0 
    0x0922c583, // nor $3,$11,$11 
    0x4020007f, // nop $127 
    0x00200000, // lnop 
    0x0820c502, // or $2,$10,$3 
    0x28820984, // stqx $4,$19,$8 
    0x4020007f, // nop $127 
    0x21000202, // brnz $2,84c <doTheMultThingy+0xd4> # 84c 
    0x1c004315, // ai $21,$6,1 
    0xb2810a87, // shufb $20,$21,$4,$7 
    0x28820994, // stqx $20,$19,$8 
    0x42601043, // ila $67,c020 <main+0xad10> 
    0x4089c446, // il $70,5000 # 1388 
    0x00200000, // lnop 
    0x408000aa, // il $42,1 
    0x35900000, // hbrp 85c <doTheMultThingy+0xe4>,$0 
    0x3890c7c5, // lqx $69,$15,$67 
    0x3ad0c7c7, // cwx $71,$15,$67 
    0xb8916347, // shufb $68,$70,$69,$71 
    0x2890c7c4, // stqx $68,$15,$67 
    0x21a00b2a, // wrch $ch22,$42 
    0x00200000, // lnop 
    0x1c0e21c1, // ai $65,$67,56 # 38 
    0x340051c2, // lqd $66,16($35) 
    0x4020007f, // nop $127 
    0x3f822140, // rotqbyi $64,$66,8 
    0x1810603f, // a $63,$64,$65 
    0x3890603e, // lqx $62,$64,$65 
    0x1c08203d, // ai $61,$64,32 # 20 
    0x3b8fdf3c, // rotqby $60,$62,$63 
    0x35900000, // hbrp 898 <doTheMultThingy+0x120>,$0 
    0x21a0083d, // wrch $ch16,$61 
    0x21a0093c, // wrch $ch18,$60 
    0x40a0002b, // il $43,16384 # 4000 
    0x21a009ab, // wrch $ch19,$43 
    0x21a00a2a, // wrch $ch20,$42 
    0x40802029, // il $41,64 # 40 
    0x21a00aa9, // wrch $ch21,$41 
    0x4260543a, // ila $58,c0a8 <main+0xad98> 
    0x40a01039, // il $57,16416 # 4020 
    0x340051bb, // lqd $59,16($35) 
    0x3f821db8, // rotqbyi $56,$59,8 
    0x180e9c37, // a $55,$56,$58 
    0x388e9c36, // lqx $54,$56,$58 
    0x180e5c35, // a $53,$56,$57 
    0x3b8ddb34, // rotqby $52,$54,$55 
    0x21a00835, // wrch $ch16,$53 
    0x21a00934, // wrch $ch18,$52 
    0x21a009ab, // wrch $ch19,$43 
    0x21a00a2a, // wrch $ch20,$42 
    0x21a00aa9, // wrch $ch21,$41 
    0x42607c32, // ila $50,c0f8 <main+0xade8> 
    0x42401031, // ila $49,8020 <main+0x6d10> 
    0x340051b3, // lqd $51,16($35) 
    0x4020007f, // nop $127 
    0x3f8219b0, // rotqbyi $48,$51,8 
    0x180c982f, // a $47,$48,$50 
    0x388c982e, // lqx $46,$48,$50 
    0x180c582d, // a $45,$48,$49 
    0x3b8bd72c, // rotqby $44,$46,$47 
    0x21a0082d, // wrch $ch16,$45 
    0x21a0092c, // wrch $ch18,$44 
    0x21a009ab, // wrch $ch19,$43 
    0x21a00a2a, // wrch $ch20,$42 
    0x21a00aa9, // wrch $ch21,$41 
    0x40800128, // il $40,2 
    0x21a00b28, // wrch $ch22,$40 
    0x21a00ba8, // wrch $ch23,$40 
    0x01a00c03, // rdch $3,$ch24 
    0x40800421, // il $33,8 
    0x40800e20, // il $32,28 # 1c 
    0x00200000, // lnop 
    0x40800225, // il $37,4 
    0x340051a7, // lqd $39,16($35) 
    0x4080040a, // il $10,8 
    0x3f8213a6, // rotqbyi $38,$39,8 
    0x1c02131c, // ai $28,$38,8 
    0x38885317, // lqx $23,$38,$33 
    0x1c07131a, // ai $26,$38,28 # 1c 
    0x38881319, // lqx $25,$38,$32 
    0x1c01131f, // ai $31,$38,4 
    0x34001318, // lqd $24,0($38) 
    0x0400130f, // ori $15,$38,0 
    0x3889531e, // lqx $30,$38,$37 
    0x3b870b9b, // rotqby $27,$23,$28 
    0x3b868c96, // rotqby $22,$25,$26 
    0x3b898c08, // rotqby $8,$24,$38 
    0x3b87cf1d, // rotqby $29,$30,$31 
    0x04000d97, // ori $23,$27,0 
    0x00200000, // lnop 
    0x0400041f, // ori $31,$8,0 
    0x2000f896, // brz $22,1150 <doTheMultThingy+0x9d8> # 1150 
    0x04000eae, // ori $46,$29,0 
    0x3fe0042f, // shlqbyi $47,$8,0 
    0x40800020, // il $32,0 
    0x3280001e, // fsmbi $30,0 
    0x4080019c, // il $28,3 
    0x4080021b, // il $27,4 
    0x4260100b, // ila $11,c020 <main+0xad10> 
    0x4089c3c9, // il $73,4999 # 1387 
    0x1802c7cc, // a $76,$15,$11 
    0x35900000, // hbrp 9b4 <doTheMultThingy+0x23c>,$0 
    0x3882c7cb, // lqx $75,$15,$11 
    0x3b9325ca, // rotqby $74,$75,$76 
    0x48126548, // cgt $72,$74,$73 
    0x200112c8, // brz $72,1258 <doTheMultThingy+0xae0> # 1258 
    0x42608012, // ila $18,c100 <main+0xadf0> 
    0x3882c791, // lqx $17,$15,$11 
    0x4260140c, // ila $12,c028 <main+0xad18> 
    0x3ac2c787, // cwx $7,$15,$11 
    0x18048790, // a $16,$15,$18 
    0x3884878e, // lqx $14,$15,$18 
    0x4260124d, // ila $77,c024 <main+0xad14> 
    0x3ac30786, // cwx $6,$15,$12 
    0x35900000, // hbrp 9e8 <doTheMultThingy+0x270>,$0 
    0x3ad34789, // cwx $9,$15,$77 
    0xb1445107, // shufb $10,$34,$17,$7 
    0x3b840704, // rotqby $4,$14,$16 
    0x2882c78a, // stqx $10,$15,$11 
    0x3883078d, // lqx $13,$15,$12 
    0x7f000200, // heqi $0,$4,0 
    0x12000492, // hbrr a4c <doTheMultThingy+0x2d4>,a28 <doTheMultThingy+0x2b0> # a28 
    0x54a0020a, // clz $10,$4 
    0x54a0110e, // clz $14,$34 
    0x40800090, // il $16,1 
    0x32800002, // fsmbi $2,0 
    0x0802870a, // sf $10,$14,$10 
    0x04001105, // ori $5,$34,0 
    0x0b628810, // shl $16,$16,$10 
    0x0b62820e, // shl $14,$4,$10 
    0x0824010b, // or $11,$2,$16 
    0x3f3fc810, // rotqmbii $16,$16,-1 
    0x58014711, // clgt $17,$14,$5 
    0x00200000, // lnop 
    0x0801470a, // sf $10,$14,$5 
    0x3f3fc70e, // rotqmbii $14,$14,-1 
    0x80408591, // selb $2,$11,$2,$17 
    0x00200000, // lnop 
    0x80a14511, // selb $5,$10,$5,$17 
    0x217ffb90, // brnz $16,a28 <doTheMultThingy+0x2b0> # a28 
    0xb0634106, // shufb $3,$2,$13,$6 
    0x28830783, // stqx $3,$15,$12 
    0x389347cf, // lqx $79,$15,$77 
    0xb9d3c289, // shufb $78,$5,$79,$9 
    0x289347ce, // stqx $78,$15,$77 
    0x4260140d, // ila $13,c028 <main+0xad18> 
    0x4260300e, // ila $14,c060 <main+0xad50> 
    0x00200000, // lnop 
    0x180347a5, // a $37,$15,$13 
    0x35900000, // hbrp a74 <doTheMultThingy+0x2fc>,$0 
    0x4020007f, // nop $127 
    0x388347a1, // lqx $33,$15,$13 
    0x1803879d, // a $29,$15,$14 
    0x3883879a, // lqx $26,$15,$14 
    0x4260120b, // ila $11,c024 <main+0xad14> 
    0x4260900a, // ila $10,c120 <main+0xae10> 
    0x1802c798, // a $24,$15,$11 
    0x3882c796, // lqx $22,$15,$11 
    0x18028795, // a $21,$15,$10 
    0x38828784, // lqx $4,$15,$10 
    0x3b895088, // rotqby $8,$33,$37 
    0x3b874d19, // rotqby $25,$26,$29 
    0x3b860b10, // rotqby $16,$22,$24 
    0x35900000, // hbrp aac <doTheMultThingy+0x334>,$0 
    0x78a64413, // mpyh $19,$8,$25 
    0x3b854209, // rotqby $9,$4,$21 
    0x78a20c94, // mpyh $20,$25,$8 
    0x79864412, // mpyu $18,$8,$25 
    0x18050987, // a $7,$19,$20 
    0x18048391, // a $17,$7,$18 
    0x0f608886, // shli $6,$17,2 
    0x1803c303, // a $3,$6,$15 
    0x34008182, // lqd $2,32($3) # 20 
    0x3b80c10f, // rotqby $15,$2,$3 
    0x21a0082f, // wrch $ch16,$47 
    0x21a0090f, // wrch $ch18,$15 
    0x21a00989, // wrch $ch19,$9 
    0x04000f47, // ori $71,$30,0 
    0x21a00a47, // wrch $ch20,$71 
    0x40802009, // il $9,64 # 40 
    0x040004b6, // ori $54,$9,0 
    0x21a00ab6, // wrch $ch21,$54 
    0x40a0100f, // il $15,16416 # 4020 
    0x00200000, // lnop 
    0x0f608805, // shli $5,$16,2 
    0x3400518c, // lqd $12,16($35) 
    0x4020007f, // nop $127 
    0x3f82064f, // rotqbyi $79,$12,8 
    0x1813c2ce, // a $78,$5,$79 
    0x3882a7cc, // lqx $76,$79,$10 
    0x1802a7cd, // a $77,$79,$10 
    0x00200000, // lnop 
    0x1803e74b, // a $75,$78,$15 
    0x3883e74a, // lqx $74,$78,$15 
    0x3b936648, // rotqby $72,$76,$77 
    0x3b92e549, // rotqby $73,$74,$75 
    0x21a0082e, // wrch $ch16,$46 
    0x21a00949, // wrch $ch18,$73 
    0x21a009c8, // wrch $ch19,$72 
    0x21a00a47, // wrch $ch20,$71 
    0x21a00ab6, // wrch $ch21,$54 
    0x42601045, // ila $69,c020 <main+0xad10> 
    0x4240103d, // ila $61,8020 <main+0x6d10> 
    0x00200000, // lnop 
    0x04000bad, // ori $45,$23,0 
    0x340051c6, // lqd $70,16($35) 
    0x4020007f, // nop $127 
    0x3f82233f, // rotqbyi $63,$70,8 
    0x18115fc4, // a $68,$63,$69 
    0x38915fc3, // lqx $67,$63,$69 
    0x18029fc1, // a $65,$63,$10 
    0x38829fc0, // lqx $64,$63,$10 
    0x3b9121c2, // rotqby $66,$67,$68 
    0x3b906038, // rotqby $56,$64,$65 
    0x0f60a13e, // shli $62,$66,2 
    0x180fdf3c, // a $60,$62,$63 
    0x180f5e3b, // a $59,$60,$61 
    0x388f5e3a, // lqx $58,$60,$61 
    0x3b8edd39, // rotqby $57,$58,$59 
    0x21a0082d, // wrch $ch16,$45 
    0x21a00939, // wrch $ch18,$57 
    0x21a009b8, // wrch $ch19,$56 
    0x408002b7, // il $55,5 
    0x21a00a37, // wrch $ch20,$55 
    0x21a00ab6, // wrch $ch21,$54 
    0x40801035, // il $53,32 # 20 
    0x21a00b35, // wrch $ch22,$53 
    0x40800104, // il $4,2 
    0x04000234, // ori $52,$4,0 
    0x21a00bb4, // wrch $ch23,$52 
    0x01a00c03, // rdch $3,$ch24 
    0x340051b3, // lqd $51,16($35) 
    0x4020007f, // nop $127 
    0x3f8219b2, // rotqbyi $50,$51,8 
    0x04001906, // ori $6,$50,0 
    0x38839931, // lqx $49,$50,$14 
    0x5c0058b0, // clgti $48,$49,1 
    0x3fe0188c, // shlqbyi $12,$49,0 
    0x4020007f, // nop $127 
    0x20004f30, // brz $48,e54 <doTheMultThingy+0x6dc> # e54 
    0x40800098, // il $24,1 
    0x3fe0049a, // shlqbyi $26,$9,0 
    0x04000219, // ori $25,$4,0 
    0x3fe006a5, // shlqbyi $37,$13,0 
    0x040005a7, // ori $39,$11,0 
    0x3fe00529, // shlqbyi $41,$10,0 
    0x040007aa, // ori $42,$15,0 
    0x3fe0072c, // shlqbyi $44,$14,0 
    0x42605826, // ila $38,c0b0 <main+0xada0> 
    0x3fe00c21, // shlqbyi $33,$24,0 
    0x408000a8, // il $40,1 
    0x4260342b, // ila $43,c068 <main+0xad58> 
    0x4080004f, // il $79,0 
    0x3889430b, // lqx $11,$6,$37 
    0x40800090, // il $16,1 
    0x35900000, // hbrp c1c <doTheMultThingy+0x4a4>,$0 
    0x38898304, // lqx $4,$6,$38 
    0x34000307, // lqd $7,0($6) 
    0x3889c308, // lqx $8,$6,$39 
    0x3400430a, // lqd $10,16($6) 
    0x3b894595, // rotqby $21,$11,$37 
    0x3b898214, // rotqby $20,$4,$38 
    0x7801cf93, // ceq $19,$31,$7 
    0x3f83039d, // rotqbyi $29,$7,12 
    0x4020007f, // nop $127 
    0x3b89c449, // rotqby $73,$8,$39 
    0x78a54611, // mpyh $17,$12,$21 
    0x3f810396, // rotqbyi $22,$7,4 
    0x78a30a92, // mpyh $18,$21,$12 
    0x35900000, // hbrp c54 <doTheMultThingy+0x4dc>,$0 
    0x78a50c06, // mpyh $6,$24,$20 
    0x78a60a09, // mpyh $9,$20,$24 
    0x79854603, // mpyu $3,$12,$21 
    0x79850c02, // mpyu $2,$24,$20 
    0x7c0009ca, // ceqi $74,$19,0 
    0x18048885, // a $5,$17,$18 
    0x83e1ceca, // selb $31,$29,$7,$74 
    0x1802430f, // a $15,$6,$9 
    0x1800c2af, // a $47,$5,$3 
    0x180087c8, // a $72,$15,$2 
    0x816427ca, // selb $11,$79,$16,$74 
    0x87c5854a, // selb $62,$10,$22,$74 
    0x1812643d, // a $61,$72,$73 
    0x180bcc47, // a $71,$24,$47 
    0x44004f1d, // xori $29,$30,1 
    0x21a00b21, // wrch $ch22,$33 
    0x0f60a3c5, // shli $69,$71,2 
    0x340051c6, // lqd $70,16($35) 
    0x04000faf, // ori $47,$31,0 
    0x3f822344, // rotqbyi $68,$70,8 
    0x181122c1, // a $65,$69,$68 
    0x388a6242, // lqx $66,$68,$41 
    0x180a6243, // a $67,$68,$41 
    0x3400a0c0, // lqd $64,32($65) # 20 
    0x3b90e12e, // rotqby $46,$66,$67 
    0x3b90603f, // rotqby $63,$64,$65 
    0x21a0082f, // wrch $ch16,$47 
    0x21a0093f, // wrch $ch18,$63 
    0x21a009ae, // wrch $ch19,$46 
    0x04000eb2, // ori $50,$29,0 
    0x21a00a32, // wrch $ch20,$50 
    0x21a00a9a, // wrch $ch21,$26 
    0x0f609ebb, // shli $59,$61,2 
    0x340051bc, // lqd $60,16($35) 
    0x04001f2e, // ori $46,$62,0 
    0x3f821e3a, // rotqbyi $58,$60,8 
    0x180e9db9, // a $57,$59,$58 
    0x388a5d37, // lqx $55,$58,$41 
    0x180a5d38, // a $56,$58,$41 
    0x00200000, // lnop 
    0x180a9cb6, // a $54,$57,$42 
    0x388a9cb5, // lqx $53,$57,$42 
    0x3b8e1bb3, // rotqby $51,$55,$56 
    0x3b8d9ab4, // rotqby $52,$53,$54 
    0x21a0082e, // wrch $ch16,$46 
    0x21a00934, // wrch $ch18,$52 
    0x21a009b3, // wrch $ch19,$51 
    0x21a00a32, // wrch $ch20,$50 
    0x21a00a9a, // wrch $ch21,$26 
    0x0b679431, // shl $49,$40,$30 
    0x21a00b31, // wrch $ch22,$49 
    0x21a00b99, // wrch $ch23,$25 
    0x01a00c03, // rdch $3,$ch24 
    0x340051b0, // lqd $48,16($35) 
    0x4020007f, // nop $127 
    0x3f82180e, // rotqbyi $14,$48,8 
    0x180ac70a, // a $10,$14,$43 
    0x388ac70d, // lqx $13,$14,$43 
    0x0400070f, // ori $15,$14,0 
    0x3b82869e, // rotqby $30,$13,$10 
    0x04000f10, // ori $16,$30,0 
    0x2100458b, // brnz $11,f78 <doTheMultThingy+0x800> # f78 
    0x40800231, // il $49,4 
    0x32800015, // fsmbi $21,0 
    0x1c010730, // ai $48,$14,4 
    0x32800014, // fsmbi $20,0 
    0x0f3f8f11, // rotmi $17,$30,-2 
    0x388c470e, // lqx $14,$14,$49 
    0x0f608f16, // shli $22,$30,2 
    0x32800012, // fsmbi $18,0 
    0x4020007f, // nop $127 
    0x3b8c071e, // rotqby $30,$14,$48 
    0x04000f13, // ori $19,$30,0 
    0x20001590, // brz $16,e28 <doTheMultThingy+0x6b0> # e28 
    0x0f610abe, // shli $62,$21,4 
    0x3280000e, // fsmbi $14,0 
    0x0f61088b, // shli $11,$17,4 
    0x00200000, // lnop 
    0x1805df0a, // a $10,$62,$23 
    0x20001011, // brz $17,e14 <doTheMultThingy+0x69c> # e14 
    0x0f610734, // shli $52,$14,4 
    0x12000597, // hbrr df8 <doTheMultThingy+0x680>,dc8 <doTheMultThingy+0x650> # dc8 
    0x04000906, // ori $6,$18,0 
    0x340007b3, // lqd $51,0($15) 
    0x415fc00d, // ilhu $13,49024 # bf80 
    0x34000508, // lqd $8,0($10) 
    0x4281018c, // ila $12,10203 <main+0xeef3> 
    0x3fe00805, // shlqbyi $5,$16,0 
    0x180d0987, // a $7,$19,$52 
    0x3b83d9b2, // rotqby $50,$51,$15 
    0x04001909, // ori $9,$50,0 
    0x00200000, // lnop 
    0x180184bb, // a $59,$9,$6 
    0x388184ba, // lqx $58,$9,$6 
    0x1cffc285, // ai $5,$5,-1 
    0x340003b7, // lqd $55,0($7) 
    0x1c010306, // ai $6,$6,4 
    0x1802c387, // a $7,$7,$11 
    0x3b8edd39, // rotqby $57,$58,$59 
    0x58c35cb8, // fm $56,$57,$13 
    0xb6ce1c0c, // shufb $54,$56,$56,$12 
    0xe6addb08, // fma $53,$54,$55,$8 
    0x04001a88, // ori $8,$53,0 
    0x24000535, // stqd $53,0($10) 
    0x217ffa05, // brnz $5,dc8 <doTheMultThingy+0x650> # dc8 
    0x1c00470e, // ai $14,$14,1 
    0x780388bc, // ceq $60,$17,$14 
    0x210001bc, // brnz $60,e10 <doTheMultThingy+0x698> # e10 
    0x1c04050a, // ai $10,$10,16 
    0x327ff180, // br d98 <doTheMultThingy+0x620> # d98 
    0x18044a95, // a $21,$21,$17 
    0x1c004a14, // ai $20,$20,1 
    0x7805083d, // ceq $61,$16,$20 
    0x210001bd, // brnz $61,e28 <doTheMultThingy+0x6b0> # e28 
    0x18058912, // a $18,$18,$22 
    0x327feb80, // br d80 <doTheMultThingy+0x608> # d80 
    0x4020007f, // nop $127 
    0x127fbc89, // hbrr e50 <doTheMultThingy+0x6d8>,c10 <doTheMultThingy+0x498> # c10 
    0x180b0782, // a $2,$15,$44 
    0x388b078c, // lqx $12,$15,$44 
    0x04000786, // ori $6,$15,0 
    0x3fe00e9e, // shlqbyi $30,$29,0 
    0x1c004c18, // ai $24,$24,1 
    0x3b80860f, // rotqby $15,$12,$2 
    0x58060790, // clgt $16,$15,$24 
    0x3fe0078c, // shlqbyi $12,$15,0 
    0x217fb810, // brnz $16,c10 <doTheMultThingy+0x498> # c10 
    0x40800098, // il $24,1 
    0x0b678c2c, // shl $44,$24,$30 
    0x21a00b2c, // wrch $ch22,$44 
    0x4080012b, // il $43,2 
    0x21a00bab, // wrch $ch23,$43 
    0x01a00c02, // rdch $2,$ch24 
    0x340051aa, // lqd $42,16($35) 
    0x4020007f, // nop $127 
    0x3f821529, // rotqbyi $41,$42,8 
    0x0400148f, // ori $15,$41,0 
    0x340014a8, // lqd $40,0($41) 
    0x3b8a5427, // rotqby $39,$40,$41 
    0x7809cfa6, // ceq $38,$31,$39 
    0x4020007f, // nop $127 
    0x21005ba6, // brnz $38,1168 <doTheMultThingy+0x9f0> # 1168 
    0x42603408, // ila $8,c068 <main+0xad58> 
    0x34005487, // lqd $7,16($41) 
    0x40800015, // il $21,0 
    0x32800014, // fsmbi $20,0 
    0x180214b6, // a $54,$41,$8 
    0x388214b5, // lqx $53,$41,$8 
    0x40800012, // il $18,0 
    0x3b8a43b7, // rotqby $55,$7,$41 
    0x3b8d9ab4, // rotqby $52,$53,$54 
    0x04001b93, // ori $19,$55,0 
    0x0f3f9a11, // rotmi $17,$52,-2 
    0x3fe01a10, // shlqbyi $16,$52,0 
    0x0f609a16, // shli $22,$52,2 
    0x200031b4, // brz $52,1050 <doTheMultThingy+0x8d8> # 1050 
    0x0f610ac4, // shli $68,$21,4 
    0x3280000e, // fsmbi $14,0 
    0x0f61088c, // shli $12,$17,4 
    0x00200000, // lnop 
    0x1805e20a, // a $10,$68,$23 
    0x20001111, // brz $17,f64 <doTheMultThingy+0x7ec> # f64 
    0x4080063b, // il $59,12 
    0x12000699, // hbrr f48 <doTheMultThingy+0x7d0>,f18 <doTheMultThingy+0x7a0> # f18 
    0x1c0307ba, // ai $58,$15,12 
    0x34000509, // lqd $9,0($10) 
    0x0f61070b, // shli $11,$14,4 
    0x388ec7b9, // lqx $57,$15,$59 
    0x04000906, // ori $6,$18,0 
    0x3fe00807, // shlqbyi $7,$16,0 
    0x415fc00d, // ilhu $13,49024 # bf80 
    0x1802c988, // a $8,$19,$11 
    0x4281018b, // ila $11,10203 <main+0xeef3> 
    0x3b8e9cb8, // rotqby $56,$57,$58 
    0x04001c05, // ori $5,$56,0 
    0x00200000, // lnop 
    0x180182c2, // a $66,$5,$6 
    0x388182c1, // lqx $65,$5,$6 
    0x1cffc387, // ai $7,$7,-1 
    0x3400043e, // lqd $62,0($8) 
    0x1c010306, // ai $6,$6,4 
    0x18030408, // a $8,$8,$12 
    0x3b90a0c0, // rotqby $64,$65,$66 
    0x58c3603f, // fm $63,$64,$13 
    0xb7afdf8b, // shufb $61,$63,$63,$11 
    0xe78f9e89, // fma $60,$61,$62,$9 
    0x04001e09, // ori $9,$60,0 
    0x2400053c, // stqd $60,0($10) 
    0x217ffa07, // brnz $7,f18 <doTheMultThingy+0x7a0> # f18 
    0x1c00470e, // ai $14,$14,1 
    0x780388c3, // ceq $67,$17,$14 
    0x210001c3, // brnz $67,f60 <doTheMultThingy+0x7e8> # f60 
    0x1c04050a, // ai $10,$10,16 
    0x327ff080, // br ee0 <doTheMultThingy+0x768> # ee0 
    0x18044a95, // a $21,$21,$17 
    0x1c004a14, // ai $20,$20,1 
    0x7805080a, // ceq $10,$16,$20 
    0x21001c8a, // brnz $10,1050 <doTheMultThingy+0x8d8> # 1050 
    0x18058912, // a $18,$18,$22 
    0x327fea80, // br ec8 <doTheMultThingy+0x750> # ec8 
    0x0f3f8f11, // rotmi $17,$30,-2 
    0x34004740, // lqd $64,16($14) 
    0x0f608f16, // shli $22,$30,2 
    0x32800015, // fsmbi $21,0 
    0x40800014, // il $20,0 
    0x32800012, // fsmbi $18,0 
    0x4020007f, // nop $127 
    0x3b83a03f, // rotqby $63,$64,$14 
    0x04001f93, // ori $19,$63,0 
    0x207fd19e, // brz $30,e28 <doTheMultThingy+0x6b0> # e28 
    0x0f610acf, // shli $79,$21,4 
    0x3280000e, // fsmbi $14,0 
    0x0f61088c, // shli $12,$17,4 
    0x00200000, // lnop 
    0x1813cb8a, // a $10,$23,$79 
    0x20001111, // brz $17,103c <doTheMultThingy+0x8c4> # 103c 
    0x40800645, // il $69,12 
    0x12000699, // hbrr 1020 <doTheMultThingy+0x8a8>,ff0 <doTheMultThingy+0x878> # ff0 
    0x1c0307c3, // ai $67,$15,12 
    0x34000509, // lqd $9,0($10) 
    0x0f610744, // shli $68,$14,4 
    0x389147c2, // lqx $66,$15,$69 
    0x04000906, // ori $6,$18,0 
    0x3fe00807, // shlqbyi $7,$16,0 
    0x415fc00d, // ilhu $13,49024 # bf80 
    0x4281018b, // ila $11,10203 <main+0xeef3> 
    0x1804e208, // a $8,$68,$19 
    0x3b90e141, // rotqby $65,$66,$67 
    0x04002085, // ori $5,$65,0 
    0x00200000, // lnop 
    0x1801434c, // a $76,$6,$5 
    0x3881434b, // lqx $75,$6,$5 
    0x1cffc387, // ai $7,$7,-1 
    0x34000448, // lqd $72,0($8) 
    0x1c010306, // ai $6,$6,4 
    0x18030408, // a $8,$8,$12 
    0x3b9325ca, // rotqby $74,$75,$76 
    0x58c36549, // fm $73,$74,$13 
    0xb8f2648b, // shufb $71,$73,$73,$11 
    0xe8d22389, // fma $70,$71,$72,$9 
    0x04002309, // ori $9,$70,0 
    0x24000546, // stqd $70,0($10) 
    0x217ffa07, // brnz $7,ff0 <doTheMultThingy+0x878> # ff0 
    0x1c00470e, // ai $14,$14,1 
    0x780388cd, // ceq $77,$17,$14 
    0x210001cd, // brnz $77,1038 <doTheMultThingy+0x8c0> # 1038 
    0x1c04050a, // ai $10,$10,16 
    0x327ff080, // br fb8 <doTheMultThingy+0x840> # fb8 
    0x18044a95, // a $21,$21,$17 
    0x1c004a14, // ai $20,$20,1 
    0x7805084e, // ceq $78,$16,$20 
    0x217fbcce, // brnz $78,e28 <doTheMultThingy+0x6b0> # e28 
    0x18058912, // a $18,$18,$22 
    0x327fea80, // br fa0 <doTheMultThingy+0x828> # fa0 
    0x42601033, // ila $51,c020 <main+0xad10> 
    0x127f2abc, // hbrr 1144 <doTheMultThingy+0x9cc>,9a8 <doTheMultThingy+0x230> # 9a8 
    0x42401006, // ila $6,8020 <main+0x6d10> 
    0x00200000, // lnop 
    0x180cc784, // a $4,$15,$51 
    0x388cc7b1, // lqx $49,$15,$51 
    0x42609032, // ila $50,c120 <main+0xae10> 
    0x00200000, // lnop 
    0x180c879d, // a $29,$15,$50 
    0x388c8785, // lqx $5,$15,$50 
    0x3b8118b0, // rotqby $48,$49,$4 
    0x3b874299, // rotqby $25,$5,$29 
    0x0f609809, // shli $9,$48,2 
    0x1803c483, // a $3,$9,$15 
    0x180181a5, // a $37,$3,$6 
    0x388181a1, // lqx $33,$3,$6 
    0x3b89509a, // rotqby $26,$33,$37 
    0x21a0082d, // wrch $ch16,$45 
    0x21a0091a, // wrch $ch18,$26 
    0x21a00999, // wrch $ch19,$25 
    0x04000e18, // ori $24,$28,0 
    0x21a00a18, // wrch $ch20,$24 
    0x4080102c, // il $44,32 # 20 
    0x21a00aac, // wrch $ch21,$44 
    0x408000ab, // il $43,1 
    0x0b66d5aa, // shl $42,$43,$27 
    0x21a00b2a, // wrch $ch22,$42 
    0x40800129, // il $41,2 
    0x21a00ba9, // wrch $ch23,$41 
    0x01a00c04, // rdch $4,$ch24 
    0x40800a0f, // il $15,20 # 14 
    0x40800e4e, // il $78,28 # 1c 
    0x7c00ce47, // ceqi $71,$28,3 
    0x340051a8, // lqd $40,16($35) 
    0x40800427, // il $39,8 
    0x7c002390, // ceqi $16,$71,0 
    0x7c00239c, // ceqi $28,$71,0 
    0x1c005020, // ai $32,$32,1 
    0x04000e17, // ori $23,$28,0 
    0x00200000, // lnop 
    0x4080018e, // il $14,3 
    0x3f82140c, // rotqbyi $12,$40,8 
    0x40800216, // il $22,4 
    0x3f821402, // rotqbyi $2,$40,8 
    0x40800215, // il $21,4 
    0x40800194, // il $20,3 
    0x1c02064d, // ai $77,$12,8 
    0x3889c64c, // lqx $76,$12,$39 
    0x1c05014a, // ai $74,$2,20 # 14 
    0x3883c149, // lqx $73,$2,$15 
    0x81e30110, // selb $15,$2,$12,$16 
    0x00200000, // lnop 
    0x1c0707c6, // ai $70,$15,28 # 1c 
    0x389387c5, // lqx $69,$15,$78 
    0x3b93664b, // rotqby $75,$76,$77 
    0x3b92a4c8, // rotqby $72,$73,$74 
    0x3b91a29b, // rotqby $27,$69,$70 
    0x82f2e417, // selb $23,$72,$75,$23 
    0x58080dad, // clgt $45,$27,$32 
    0x8365871c, // selb $27,$14,$22,$28 
    0x83850a9c, // selb $28,$21,$20,$28 
    0x217f0cad, // brnz $45,9a8 <doTheMultThingy+0x230> # 9a8 
    0x408000a2, // il $34,1 
    0x0b67110a, // shl $10,$34,$28 
    0x04000524, // ori $36,$10,0 
    0x21a00b24, // wrch $ch22,$36 
    0x40800123, // il $35,2 
    0x21a00ba3, // wrch $ch23,$35 
    0x01a00c02, // rdch $2,$ch24 
    0x35000000, // bi $0 
    0x42603406, // ila $6,c068 <main+0xad58> 
    0x32800014, // fsmbi $20,0 
    0x40800209, // il $9,4 
    0x32800015, // fsmbi $21,0 
    0x18019483, // a $3,$41,$6 
    0x38819485, // lqx $5,$41,$6 
    0x1c0114a1, // ai $33,$41,4 
    0x3882549a, // lqx $26,$41,$9 
    0x40800013, // il $19,0 
    0x3b80c2a5, // rotqby $37,$5,$3 
    0x4020007f, // nop $127 
    0x3b884d19, // rotqby $25,$26,$33 
    0x0f3f9291, // rotmi $17,$37,-2 
    0x3fe01290, // shlqbyi $16,$37,0 
    0x0f609296, // shli $22,$37,2 
    0x3fe00c92, // shlqbyi $18,$25,0 
    0x4020007f, // nop $127 
    0x207fd4a5, // brz $37,1050 <doTheMultThingy+0x8d8> # 1050 
    0x0f610a0d, // shli $13,$20,4 
    0x3280000e, // fsmbi $14,0 
    0x0f61088b, // shli $11,$17,4 
    0x00200000, // lnop 
    0x1805c68a, // a $10,$13,$23 
    0x20001011, // brz $17,1244 <doTheMultThingy+0xacc> # 1244 
    0x0f610730, // shli $48,$14,4 
    0x12000597, // hbrr 1228 <doTheMultThingy+0xab0>,11f8 <doTheMultThingy+0xa80> # 11f8 
    0x04000986, // ori $6,$19,0 
    0x34000784, // lqd $4,0($15) 
    0x415fc00d, // ilhu $13,49024 # bf80 
    0x34000508, // lqd $8,0($10) 
    0x4281018c, // ila $12,10203 <main+0xeef3> 
    0x3fe00805, // shlqbyi $5,$16,0 
    0x18049807, // a $7,$48,$18 
    0x3b83c21d, // rotqby $29,$4,$15 
    0x04000e89, // ori $9,$29,0 
    0x00200000, // lnop 
    0x180184b7, // a $55,$9,$6 
    0x388184b6, // lqx $54,$9,$6 
    0x1cffc285, // ai $5,$5,-1 
    0x340003b3, // lqd $51,0($7) 
    0x1c010306, // ai $6,$6,4 
    0x1802c387, // a $7,$7,$11 
    0x3b8ddb35, // rotqby $53,$54,$55 
    0x58c35ab4, // fm $52,$53,$13 
    0xb64d1a0c, // shufb $50,$52,$52,$12 
    0xe62cd908, // fma $49,$50,$51,$8 
    0x04001888, // ori $8,$49,0 
    0x24000531, // stqd $49,0($10) 
    0x217ffa05, // brnz $5,11f8 <doTheMultThingy+0xa80> # 11f8 
    0x1c00470e, // ai $14,$14,1 
    0x78044707, // ceq $7,$14,$17 
    0x21000187, // brnz $7,1240 <doTheMultThingy+0xac8> # 1240 
    0x1c04050a, // ai $10,$10,16 
    0x327ff180, // br 11c8 <doTheMultThingy+0xa50> # 11c8 
    0x18044a14, // a $20,$20,$17 
    0x1c004a95, // ai $21,$21,1 
    0x78054808, // ceq $8,$16,$21 
    0x217fc088, // brnz $8,1050 <doTheMultThingy+0x8d8> # 1050 
    0x18058993, // a $19,$19,$22 
    0x327feb80, // br 11b0 <doTheMultThingy+0xa38> # 11b0 
    0x4260802d, // ila $45,c100 <main+0xadf0> 
    0x3400522c, // lqd $44,16($36) 
    0x42601418, // ila $24,c028 <main+0xad18> 
    0x3882c7a9, // lqx $41,$15,$11 
    0x180b47a8, // a $40,$15,$45 
    0x388b47a7, // lqx $39,$15,$45 
    0x42601213, // ila $19,c024 <main+0xad14> 
    0x3ac2c7aa, // cwx $42,$15,$11 
    0x3ac6079d, // cwx $29,$15,$24 
    0x3ac4c796, // cwx $22,$15,$19 
    0x3f83162b, // rotqbyi $43,$44,12 
    0x3b8a13a5, // rotqby $37,$39,$40 
    0x180ae526, // a $38,$74,$43 
    0x7f001280, // heqi $0,$37,0 
    0x12000492, // hbrr 12d8 <doTheMultThingy+0xb60>,12b4 <doTheMultThingy+0xb3c> # 12b4 
    0x54a01290, // clz $16,$37 
    0x54a0130c, // clz $12,$38 
    0x4080008d, // il $13,1 
    0x3280001a, // fsmbi $26,0 
    0x08040610, // sf $16,$12,$16 
    0x04001315, // ori $21,$38,0 
    0x0b64068d, // shl $13,$13,$16 
    0x0b64128c, // shl $12,$37,$16 
    0x08234d11, // or $17,$26,$13 
    0x3f3fc68d, // rotqmbii $13,$13,-1 
    0x5805460e, // clgt $14,$12,$21 
    0x00200000, // lnop 
    0x08054610, // sf $16,$12,$21 
    0x3f3fc60c, // rotqmbii $12,$12,-1 
    0x8346888e, // selb $26,$17,$26,$14 
    0x00200000, // lnop 
    0x82a5480e, // selb $21,$16,$21,$14 
    0x217ffb8d, // brnz $13,12b4 <doTheMultThingy+0xb3c> # 12b4 
    0xb42a532a, // shufb $33,$38,$41,$42 
    0x4020007f, // nop $127 
    0x127ef009, // hbrr 1308 <doTheMultThingy+0xb90>,a64 <doTheMultThingy+0x2ec> # a64 
    0x4020007f, // nop $127 
    0x2882c7a1, // stqx $33,$15,$11 
    0x38860799, // lqx $25,$15,$24 
    0xb1064d1d, // shufb $8,$26,$25,$29 
    0x28860788, // stqx $8,$15,$24 
    0x3884c794, // lqx $20,$15,$19 
    0xb1650a96, // shufb $11,$21,$20,$22 
    0x2884c78b, // stqx $11,$15,$19 
    0x327eeb80, // br a64 <doTheMultThingy+0x2ec> # a64 
    0x00200000, // lnop 
};

#define DOTHEMULTTHINGY_SIZE 2968


uint32_t main2[] __attribute__((aligned(128))) = 
{
    0x04000219, // ori $25,$4,0 
    0x3fe001a4, // shlqbyi $36,$3,0 
    0x40800204, // il $4,4 
    0x35900000, // hbrp 131c <main+0xc>,$0 
    0x4020007f, // nop $127 
    0x3fe002a5, // shlqbyi $37,$5,0 
    0x4080044b, // il $75,8 
    0x34004c86, // lqd $6,16($25) 
    0x40800645, // il $69,12 
    0x34000c90, // lqd $16,0($25) 
    0x40800a3a, // il $58,20 # 14 
    0x408000a7, // il $39,1 
    0x3f82033b, // rotqbyi $59,$6,8 
    0x34001d8e, // lqd $14,0($59) 
    0x3ec01dc4, // cwd $68,0($59) 
    0x3ec11dbf, // cwd $63,4($59) 
    0x3ec21dcf, // cwd $79,8($59) 
    0x35900000, // hbrp 1354 <main+0x44>,$0 
    0x3ec31dc9, // cwd $73,12($59) 
    0xb1438844, // shufb $10,$16,$14,$68 
    0x24001d8a, // stqd $10,0($59) 
    0x34000c89, // lqd $9,0($25) 
    0x38811d87, // lqx $7,$59,$4 
    0x3f810488, // rotqbyi $8,$9,4 
    0xb061c43f, // shufb $3,$8,$7,$63 
    0x28811d83, // stqx $3,$59,$4 
    0x34000c85, // lqd $5,0($25) 
    0x3892ddcd, // lqx $77,$59,$75 
    0x3f8202ce, // rotqbyi $78,$5,8 
    0xb993674f, // shufb $76,$78,$77,$79 
    0x2892ddcc, // stqx $76,$59,$75 
    0x34000cca, // lqd $74,0($25) 
    0x38915dc7, // lqx $71,$59,$69 
    0x3f832548, // rotqbyi $72,$74,12 
    0xb8d1e449, // shufb $70,$72,$71,$73 
    0x28915dc6, // stqx $70,$59,$69 
    0x34005dc2, // lqd $66,16($59) 
    0x34004cc3, // lqd $67,16($25) 
    0xb830a1c4, // shufb $65,$67,$66,$68 
    0x24005dc1, // stqd $65,16($59) 
    0x34004cc0, // lqd $64,16($25) 
    0x388e9dbd, // lqx $61,$59,$58 
    0x3f81203e, // rotqbyi $62,$64,4 
    0xb78f5f3f, // shufb $60,$62,$61,$63 
    0x288e9dbc, // stqx $60,$59,$58 
    0x21a00b27, // wrch $ch22,$39 
    0x42601832, // ila $50,c030 <main+0xad20> 
    0x34004cb8, // lqd $56,16($25) 
    0x340012b9, // lqd $57,0($37) 
    0x3f821c37, // rotqbyi $55,$56,8 
    0x3f821cb5, // rotqbyi $53,$57,8 
    0x180c9bb6, // a $54,$55,$50 
    0x21a00836, // wrch $ch16,$54 
    0x21a00935, // wrch $ch18,$53 
    0x40802828, // il $40,80 # 50 
    0x21a009a8, // wrch $ch19,$40 
    0x21a00a27, // wrch $ch20,$39 
    0x40802026, // il $38,64 # 40 
    0x21a00aa6, // wrch $ch21,$38 
    0x4080010b, // il $11,2 
    0x21a00b0b, // wrch $ch22,$11 
    0x21a00b8b, // wrch $ch23,$11 
    0x01a00c02, // rdch $2,$ch24 
    0x00200000, // lnop 
    0x1c141931, // ai $49,$50,80 # 50 
    0x34004cb3, // lqd $51,16($25) 
    0x340012b4, // lqd $52,0($37) 
    0x3f8219b0, // rotqbyi $48,$51,8 
    0x3f831a2e, // rotqbyi $46,$52,12 
    0x180c582f, // a $47,$48,$49 
    0x21a0082f, // wrch $ch16,$47 
    0x21a0092e, // wrch $ch18,$46 
    0x21a009a8, // wrch $ch19,$40 
    0x21a00a27, // wrch $ch20,$39 
    0x21a00aa6, // wrch $ch21,$38 
    0x21a00b0b, // wrch $ch22,$11 
    0x21a00b8b, // wrch $ch23,$11 
    0x01a00c03, // rdch $3,$ch24 
    0x4260682c, // ila $44,c0d0 <main+0xadc0> 
    0x34004cad, // lqd $45,16($25) 
    0x340052a9, // lqd $41,16($37) 
    0x3f8216ab, // rotqbyi $43,$45,8 
    0x180b15aa, // a $42,$43,$44 
    0x21a0082a, // wrch $ch16,$42 
    0x21a00929, // wrch $ch18,$41 
    0x21a009a8, // wrch $ch19,$40 
    0x21a00a27, // wrch $ch20,$39 
    0x21a00aa6, // wrch $ch21,$38 
    0x21a00b0b, // wrch $ch22,$11 
    0x21a00b8b, // wrch $ch23,$11 
    0x01a00c02, // rdch $2,$ch24 
    0x42603422, // ila $34,c068 <main+0xad58> 
    0x42609012, // ila $18,c120 <main+0xae10> 
    0x00200000, // lnop 
    0x42607e0c, // ila $12,c0fc <main+0xadec> 
    0x34004c8f, // lqd $15,16($25) 
    0x40800e0b, // il $11,28 # 1c 
    0x3f8207a3, // rotqbyi $35,$15,8 
    0x180891a1, // a $33,$35,$34 
    0x388891a0, // lqx $32,$35,$34 
    0x1803119f, // a $31,$35,$12 
    0x38849194, // lqx $20,$35,$18 
    0x0400118f, // ori $15,$35,0 
    0x3ac49196, // cwx $22,$35,$18 
    0x3883119e, // lqx $30,$35,$12 
    0x3882d18a, // lqx $10,$35,$11 
    0x3ec3118c, // cwd $12,12($35) 
    0x3b88501c, // rotqby $28,$32,$33 
    0x3b87cf11, // rotqby $17,$30,$31 
    0x0f608e1d, // shli $29,$28,2 
    0x78a74e1a, // mpyh $26,$28,$29 
    0x78a70e9b, // mpyh $27,$29,$28 
    0x79874e18, // mpyu $24,$28,$29 
    0x1806cd17, // a $23,$26,$27 
    0x18060b95, // a $21,$23,$24 
    0xb2650a96, // shufb $19,$21,$20,$22 
    0x28849193, // stqx $19,$35,$18 
    0x3400528d, // lqd $13,16($37) 
    0x3f830682, // rotqbyi $2,$13,12 
    0x7f000100, // heqi $0,$2,0 
    0x12000492, // hbrr 1538 <main+0x228>,1514 <main+0x204> # 1514 
    0x54a00109, // clz $9,$2 
    0x54a00885, // clz $5,$17 
    0x40800087, // il $7,1 
    0x3280000e, // fsmbi $14,0 
    0x08024289, // sf $9,$5,$9 
    0x04000890, // ori $16,$17,0 
    0x0b624387, // shl $7,$7,$9 
    0x0b624105, // shl $5,$2,$9 
    0x0821c70d, // or $13,$14,$7 
    0x3f3fc387, // rotqmbii $7,$7,-1 
    0x58040288, // clgt $8,$5,$16 
    0x00200000, // lnop 
    0x08040289, // sf $9,$5,$16 
    0x3f3fc285, // rotqmbii $5,$5,-1 
    0x81c38688, // selb $14,$13,$14,$8 
    0x00200000, // lnop 
    0x82040488, // selb $16,$9,$16,$8 
    0x217ffb87, // brnz $7,1514 <main+0x204> # 1514 
    0x1200070a, // hbrr 1564 <main+0x254>,1574 <main+0x264> # 1574 
    0x58090808, // clgt $8,$16,$36 
    0xb142870c, // shufb $10,$14,$10,$12 
    0x7c000807, // ceqi $7,$16,0 
    0x09220403, // nor $3,$8,$8 
    0x4020007f, // nop $127 
    0x00200000, // lnop 
    0x0820c386, // or $6,$7,$3 
    0x2882d18a, // stqx $10,$35,$11 
    0x4020007f, // nop $127 
    0x21000206, // brnz $6,1574 <main+0x264> # 1574 
    0x1c004711, // ai $17,$14,1 
    0xb042888c, // shufb $2,$17,$10,$12 
    0x2882d182, // stqx $2,$35,$11 
    0x4260100c, // ila $12,c020 <main+0xad10> 
    0x4089c43e, // il $62,5000 # 1388 
    0x00200000, // lnop 
    0x4080008b, // il $11,1 
    0x35900000, // hbrp 1584 <main+0x274>,$0 
    0x388307bd, // lqx $61,$15,$12 
    0x3ac307bf, // cwx $63,$15,$12 
    0xb78f5f3f, // shufb $60,$62,$61,$63 
    0x288307bc, // stqx $60,$15,$12 
    0x21a00b0b, // wrch $ch22,$11 
    0x42602c3a, // ila $58,c058 <main+0xad48> 
    0x34004cbb, // lqd $59,16($25) 
    0x3f821db9, // rotqbyi $57,$59,8 
    0x180e9cb8, // a $56,$57,$58 
    0x388e9cb7, // lqx $55,$57,$58 
    0x1c081cb6, // ai $54,$57,32 # 20 
    0x3b8e1b8d, // rotqby $13,$55,$56 
    0x35900000, // hbrp 15b8 <main+0x2a8>,$0 
    0x21a00836, // wrch $ch16,$54 
    0x21a0090d, // wrch $ch18,$13 
    0x40a00026, // il $38,16384 # 4000 
    0x21a009a6, // wrch $ch19,$38 
    0x21a00a0b, // wrch $ch20,$11 
    0x4080200d, // il $13,64 # 40 
    0x0400068f, // ori $15,$13,0 
    0x21a00a8f, // wrch $ch21,$15 
    0x42605434, // ila $52,c0a8 <main+0xad98> 
    0x40a01033, // il $51,16416 # 4020 
    0x34004cb5, // lqd $53,16($25) 
    0x4020007f, // nop $127 
    0x3f821ab2, // rotqbyi $50,$53,8 
    0x180d1931, // a $49,$50,$52 
    0x388d1930, // lqx $48,$50,$52 
    0x180cd92f, // a $47,$50,$51 
    0x3b8c582e, // rotqby $46,$48,$49 
    0x21a0082f, // wrch $ch16,$47 
    0x21a0092e, // wrch $ch18,$46 
    0x21a009a6, // wrch $ch19,$38 
    0x21a00a0b, // wrch $ch20,$11 
    0x21a00a8f, // wrch $ch21,$15 
    0x42607c2c, // ila $44,c0f8 <main+0xade8> 
    0x4240100e, // ila $14,8020 <main+0x6d10> 
    0x34004cad, // lqd $45,16($25) 
    0x4020007f, // nop $127 
    0x3f8216ab, // rotqbyi $43,$45,8 
    0x180b15aa, // a $42,$43,$44 
    0x388b15a9, // lqx $41,$43,$44 
    0x180395a8, // a $40,$43,$14 
    0x3b8a94a7, // rotqby $39,$41,$42 
    0x21a00828, // wrch $ch16,$40 
    0x21a00927, // wrch $ch18,$39 
    0x21a009a6, // wrch $ch19,$38 
    0x21a00a0b, // wrch $ch20,$11 
    0x21a00a8f, // wrch $ch21,$15 
    0x4080010a, // il $10,2 
    0x04000523, // ori $35,$10,0 
    0x21a00b23, // wrch $ch22,$35 
    0x21a00ba3, // wrch $ch23,$35 
    0x01a00c03, // rdch $3,$ch24 
    0x4080041f, // il $31,8 
    0x40800e1e, // il $30,28 # 1c 
    0x40800220, // il $32,4 
    0x34004ca2, // lqd $34,16($25) 
    0x4080040b, // il $11,8 
    0x3f821121, // rotqbyi $33,$34,8 
    0x1c02109a, // ai $26,$33,8 
    0x3887d097, // lqx $23,$33,$31 
    0x1c071096, // ai $22,$33,28 # 1c 
    0x38879095, // lqx $21,$33,$30 
    0x1c01109d, // ai $29,$33,4 
    0x34001094, // lqd $20,0($33) 
    0x0400108f, // ori $15,$33,0 
    0x3888109c, // lqx $28,$33,$32 
    0x3b868b98, // rotqby $24,$23,$26 
    0x3b858a92, // rotqby $18,$21,$22 
    0x3b884a13, // rotqby $19,$20,$33 
    0x3b874e1b, // rotqby $27,$28,$29 
    0x04000c17, // ori $23,$24,0 
    0x00200000, // lnop 
    0x0400099a, // ori $26,$19,0 
    0x2000f712, // brz $18,1e6c <main+0xb5c> # 1e6c 
    0x04000626, // ori $38,$12,0 
    0x32800021, // fsmbi $33,0 
    0x40801040, // il $64,32 # 20 
    0x3fe00db9, // shlqbyi $57,$27,0 
    0x4080028c, // il $12,5 
    0x3fe006bb, // shlqbyi $59,$13,0 
    0x0400053c, // ori $60,$10,0 
    0x3fe009b7, // shlqbyi $55,$19,0 
    0x0400069e, // ori $30,$13,0 
    0x3fe0051c, // shlqbyi $28,$10,0 
    0x0400072c, // ori $44,$14,0 
    0x3280001b, // fsmbi $27,0 
    0x408001a0, // il $32,3 
    0x3fe010b8, // shlqbyi $56,$33,0 
    0x4080021f, // il $31,4 
    0x3fe02023, // shlqbyi $35,$64,0 
    0x42601427, // ila $39,c028 <main+0xad18> 
    0x3fe0061d, // shlqbyi $29,$12,0 
    0x42608028, // ila $40,c100 <main+0xadf0> 
    0x42601229, // ila $41,c024 <main+0xad14> 
    0x4260302a, // ila $42,c060 <main+0xad50> 
    0x4260902b, // ila $43,c120 <main+0xae10> 
    0x408000b5, // il $53,1 
    0x42603436, // ila $54,c068 <main+0xad58> 
    0x180987c5, // a $69,$15,$38 
    0x388987c4, // lqx $68,$15,$38 
    0x4089c3c2, // il $66,4999 # 1387 
    0x35900000, // hbrp 1724 <main+0x414>,$0 
    0x3b916243, // rotqby $67,$68,$69 
    0x4810a1c1, // cgt $65,$67,$66 
    0x4020007f, // nop $127 
    0x200109c1, // brz $65,1f80 <main+0xc70> # 1f80 
    0x180a0785, // a $5,$15,$40 
    0x38898783, // lqx $3,$15,$38 
    0x3ac98784, // cwx $4,$15,$38 
    0x388a0787, // lqx $7,$15,$40 
    0x3ac9c7cd, // cwx $77,$15,$39 
    0x3aca47c9, // cwx $73,$15,$41 
    0xb9c0d204, // shufb $78,$36,$3,$4 
    0x3b8143cf, // rotqby $79,$7,$5 
    0x288987ce, // stqx $78,$15,$38 
    0x35900000, // hbrp 175c <main+0x44c>,$0 
    0x3889c7cb, // lqx $75,$15,$39 
    0x7f002780, // heqi $0,$79,0 
    0x12000492, // hbrr 17b0 <main+0x4a0>,178c <main+0x47c> # 178c 
    0x54a0278a, // clz $10,$79 
    0x54a0120e, // clz $14,$36 
    0x40800088, // il $8,1 
    0x3280004c, // fsmbi $76,0 
    0x0802870a, // sf $10,$14,$10 
    0x04001248, // ori $72,$36,0 
    0x0b628408, // shl $8,$8,$10 
    0x0b62a78e, // shl $14,$79,$10 
    0x0822260b, // or $11,$76,$8 
    0x3f3fc408, // rotqmbii $8,$8,-1 
    0x58120709, // clgt $9,$14,$72 
    0x00200000, // lnop 
    0x0812070a, // sf $10,$14,$72 
    0x3f3fc70e, // rotqmbii $14,$14,-1 
    0x89930589, // selb $76,$11,$76,$9 
    0x00200000, // lnop 
    0x89120509, // selb $72,$10,$72,$9 
    0x217ffb88, // brnz $8,178c <main+0x47c> # 178c 
    0xb952e64d, // shufb $74,$76,$75,$77 
    0x2889c7ca, // stqx $74,$15,$39 
    0x388a47c7, // lqx $71,$15,$41 
    0xb8d1e449, // shufb $70,$72,$71,$73 
    0x288a47c6, // stqx $70,$15,$41 
    0x1809c7a2, // a $34,$15,$39 
    0x3889c782, // lqx $2,$15,$39 
    0x180a8798, // a $24,$15,$42 
    0x35900000, // hbrp 17d4 <main+0x4c4>,$0 
    0x4020007f, // nop $127 
    0x388a8796, // lqx $22,$15,$42 
    0x180a4795, // a $21,$15,$41 
    0x388ac784, // lqx $4,$15,$43 
    0x180ac792, // a $18,$15,$43 
    0x388a4785, // lqx $5,$15,$41 
    0x3b888113, // rotqby $19,$2,$34 
    0x3b860b14, // rotqby $20,$22,$24 
    0x3b84824f, // rotqby $79,$4,$18 
    0x3b8542ce, // rotqby $78,$5,$21 
    0x78a50991, // mpyh $17,$19,$20 
    0x78a4ca06, // mpyh $6,$20,$19 
    0x7985098e, // mpyu $14,$19,$20 
    0x35900000, // hbrp 180c <main+0x4fc>,$0 
    0x18018889, // a $9,$17,$6 
    0x18038490, // a $16,$9,$14 
    0x0f60880a, // shli $10,$16,2 
    0x1803c503, // a $3,$10,$15 
    0x34008188, // lqd $8,32($3) # 20 
    0x3b80c407, // rotqby $7,$8,$3 
    0x21a00837, // wrch $ch16,$55 
    0x21a00907, // wrch $ch18,$7 
    0x21a009cf, // wrch $ch19,$79 
    0x04001c3a, // ori $58,$56,0 
    0x21a00a38, // wrch $ch20,$56 
    0x21a00a9e, // wrch $ch21,$30 
    0x40a01007, // il $7,16416 # 4020 
    0x00200000, // lnop 
    0x0f60a74c, // shli $76,$78,2 
    0x34004ccd, // lqd $77,16($25) 
    0x4020007f, // nop $127 
    0x3f8226cb, // rotqbyi $75,$77,8 
    0x1812e64a, // a $74,$76,$75 
    0x388ae5c8, // lqx $72,$75,$43 
    0x180ae5c9, // a $73,$75,$43 
    0x00200000, // lnop 
    0x1801e547, // a $71,$74,$7 
    0x3881e546, // lqx $70,$74,$7 
    0x3b926438, // rotqby $56,$72,$73 
    0x3b91e345, // rotqby $69,$70,$71 
    0x21a00839, // wrch $ch16,$57 
    0x21a00945, // wrch $ch18,$69 
    0x21a009b8, // wrch $ch19,$56 
    0x04000db8, // ori $56,$27,0 
    0x21a00a3a, // wrch $ch20,$58 
    0x21a00a9e, // wrch $ch21,$30 
    0x04000bba, // ori $58,$23,0 
    0x34004cc4, // lqd $68,16($25) 
    0x4020007f, // nop $127 
    0x3f82223f, // rotqbyi $63,$68,8 
    0x18099fc3, // a $67,$63,$38 
    0x38899fc2, // lqx $66,$63,$38 
    0x180adfc0, // a $64,$63,$43 
    0x388adf8c, // lqx $12,$63,$43 
    0x3b90e141, // rotqby $65,$66,$67 
    0x3b900632, // rotqby $50,$12,$64 
    0x0f60a0be, // shli $62,$65,2 
    0x180fdf3d, // a $61,$62,$63 
    0x180b1e8d, // a $13,$61,$44 
    0x388b1eb4, // lqx $52,$61,$44 
    0x3b835a33, // rotqby $51,$52,$13 
    0x21a0083a, // wrch $ch16,$58 
    0x21a00933, // wrch $ch18,$51 
    0x21a009b2, // wrch $ch19,$50 
    0x21a00a1d, // wrch $ch20,$29 
    0x21a00a9e, // wrch $ch21,$30 
    0x21a00b23, // wrch $ch22,$35 
    0x21a00b9c, // wrch $ch23,$28 
    0x01a00c03, // rdch $3,$ch24 
    0x34004cb1, // lqd $49,16($25) 
    0x4020007f, // nop $127 
    0x3f8218b0, // rotqbyi $48,$49,8 
    0x04001806, // ori $6,$48,0 
    0x388a982f, // lqx $47,$48,$42 
    0x4020007f, // nop $127 
    0x3b8a97ae, // rotqby $46,$47,$42 
    0x5c00570f, // clgti $15,$46,1 
    0x3fe0170c, // shlqbyi $12,$46,0 
    0x4020007f, // nop $127 
    0x20006b8f, // brz $15,1c70 <main+0x960> # 1c70 
    0x40800098, // il $24,1 
    0x3fe003b2, // shlqbyi $50,$7,0 
    0x04000d90, // ori $16,$27,0 
    0x00200000, // lnop 
    0x4260142d, // ila $45,c028 <main+0xad18> 
    0x3fe00c22, // shlqbyi $34,$24,0 
    0x4260582e, // ila $46,c0b0 <main+0xada0> 
    0x4260122f, // ila $47,c024 <main+0xad14> 
    0x408000b0, // il $48,1 
    0x42609031, // ila $49,c120 <main+0xae10> 
    0x42603433, // ila $51,c068 <main+0xad58> 
    0x42603034, // ila $52,c060 <main+0xad50> 
    0x40800007, // il $7,0 
    0x388b4348, // lqx $72,$6,$45 
    0x40800084, // il $4,1 
    0x35900000, // hbrp 1954 <main+0x644>,$0 
    0x4020007f, // nop $127 
    0x388b8347, // lqx $71,$6,$46 
    0x44004d9b, // xori $27,$27,1 
    0x34000338, // lqd $56,0($6) 
    0x3400434a, // lqd $74,16($6) 
    0x388bc346, // lqx $70,$6,$47 
    0x3b8b6443, // rotqby $67,$72,$45 
    0x3b8ba3c2, // rotqby $66,$71,$46 
    0x78069c0b, // ceq $11,$56,$26 
    0x3f831c45, // rotqbyi $69,$56,12 
    0x4020007f, // nop $127 
    0x3f811c44, // rotqbyi $68,$56,4 
    0x78b0c640, // mpyh $64,$12,$67 
    0x3b8be312, // rotqby $18,$70,$47 
    0x35900000, // hbrp 1990 <main+0x680>,$0 
    0x78a321c1, // mpyh $65,$67,$12 
    0x78b08c06, // mpyh $6,$24,$66 
    0x78a6210f, // mpyh $15,$66,$24 
    0x7990c63f, // mpyu $63,$12,$67 
    0x79908c3e, // mpyu $62,$24,$66 
    0x7c000593, // ceqi $19,$11,0 
    0x18106005, // a $5,$64,$65 
    0x89712513, // selb $75,$74,$68,$19 
    0x1803c31a, // a $26,$6,$15 
    0x180fc2b7, // a $55,$5,$63 
    0x180f8d11, // a $17,$26,$62 
    0x81610393, // selb $11,$7,$4,$19 
    0x834e2293, // selb $26,$69,$56,$19 
    0x180488ca, // a $74,$17,$18 
    0x18061b8e, // a $14,$55,$24 
    0x21a00b22, // wrch $ch22,$34 
    0x00200000, // lnop 
    0x0f60870a, // shli $10,$14,2 
    0x34004c89, // lqd $9,16($25) 
    0x04000d37, // ori $55,$26,0 
    0x3f820483, // rotqbyi $3,$9,8 
    0x1800c54e, // a $78,$10,$3 
    0x388c41cf, // lqx $79,$3,$49 
    0x180c4188, // a $8,$3,$49 
    0x3400a74d, // lqd $77,32($78) # 20 
    0x3b8227b9, // rotqby $57,$79,$8 
    0x3b93a6cc, // rotqby $76,$77,$78 
    0x21a00837, // wrch $ch16,$55 
    0x21a0094c, // wrch $ch18,$76 
    0x21a009b9, // wrch $ch19,$57 
    0x04000dc0, // ori $64,$27,0 
    0x21a00a40, // wrch $ch20,$64 
    0x21a00abb, // wrch $ch21,$59 
    0x0f60a548, // shli $72,$74,2 
    0x34004cc9, // lqd $73,16($25) 
    0x040025b9, // ori $57,$75,0 
    0x3f8224c7, // rotqbyi $71,$73,8 
    0x1811e446, // a $70,$72,$71 
    0x388c63b8, // lqx $56,$71,$49 
    0x180c63c5, // a $69,$71,$49 
    0x00200000, // lnop 
    0x180ca344, // a $68,$70,$50 
    0x388ca343, // lqx $67,$70,$50 
    0x3b915c41, // rotqby $65,$56,$69 
    0x3b9121c2, // rotqby $66,$67,$68 
    0x21a00839, // wrch $ch16,$57 
    0x21a00942, // wrch $ch18,$66 
    0x21a009c1, // wrch $ch19,$65 
    0x04000db8, // ori $56,$27,0 
    0x21a00a40, // wrch $ch20,$64 
    0x21a00abb, // wrch $ch21,$59 
    0x0b64180c, // shl $12,$48,$16 
    0x21a00b0c, // wrch $ch22,$12 
    0x21a00bbc, // wrch $ch23,$60 
    0x01a00c03, // rdch $3,$ch24 
    0x34004c8f, // lqd $15,16($25) 
    0x3f8207bf, // rotqbyi $63,$15,8 
    0x180cdfbe, // a $62,$63,$51 
    0x388cdfbd, // lqx $61,$63,$51 
    0x04001f8f, // ori $15,$63,0 
    0x3b8f9e8d, // rotqby $13,$61,$62 
    0x04000690, // ori $16,$13,0 
    0x2100218b, // brnz $11,1b98 <main+0x888> # 1b98 
    0x4080024d, // il $77,4 
    0x32800015, // fsmbi $21,0 
    0x1c011fcc, // ai $76,$63,4 
    0x32800014, // fsmbi $20,0 
    0x0f3f8691, // rotmi $17,$13,-2 
    0x38935fcb, // lqx $75,$63,$77 
    0x0f608696, // shli $22,$13,2 
    0x32800012, // fsmbi $18,0 
    0x4020007f, // nop $127 
    0x3b93258c, // rotqby $12,$75,$76 
    0x04000613, // ori $19,$12,0 
    0x2000158d, // brz $13,1b68 <main+0x858> # 1b68 
    0x0f610a89, // shli $9,$21,4 
    0x3280000e, // fsmbi $14,0 
    0x0f61088b, // shli $11,$17,4 
    0x00200000, // lnop 
    0x18024b8a, // a $10,$23,$9 
    0x20001011, // brz $17,1b54 <main+0x844> # 1b54 
    0x0f610703, // shli $3,$14,4 
    0x12000597, // hbrr 1b38 <main+0x828>,1b08 <main+0x7f8> # 1b08 
    0x04000906, // ori $6,$18,0 
    0x340007cf, // lqd $79,0($15) 
    0x415fc00d, // ilhu $13,49024 # bf80 
    0x34000508, // lqd $8,0($10) 
    0x4281018c, // ila $12,10203 <main+0xeef3> 
    0x3fe00805, // shlqbyi $5,$16,0 
    0x1800c987, // a $7,$19,$3 
    0x3b83e7ce, // rotqby $78,$79,$15 
    0x04002709, // ori $9,$78,0 
    0x00200000, // lnop 
    0x180184c1, // a $65,$9,$6 
    0x388184c0, // lqx $64,$9,$6 
    0x1cffc285, // ai $5,$5,-1 
    0x34000384, // lqd $4,0($7) 
    0x1c010306, // ai $6,$6,4 
    0x1802c387, // a $7,$7,$11 
    0x3b90603f, // rotqby $63,$64,$65 
    0x58c35fbe, // fm $62,$63,$13 
    0xb7af9f0c, // shufb $61,$62,$62,$12 
    0xe0411e88, // fma $2,$61,$4,$8 
    0x04000108, // ori $8,$2,0 
    0x24000502, // stqd $2,0($10) 
    0x217ffa05, // brnz $5,1b08 <main+0x7f8> # 1b08 
    0x1c00470e, // ai $14,$14,1 
    0x78044708, // ceq $8,$14,$17 
    0x21000188, // brnz $8,1b50 <main+0x840> # 1b50 
    0x1c04050a, // ai $10,$10,16 
    0x327ff180, // br 1ad8 <main+0x7c8> # 1ad8 
    0x18044a95, // a $21,$21,$17 
    0x1c004a14, // ai $20,$20,1 
    0x7805080a, // ceq $10,$16,$20 
    0x2100018a, // brnz $10,1b68 <main+0x858> # 1b68 
    0x18058912, // a $18,$18,$22 
    0x327feb80, // br 1ac0 <main+0x7b0> # 1ac0 
    0x180d0793, // a $19,$15,$52 
    0x127fbb8a, // hbrr 1b94 <main+0x884>,1948 <main+0x638> # 1948 
    0x1c004c18, // ai $24,$24,1 
    0x388d0792, // lqx $18,$15,$52 
    0x04000786, // ori $6,$15,0 
    0x3b84c911, // rotqby $17,$18,$19 
    0x58060890, // clgt $16,$17,$24 
    0x3fe0088c, // shlqbyi $12,$17,0 
    0x20001d10, // brz $16,1c70 <main+0x960> # 1c70 
    0x04000d90, // ori $16,$27,0 
    0x4020007f, // nop $127 
    0x327fb680, // br 1948 <main+0x638> # 1948 
    0x0f3f8691, // rotmi $17,$13,-2 
    0x34005f87, // lqd $7,16($63) 
    0x0f608696, // shli $22,$13,2 
    0x32800015, // fsmbi $21,0 
    0x40800014, // il $20,0 
    0x32800012, // fsmbi $18,0 
    0x4020007f, // nop $127 
    0x3b8fc38e, // rotqby $14,$7,$63 
    0x04000713, // ori $19,$14,0 
    0x207ff58d, // brz $13,1b68 <main+0x858> # 1b68 
    0x0f610acb, // shli $75,$21,4 
    0x3280000e, // fsmbi $14,0 
    0x0f61088c, // shli $12,$17,4 
    0x00200000, // lnop 
    0x1812cb8a, // a $10,$23,$75 
    0x20001111, // brz $17,1c5c <main+0x94c> # 1c5c 
    0x40800606, // il $6,12 
    0x12000699, // hbrr 1c40 <main+0x930>,1c10 <main+0x900> # 1c10 
    0x1c030785, // ai $5,$15,12 
    0x34000509, // lqd $9,0($10) 
    0x0f61070d, // shli $13,$14,4 
    0x388187c3, // lqx $67,$15,$6 
    0x4281018b, // ila $11,10203 <main+0xeef3> 
    0x3fe00906, // shlqbyi $6,$18,0 
    0x04000807, // ori $7,$16,0 
    0x18034988, // a $8,$19,$13 
    0x415fc00d, // ilhu $13,49024 # bf80 
    0x3b8161c2, // rotqby $66,$67,$5 
    0x04002105, // ori $5,$66,0 
    0x00200000, // lnop 
    0x180182ca, // a $74,$5,$6 
    0x388182c9, // lqx $73,$5,$6 
    0x1cffc387, // ai $7,$7,-1 
    0x34000446, // lqd $70,0($8) 
    0x1c010306, // ai $6,$6,4 
    0x18030408, // a $8,$8,$12 
    0x3b92a4c8, // rotqby $72,$73,$74 
    0x58c36447, // fm $71,$72,$13 
    0xb8b1e38b, // shufb $69,$71,$71,$11 
    0xe891a289, // fma $68,$69,$70,$9 
    0x04002209, // ori $9,$68,0 
    0x24000544, // stqd $68,0($10) 
    0x217ffa07, // brnz $7,1c10 <main+0x900> # 1c10 
    0x1c00470e, // ai $14,$14,1 
    0x7803888b, // ceq $11,$17,$14 
    0x2100018b, // brnz $11,1c58 <main+0x948> # 1c58 
    0x1c04050a, // ai $10,$10,16 
    0x327ff080, // br 1bd8 <main+0x8c8> # 1bd8 
    0x18044a95, // a $21,$21,$17 
    0x1c004a14, // ai $20,$20,1 
    0x7805080c, // ceq $12,$16,$20 
    0x217fe08c, // brnz $12,1b68 <main+0x858> # 1b68 
    0x18058912, // a $18,$18,$22 
    0x327fea80, // br 1bc0 <main+0x8b0> # 1bc0 
    0x0b66dab2, // shl $50,$53,$27 
    0x21a00b32, // wrch $ch22,$50 
    0x21a00b9c, // wrch $ch23,$28 
    0x01a00c02, // rdch $2,$ch24 
    0x34004cb1, // lqd $49,16($25) 
    0x3f8218b0, // rotqbyi $48,$49,8 
    0x0400180f, // ori $15,$48,0 
    0x3400182f, // lqd $47,0($48) 
    0x3b8c17ae, // rotqby $46,$47,$48 
    0x7806972d, // ceq $45,$46,$26 
    0x4020007f, // nop $127 
    0x21003fad, // brnz $45,1e98 <main+0xb88> # 1e98 
    0x180d9846, // a $70,$48,$54 
    0x388d9845, // lqx $69,$48,$54 
    0x40800015, // il $21,0 
    0x3400580d, // lqd $13,16($48) 
    0x40800014, // il $20,0 
    0x32800012, // fsmbi $18,0 
    0x3b91a2c4, // rotqby $68,$69,$70 
    0x3b8c0687, // rotqby $7,$13,$48 
    0x0f3fa211, // rotmi $17,$68,-2 
    0x3fe02210, // shlqbyi $16,$68,0 
    0x0f60a216, // shli $22,$68,2 
    0x3fe00393, // shlqbyi $19,$7,0 
    0x4020007f, // nop $127 
    0x200016c4, // brz $68,1d88 <main+0xa78> # 1d88 
    0x0f610a8b, // shli $11,$21,4 
    0x3280000e, // fsmbi $14,0 
    0x0f61088c, // shli $12,$17,4 
    0x00200000, // lnop 
    0x1805c58a, // a $10,$11,$23 
    0x20001111, // brz $17,1d74 <main+0xa64> # 1d74 
    0x4080060d, // il $13,12 
    0x12000699, // hbrr 1d58 <main+0xa48>,1d28 <main+0xa18> # 1d28 
    0x1c030785, // ai $5,$15,12 
    0x34000509, // lqd $9,0($10) 
    0x0f610707, // shli $7,$14,4 
    0x388347c3, // lqx $67,$15,$13 
    0x04000906, // ori $6,$18,0 
    0x415fc00d, // ilhu $13,49024 # bf80 
    0x4281018b, // ila $11,10203 <main+0xeef3> 
    0x00200000, // lnop 
    0x1801c988, // a $8,$19,$7 
    0x3fe00807, // shlqbyi $7,$16,0 
    0x3b8161c2, // rotqby $66,$67,$5 
    0x04002105, // ori $5,$66,0 
    0x1801434a, // a $74,$6,$5 
    0x38814349, // lqx $73,$6,$5 
    0x1cffc387, // ai $7,$7,-1 
    0x34000446, // lqd $70,0($8) 
    0x1c010306, // ai $6,$6,4 
    0x18030408, // a $8,$8,$12 
    0x3b92a4c8, // rotqby $72,$73,$74 
    0x58c36447, // fm $71,$72,$13 
    0xb8b1e38b, // shufb $69,$71,$71,$11 
    0xe891a289, // fma $68,$69,$70,$9 
    0x04002209, // ori $9,$68,0 
    0x24000544, // stqd $68,0($10) 
    0x217ffa07, // brnz $7,1d28 <main+0xa18> # 1d28 
    0x1c00470e, // ai $14,$14,1 
    0x78044706, // ceq $6,$14,$17 
    0x21000186, // brnz $6,1d70 <main+0xa60> # 1d70 
    0x1c04050a, // ai $10,$10,16 
    0x327ff080, // br 1cf0 <main+0x9e0> # 1cf0 
    0x18044a95, // a $21,$21,$17 
    0x1c004a14, // ai $20,$20,1 
    0x7805080a, // ceq $10,$16,$20 
    0x2100018a, // brnz $10,1d88 <main+0xa78> # 1d88 
    0x18058912, // a $18,$18,$22 
    0x327fea80, // br 1cd8 <main+0x9c8> # 1cd8 
    0x18098785, // a $5,$15,$38 
    0x388987c3, // lqx $67,$15,$38 
    0x180ac789, // a $9,$15,$43 
    0x127f30b4, // hbrr 1e64 <main+0xb54>,1718 <main+0x408> # 1718 
    0x388ac784, // lqx $4,$15,$43 
    0x3b8161c2, // rotqby $66,$67,$5 
    0x3b82423d, // rotqby $61,$4,$9 
    0x0f60a108, // shli $8,$66,2 
    0x1803c441, // a $65,$8,$15 
    0x00200000, // lnop 
    0x180b20c0, // a $64,$65,$44 
    0x388b20bf, // lqx $63,$65,$44 
    0x3b901fbe, // rotqby $62,$63,$64 
    0x21a0083a, // wrch $ch16,$58 
    0x21a0093e, // wrch $ch18,$62 
    0x21a009bd, // wrch $ch19,$61 
    0x04001002, // ori $2,$32,0 
    0x21a00a02, // wrch $ch20,$2 
    0x21a00aa3, // wrch $ch21,$35 
    0x0b67da83, // shl $3,$53,$31 
    0x21a00b03, // wrch $ch22,$3 
    0x21a00b9c, // wrch $ch23,$28 
    0x01a00c02, // rdch $2,$ch24 
    0x40800a0f, // il $15,20 # 14 
    0x40800e18, // il $24,28 # 1c 
    0x00200000, // lnop 
    0x7c00d02e, // ceqi $46,$32,3 
    0x34004ccf, // lqd $79,16($25) 
    0x4080044e, // il $78,8 
    0x7c001715, // ceqi $21,$46,0 
    0x7c00171f, // ceqi $31,$46,0 
    0x1c0050a1, // ai $33,$33,1 
    0x04000fa0, // ori $32,$31,0 
    0x00200000, // lnop 
    0x40800190, // il $16,3 
    0x3f8227cc, // rotqbyi $76,$79,8 
    0x4080024b, // il $75,4 
    0x3f822794, // rotqbyi $20,$79,8 
    0x4080020c, // il $12,4 
    0x4080018e, // il $14,3 
    0x1c022634, // ai $52,$76,8 
    0x3893a633, // lqx $51,$76,$78 
    0x1c050a31, // ai $49,$20,20 # 14 
    0x3883ca30, // lqx $48,$20,$15 
    0x81f30a15, // selb $15,$20,$76,$21 
    0x84038620, // selb $32,$12,$14,$32 
    0x1c0707ad, // ai $45,$15,28 # 1c 
    0x38860793, // lqx $19,$15,$24 
    0x3b8d19b2, // rotqby $50,$51,$52 
    0x3b8c582f, // rotqby $47,$48,$49 
    0x3b8b4997, // rotqby $23,$19,$45 
    0x58084bba, // clgt $58,$23,$33 
    0x82ec979f, // selb $23,$47,$50,$31 
    0x00200000, // lnop 
    0x83f2c81f, // selb $31,$16,$75,$31 
    0x217f16ba, // brnz $58,1718 <main+0x408> # 1718 
    0x0b681a8b, // shl $11,$53,$32 
    0x3580000a, // hbr 1e94 <main+0xb84>,$0 
    0x040005a4, // ori $36,$11,0 
    0x4020007f, // nop $127 
    0x4020007f, // nop $127 
    0x21a00b24, // wrch $ch22,$36 
    0x40800119, // il $25,2 
    0x21a00b99, // wrch $ch23,$25 
    0x01a00c02, // rdch $2,$ch24 
    0x40800003, // il $3,0 
    0x4020007f, // nop $127 
    0x35000000, // bi $0 
    0x40800214, // il $20,4 
    0x388d9815, // lqx $21,$48,$54 
    0x180d984c, // a $76,$48,$54 
    0x32800016, // fsmbi $22,0 
    0x1c011818, // ai $24,$48,4 
    0x38851834, // lqx $52,$48,$20 
    0x40800013, // il $19,0 
    0x32800014, // fsmbi $20,0 
    0x3b930aa2, // rotqby $34,$21,$76 
    0x3b861a33, // rotqby $51,$52,$24 
    0x04001111, // ori $17,$34,0 
    0x0f3f9110, // rotmi $16,$34,-2 
    0x0f609115, // shli $21,$34,2 
    0x3fe01992, // shlqbyi $18,$51,0 
    0x4020007f, // nop $127 
    0x207fd6a2, // brz $34,1d88 <main+0xa78> # 1d88 
    0x0f610b09, // shli $9,$22,4 
    0x3280000e, // fsmbi $14,0 
    0x0f61080b, // shli $11,$16,4 
    0x00200000, // lnop 
    0x1805c48a, // a $10,$9,$23 
    0x20001010, // brz $16,1f6c <main+0xc5c> # 1f6c 
    0x0f61074f, // shli $79,$14,4 
    0x12000597, // hbrr 1f50 <main+0xc40>,1f20 <main+0xc10> # 1f20 
    0x04000986, // ori $6,$19,0 
    0x340007ce, // lqd $78,0($15) 
    0x415fc00d, // ilhu $13,49024 # bf80 
    0x34000508, // lqd $8,0($10) 
    0x4281018c, // ila $12,10203 <main+0xeef3> 
    0x3fe00885, // shlqbyi $5,$17,0 
    0x1813c907, // a $7,$18,$79 
    0x3b83e74d, // rotqby $77,$78,$15 
    0x04002689, // ori $9,$77,0 
    0x00200000, // lnop 
    0x18024340, // a $64,$6,$9 
    0x3882433f, // lqx $63,$6,$9 
    0x1cffc285, // ai $5,$5,-1 
    0x340003bd, // lqd $61,0($7) 
    0x1c010306, // ai $6,$6,4 
    0x1802c387, // a $7,$7,$11 
    0x3b901fbe, // rotqby $62,$63,$64 
    0x58c35f04, // fm $4,$62,$13 
    0xb041020c, // shufb $2,$4,$4,$12 
    0xe06f4108, // fma $3,$2,$61,$8 
    0x04000188, // ori $8,$3,0 
    0x24000503, // stqd $3,0($10) 
    0x217ffa05, // brnz $5,1f20 <main+0xc10> # 1f20 
    0x1c00470e, // ai $14,$14,1 
    0x78040741, // ceq $65,$14,$16 
    0x210001c1, // brnz $65,1f68 <main+0xc58> # 1f68 
    0x1c04050a, // ai $10,$10,16 
    0x327ff180, // br 1ef0 <main+0xbe0> # 1ef0 
    0x18040b16, // a $22,$22,$16 
    0x1c004a14, // ai $20,$20,1 
    0x78050888, // ceq $8,$17,$20 
    0x217fc288, // brnz $8,1d88 <main+0xa78> # 1d88 
    0x18054993, // a $19,$19,$21 
    0x327feb80, // br 1ed8 <main+0xbc8> # 1ed8 
    0x180a07a2, // a $34,$15,$40 
    0x340052ad, // lqd $45,16($37) 
    0x388a0798, // lqx $24,$15,$40 
    0x38898794, // lqx $20,$15,$38 
    0x3ac98795, // cwx $21,$15,$38 
    0x3ac9c792, // cwx $18,$15,$39 
    0x3aca4789, // cwx $9,$15,$41 
    0x3f83168b, // rotqbyi $11,$45,12 
    0x3b888c16, // rotqby $22,$24,$34 
    0x1802e182, // a $2,$67,$11 
    0x7f000b00, // heqi $0,$22,0 
    0x12000492, // hbrr 1ff4 <main+0xce4>,1fd0 <main+0xcc0> # 1fd0 
    0x54a00b0a, // clz $10,$22 
    0x54a0010c, // clz $12,$2 
    0x4080008d, // il $13,1 
    0x32800011, // fsmbi $17,0 
    0x0802860a, // sf $10,$12,$10 
    0x04000110, // ori $16,$2,0 
    0x0b62868d, // shl $13,$13,$10 
    0x0b628b0c, // shl $12,$22,$10 
    0x0823488b, // or $11,$17,$13 
    0x3f3fc68d, // rotqmbii $13,$13,-1 
    0x5804060e, // clgt $14,$12,$16 
    0x00200000, // lnop 
    0x0804060a, // sf $10,$12,$16 
    0x3f3fc60c, // rotqmbii $12,$12,-1 
    0x8224458e, // selb $17,$11,$17,$14 
    0x00200000, // lnop 
    0x8204050e, // selb $16,$10,$16,$14 
    0x217ffb8d, // brnz $13,1fd0 <main+0xcc0> # 1fd0 
    0xb2650115, // shufb $19,$2,$20,$21 
    0x127ef989, // hbrr 2020 <main+0xd10>,17c8 <main+0x4b8> # 17c8 
    0x4020007f, // nop $127 
    0x28898793, // stqx $19,$15,$38 
    0x3889c786, // lqx $6,$15,$39 
    0xb1c18892, // shufb $14,$17,$6,$18 
    0x2889c78e, // stqx $14,$15,$39 
    0x388a478a, // lqx $10,$15,$41 
    0xb1028809, // shufb $8,$16,$10,$9 
    0x288a4788, // stqx $8,$15,$41 
    0x327ef500, // br 17c8 <main+0x4b8> # 17c8 
    0x00200000, // lnop 
};

#define MAIN_SIZE 3352


