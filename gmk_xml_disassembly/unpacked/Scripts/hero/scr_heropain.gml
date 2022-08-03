//argument0 is damage
//argument1 is heat induced
//argument2 is instant damage (barrier, moving wall)
//argument3 is additional paintime

if (pain==0 || (argument2==1 && superpain==0)){
    with (other){
        if (sprite_index==spr_emissile){
            instance_destroy();
            instance_create(x,y,obj_explo);
            }
        else if (sprite_index==spr_eshot || sprite_index==spr_esupershot || sprite_index==spr_shot)
            instance_destroy();
        }
    pain=30+argument3;
    if (argument2==1)
        superpain=30;
    obj_sabot.shakescreen=6;
    if (global.suit==3)
        global.hp-=argument0/2;
    else
        global.hp-=argument0;
    if (global.suit<2)
        global.heat+=argument1;
    if (global.heat>global.maxheat)
        global.heat=global.maxheat;
    //Death
    if (global.hp<=0){
        if (global.forever){
            global.foreverbomb=0;
            global.hypergun=0;
            obj_sabot.foreverinit=0;
            if (obj_sabot.flags>global.bestflags)
                global.bestflags=obj_sabot.flags;
            scr_foreverdestroy();
            }
        scr_wantsong(0);
        global.autofire=0;
        global.hp=0;
        instance_destroy();
        instance_create(x,y,obj_explospewer);
        if (global.bossmode)
            obj_sabot.alarm[0]=20;
        else
            obj_sabot.alarm[0]=80;
        scr_sound("death");
        obj_sabot.explo[2]=1;
        obj_sabot.explo[3]=1;
        with (obj_annihilator){
            timeline_index=-1;
            speed=0;
            image_single=1;
            spreadfire=0;
            sfcyc=0;
            flash=0;
            }
        with (obj_tetron){
            if (!phase3){
                if (hp<=0)
                    hp=256;
                timeline_index=-1;
                speed=0;
                image_single=2;
                obj_tetronneck.visible=1;
                obj_tetronneck.image_single=1;
                }
            }
        }
    else
        scr_sound("pain");
    }