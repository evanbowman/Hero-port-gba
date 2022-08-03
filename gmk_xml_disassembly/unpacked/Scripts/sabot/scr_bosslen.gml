if (!pause){
bosslen=0;
bosslenmax=1;
//Eliminator
if (instance_number(obj_eliminator)==1){
    bosslenmax=64;
    obj_sabot.bosslen+=obj_eliminator.hp;
    }
//Rock smasher
if (instance_number(obj_smasher)==1){
    bosslenmax=256;
    with (obj_smasherneck){
        if (sprite_index==spr_silencercore)
            obj_sabot.bosslen+=hp;
        }
    }
//Silencer
if (instance_number(obj_silencer)==1){
    bosslenmax=32;
    with (obj_silencerneck){
        if (sprite_index==spr_silencercore)
            obj_sabot.bosslen=hp;
        }
    }
//Hydlas
if (instance_number(obj_hydlas)==1){
    if (global.hardmode)
        bosslenmax=12;
    else
        bosslenmax=6;
    bosslen=bosslenmax-obj_hydlas.headkill;
    }

//Reaper drone
if (instance_number(obj_reaperdrone)==1){
    bosslenmax=64;
    bosslen=obj_reaperdrone.hp;
    }

//Liquid metal processor
if (instance_number(obj_processor)==1){
    bosslenmax=128;
    with (obj_processorneck){
        if (sprite_index==spr_silencercore)
            obj_sabot.bosslen+=hp;
        }
    }

//Grand mother
if (instance_number(obj_grandmother)==1){
    bosslenmax=128;
    if (obj_grandmother.walls<4)
        bosslen=64-obj_grandmother.walls*16;
    with (obj_grandmotherneck){
        if (sprite_index==spr_silencercore)
            obj_sabot.bosslen+=hp;
        }
    }

//Guardian
if (instance_number(obj_guardian)==1){
    bosslenmax=128;
    bosslen=obj_guardian.hp;
    if (obj_guardian.drain>3)
        bosslen+=obj_guardian.hp2;
    }

//Star splitter
if (instance_number(obj_dread)==1){
    bosslenmax=128+global.hardmode*128;
    with (obj_dreadneck){
        if (sprite_index==spr_silencercore)
            obj_sabot.bosslen+=hp;
        }
    if (obj_dread.dead1)
        bosslen=obj_dread.hp;
    }

//Elite
if (instance_number(obj_elite)>0){
    if (global.difficulty==2 && !global.bossmode)
        bosslenmax=128;
    else
        bosslenmax=256;
    with (obj_elite)
        obj_sabot.bosslen+=hp;
    }
//Tetron
if (instance_number(obj_tetron)==1){
    bosslenmax=128;
    if (obj_tetron.hp>128)
        bosslen=obj_tetron.hp-128;
    else if (obj_tetron.hp>64)
        bosslen=obj_tetron.hp-64;
    else
        bosslen=obj_tetron.hp;
    }
//Living Warmachine
if (instance_number(obj_warmachine)==1){
    bosslenmax=256;
    obj_sabot.bosslen+=obj_warmachine.hp;
    }

if (bosslen!=bosslenprev)
    bossdraw=10;
if (bossdraw>0)
    bossdraw-=1;
bosslenprev=bosslen;
bosslen2=ceil(((bosslen/bosslenmax)*38)/2)*2;
}