#ifndef __SGER_LU_H_
#define __SGER_LU_H_

unsigned int sger_lu[] __attribute__((aligned(16))) = { 1082949705U, 872432154U, 1083768903U, 898629632U, 1075839103U, 872415944U, 1084588102U, 620691585U, 67109418U, 1071645361U, 485490817U, 1071645070U, 1065487657U, 1065493519U, 67114138U, 898629632U, 403854505U, 403819833U, 403803425U, 564135962U, 564136207U, 1082140731U, 564136379U, 1082130490U, 564136506U, 1082138639U, 67110793U, 564136585U, 67114179U, 872415940U, 1065558594U, 564136003U, 564136258U, 564136379U, 564136506U, 564136585U, 67116224U, 872432321U, 1065427135U, 564136000U, 564136255U, 564136379U, 564136506U, 564136585U, 67113149U, 872432318U, 1065492284U, 564135997U, 564136252U, 564136379U, 564136506U, 564136585U, 1082130579U, 564136723U, 1082130706U, 564136850U, 27266050U, 1082965916U, 403116216U, 872451383U, 1075839103U, 872453301U, 350231580U, 872455347U, 1065556918U, 1065556660U, 1065556397U, 1065490854U, 470014770U, 470014511U, 352065838U, 470013599U, 352065452U, 257988395U, 352063400U, 257988133U, 257987609U, 403361311U, 403263387U, 67112359U, 403066264U, 564135975U, 564136230U, 1082130964U, 564136340U, 564136467U, 564136585U, 564136722U, 564136850U, 27266050U, 470286499U, 948522916U, 998691362U, 564135971U, 564136226U, 564136340U, 564136467U, 564136585U, 564136722U, 564136850U, 27266050U, 2097152U, 67111966U, 872452256U, 1065488413U, 564135966U, 564136221U, 564136340U, 564136467U, 564136585U, 564136722U, 564136850U, 27266051U, 2097152U, 470548630U, 948325783U, 998640533U, 564135958U, 564136213U, 564136340U, 564136467U, 564136585U, 564136722U, 564136850U, 27266050U, 1115750796U, 1084227593U, 872451339U, 872467720U, 872464525U, 1075839103U, 872448131U, 67110288U, 1065551242U, 1075839103U, 1065485318U, 469780101U, 1065486343U, 1075839103U, 3015754124U, 469812497U, 1071648259U, 2130707200U, 301991058U, 1419772684U, 1419772552U, 1082130570U, 847249451U, 134415372U, 67109506U, 191038730U, 191038216U, 136484237U, 1061143818U, 1476428811U, 2097152U, 134251532U, 1061143560U, 2238367371U, 2097152U, 2151712267U, 562035594U, 352258180U, 67109768U, 257982980U, 1476542978U, 1075839103U, 536874626U, 1088421776U, 301991704U, 134284495U, 847249486U, 402915842U, 1071645831U, 2080384973U, 1071645834U, 255623500U, 1071646603U, 1082131275U, 847249414U, 67118473U, 2303960653U, 469787909U, 403112707U, 564135939U, 402785041U, 564136209U, 564136330U, 564136457U, 564136587U, 486523525U, 134284164U, 402768646U, 562035461U, 402752520U, 402755075U, 1075839103U, 553725060U, 1084227589U, 872453268U, 67112836U, 898629632U, 1065552403U, 1065486866U, 469813645U, 67111177U, 352257676U, 257984006U, 1476477704U, 536875144U, 1088421793U, 847249435U, 898629632U, 2097152U, 134316701U, 301991704U, 403194656U, 1071645319U, 2080378521U, 1071645323U, 255627288U, 847249413U, 1082131223U, 1082138645U, 67111818U, 2097152U, 2194066457U, 1071647368U, 469781252U, 403161763U, 564135971U, 402801314U, 564136226U, 564136331U, 564136458U, 564136584U, 486523396U, 134316934U, 402768517U, 562035460U, 402801289U, 402739076U, 1075839103U, 553720966U, 402888088U, 872467758U, 1065424685U, 898629632U, 1476793998U, 536931726U, 1082138672U, 847249461U, 1082134575U, 2097152U, 1082130740U, 1071650853U, 1082130995U, 1071650856U, 1082130610U, 898629632U, 1075839103U, 1071650849U, 67115043U, 1071650727U, 67114925U, 1071651494U, 1084227628U, 1071651504U, 67115682U, 1071651492U, 67115552U, 1071651357U, 67115439U, 1071651118U, 872467727U, 67110795U, 564136744U, 898629632U, 564136861U, 27266051U, 872469700U, 872420675U, 1065493056U, 1065427394U, 898629632U, 2130714624U, 301991058U, 1419780114U, 1419774990U, 1082130576U, 847249471U, 134514450U, 67111943U, 191137808U, 191143950U, 136585091U, 1061144592U, 1476511505U, 2097152U, 134334226U, 1061144334U, 2280636817U, 2097152U, 2162280721U, 562035600U, 2097152U, 257990721U, 1071653129U, 67117322U, 257990588U, 2024824893U, 2024808638U, 2039504955U, 2097152U, 403168825U, 948428344U, 403676858U, 999185463U, 403627270U, 1477116726U, 67115912U, 536874422U, 1088421835U, 301991318U, 1084227591U, 847249480U, 403882826U, 847249413U, 134317001U, 1071645577U, 255632710U, 2080384199U, 2293375815U, 469787268U, 402817741U, 564136013U, 402784972U, 564136268U, 564136329U, 564136480U, 564136609U, 486523396U, 134316934U, 2097152U, 402768517U, 562035332U, 402736136U, 402736393U, 1075839103U, 553699462U, 2023934998U, 872467737U, 2024146327U, 898629632U, 872420635U, 2038615061U, 1075839103U, 1065487501U, 67112329U, 1071648138U, 403032852U, 402999827U, 257984146U, 402969996U, 257984027U, 898629632U, 2024048264U, 2023966982U, 2038728331U, 2097152U, 403099143U, 948358660U, 402752517U, 998359555U, 402834053U, 1477116561U, 67109256U, 2097152U, 536874513U, 2097152U, 1088421815U, 301991318U, 1084227591U, 847249460U, 403554998U, 847249414U, 134300597U, 1071645577U, 255630130U, 2080381619U, 2177702195U, 469780228U, 402752825U, 564135993U, 402752568U, 564136248U, 564136329U, 564136482U, 564136611U, 486523396U, 134300549U, 2097152U, 402768646U, 562035332U, 402752520U, 402752777U, 553693701U, 872467729U, 872464528U, 872420687U, 898629632U, 1065486466U, 1065494466U, 898629632U, 2130706688U, 301991058U, 1419772171U, 1419773960U, 1082130569U, 847249485U, 134398987U, 67110988U, 191022217U, 191021320U, 136472204U, 1061143689U, 1477641226U, 2097152U, 135463947U, 1061143560U, 2310227466U, 2097152U, 2308113802U, 562035593U, 257982730U, 2024040138U, 2025015499U, 2038720201U, 2023925318U, 2024998215U, 2038605327U, 1477117244U, 403891528U, 403858501U, 403825476U, 403789251U, 402907709U, 257991105U, 403742342U, 2097152U, 403120320U, 948379839U, 1075839103U, 999301054U, 403676805U, 536874172U, 1088421780U, 301991318U, 1084227591U, 847249421U, 402982163U, 2097152U, 134382482U, 1071645576U, 255625611U, 2080377100U, 2156086668U, 469778948U, 67109654U, 564135958U, 67109525U, 564136213U, 564136328U, 564136484U, 564136613U, 486523396U, 402768646U, 402768517U, 134382474U, 562035332U, 553681162U, 301992860U, 564136751U, 564136861U, 27266051U, 872469686U, 872420661U, 1065491251U, 1065425588U, 2024593842U, 2039273870U, 1071651333U, 403478809U, 402885766U, 255820547U, 2097152U, 536872707U, 2097152U, 2080375225U, 847249464U, 486523274U, 847249412U, 2263745849U, 469785475U, 486523267U, 947995194U, 1489476873U, 679559689U, 470024708U, 562036099U, 2013973563U, 847249421U, 553681595U, 564136750U, 564136861U, 27266050U, 1544537792U, 872467727U, 872464587U, 872420677U, 872469704U, 1065486281U, 336586186U, 2097152U, 67117721U, 1065493186U, 258090310U, 1065493575U, 2130715776U, 301991058U, 1419781262U, 1419781513U, 1082130570U, 847249416U, 134448270U, 67118531U, 191071498U, 191079561U, 136479760U, 1061143818U, 1477493900U, 2097152U, 135316622U, 1061143689U, 2164393996U, 2097152U, 2289092364U, 562035594U, 67118223U, 1065427652U, 403808534U, 258089409U, 403726613U, 553663680U, 2024153990U, 2024918070U, 2024924085U, 2039604147U, 2039598132U, 469788035U, 258049683U, 2130715776U, 301991058U, 1419781262U, 1419772305U, 1082130578U, 847249415U, 134449294U, 67109298U, 191072530U, 191079569U, 136610704U, 1061144850U, 1477216395U, 2097152U, 135039118U, 1061144721U, 2162280459U, 2097152U, 2253162251U, 562035602U, 258016903U, 402758418U, 403528331U, 403507479U, 403490184U, 469817605U, 2130707456U, 301991058U, 1419772939U, 1419774865U, 1082130578U, 847249417U, 134400139U, 67111828U, 191023378U, 191022097U, 136610958U, 1061144850U, 1476724874U, 2097152U, 134547595U, 1061144721U, 2166507274U, 2097152U, 2189755786U, 562035602U, 336576972U, 352256654U, 403900817U, 257985040U, 402883460U, 257984718U, 258015821U, 257984130U, 402924111U, 257992211U, 258017044U, 402696070U, 403793672U, 403793543U, 134456850U, 1477642001U, 1543751440U, 1115750807U, 872416012U, 301990931U, 998344247U, 67115916U, 536873489U, 402967178U, 1071645705U, 67111179U, 2097152U, 486524299U, 872416446U, 872416575U, 1052771515U, 998399805U, 998416316U, 469828874U, 3880715837U, 3071253819U, 603980984U, 469828745U, 562035339U, 872416009U, 998343818U, 67110156U, 301990798U, 2973959703U, 553649808U, 403016201U, 1071645572U, 201590533U, 2097152U, 486523525U, 872416449U, 470025353U, 872415810U, 3892486338U, 603980352U, 470024708U, 562035845U, 310373513U, 469780109U, 1075839103U, 2081425091U, 1075839103U, 553648835U, 469828358U, 473957384U, 473957255U, 847243264U, 257984398U, 1071647880U, 403115462U, 948374981U, 2023982998U, 2023999257U, 2038663061U, 999400132U, 67117575U, 403065613U, 402998916U, 1477116443U, 1075839103U, 536874139U, 1088421836U, 301991318U, 1084227590U, 847249423U, 403898955U, 2097152U, 134284106U, 1071645449U, 255632840U, 2080384329U, 2296636489U, 469787525U, 67109966U, 564136014U, 67109837U, 564136269U, 564136329U, 564136486U, 564136615U, 486523525U, 402752520U, 402752391U, 134284036U, 562035333U, 310322443U, 553649796U, 564136750U, 564136861U, 27266050U, 872437891U, 872467732U, 1065550215U, 1065421331U, 402770968U, 1476790660U, 561979268U, 470810753U, 889192448U, 67109905U, 310376842U, 564135953U, 67109776U, 564136208U, 470008322U, 352059727U, 564136399U, 564136496U, 564136621U, 1075839103U, 847246464U, 67109655U, 310345482U, 564135959U, 67109512U, 564136200U, 470009093U, 352060039U, 564136327U, 564136496U, 564136616U, 1075839103U, 847215104U, 67110032U, 310327178U, 564135952U, 67109890U, 564136194U, 470008655U, 352069582U, 564136398U, 564136477U, 564136616U, 1075839103U, 847196800U, 67110075U, 310332937U, 564135995U, 67109946U, 564136250U, 470008457U, 352060554U, 564136330U, 564136496U, 564136616U, 847202688U, 872467774U, 872464575U, 1065492284U, 469786557U, 2130714112U, 301991058U, 1419779602U, 1419779726U, 1082130576U, 847249412U, 134514450U, 67116685U, 191137808U, 191143438U, 136577541U, 1061144592U, 1476609809U, 2097152U, 134432530U, 1061144334U, 2155938449U, 2097152U, 2174961937U, 562035600U, 847213056U, 67109259U, 310301580U, 564135947U, 67109898U, 564136202U, 470008325U, 352060038U, 564136326U, 1082131207U, 564136455U, 1082138628U, 564136580U, 1075839103U, 847170944U, 67109420U, 310305675U, 564135980U, 67110056U, 564136232U, 470008615U, 352064422U, 564136358U, 1082131237U, 564136485U, 1082138660U, 564136612U, 847175168U, 2097152U };

unsigned int sger_lu_size = 3304; // In bytes

#endif /* __SGER_LU_H_ */