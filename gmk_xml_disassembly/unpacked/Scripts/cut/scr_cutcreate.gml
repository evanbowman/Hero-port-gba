scr_joycreate();
if (global.cut==2 || global.cut==6)
    scr_wantsong(15);
else if (global.cut!=4 && global.cut!=3)
    scr_wantsong(12);
gamesabot=0;
text1="";
text2="";
text3="";
textcyc=0;
prevtext1="";
prevtext2="";
prevtext3="";
ignoreall=0;
ignoreenter=1;
chat=0;
y2=16;
scr_cutpics();

//INTRO
if (global.cut==1){
chats=4;
texts1[0]=scr_omnitext(1);
texts2[0]=scr_omnitext(2);
texts3[0]=scr_omnitext(3);
texts1[1]=scr_omnitext(4);
texts2[1]=scr_omnitext(5);
texts3[1]=scr_omnitext(6);
texts1[2]=scr_omnitext(7);
texts2[2]=scr_omnitext(8);
texts3[2]=scr_omnitext(9);
texts1[3]=scr_omnitext(10);
texts2[3]=scr_omnitext(11);
texts3[3]=scr_omnitext(12);
}

//VICTORY
if (global.cut==2){
chats=4;
texts1[0]=scr_omnitext(13);
texts2[0]=scr_omnitext(14);
texts3[0]=scr_omnitext(15);
texts1[1]=scr_omnitext(16);
texts2[1]=scr_omnitext(17);
texts3[1]=scr_omnitext(18);
texts1[2]=scr_omnitext(19);
texts2[2]=scr_omnitext(20);
texts3[2]=scr_omnitext(21);
texts1[3]=scr_omnitext(22);
texts2[3]=scr_omnitext(23);
texts3[3]=scr_omnitext(24);
}

//PERFECT VICTORY
if (global.cut==3){
chats=4;
texts1[0]=scr_omnitext(25);
texts2[0]=scr_omnitext(26);
texts3[0]=scr_omnitext(27);
//texts1[1]=scr_omnitext(28);
//texts2[1]=scr_omnitext(29);
//texts3[1]=scr_omnitext(30);
texts1[1]=scr_omnitext(31);
texts2[1]=scr_omnitext(32);
texts3[1]=scr_omnitext(33);
texts1[2]=scr_omnitext(34);
texts2[2]=scr_omnitext(35);
texts3[2]=scr_omnitext(36);
texts1[3]=scr_omnitext(37);
texts2[3]=scr_omnitext(38);
texts3[3]=scr_omnitext(39);
}

//CREDITS
if (global.cut==4){
chats=1;
texts1[0]="";
texts2[0]="";
texts3[0]="";
texts1[1]="";
texts2[1]="";
texts3[1]="";
}

//HIDDEN DIFFICULTY INTRO
if (global.cut==5){
chats=4;
texts1[0]=scr_omnitext(40);
texts2[0]=scr_omnitext(41);
texts3[0]=scr_omnitext(42);
texts1[1]=scr_omnitext(43);
texts2[1]=scr_omnitext(44);
texts3[1]=scr_omnitext(45);
texts1[2]=scr_omnitext(46);
texts2[2]=scr_omnitext(47);
texts3[2]=scr_omnitext(48);
texts1[3]=scr_omnitext(49);
texts2[3]=scr_omnitext(50);
texts3[3]=scr_omnitext(51);
}

//HIDDEN DIFFICULTY OUTRO
if (global.cut==6){
chats=5;
texts1[0]=scr_omnitext(52);
texts2[0]=scr_omnitext(53);
texts3[0]=scr_omnitext(54);
texts1[1]=scr_omnitext(55);
texts2[1]=scr_omnitext(56);
texts3[1]=scr_omnitext(57);
texts1[2]=scr_omnitext(58);
texts2[2]=scr_omnitext(59);
texts3[2]=scr_omnitext(60);
texts1[3]=scr_omnitext(61);
texts2[3]=scr_omnitext(62);
texts3[3]=scr_omnitext(63);
texts1[4]=scr_omnitext(64);
texts2[4]=scr_omnitext(65);
texts3[4]=scr_omnitext(66);
}

if (global.reallyplay){
//INTRO
if (global.cut==1){
chats=4;
texts1[0]=scr_omnitext(67);
texts2[0]=scr_omnitext(68);
texts3[0]=scr_omnitext(69);
texts1[1]=scr_omnitext(70);
texts2[1]=scr_omnitext(71);
texts3[1]=scr_omnitext(72);
texts1[2]=scr_omnitext(73);
texts2[2]=scr_omnitext(74);
texts3[2]=scr_omnitext(75);
texts1[3]=scr_omnitext(76);
texts2[3]=scr_omnitext(77);
texts3[3]=scr_omnitext(78);
}
}