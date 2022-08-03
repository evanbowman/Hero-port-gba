global.gmversion=7; //5 or 7

//GM5
/*
//Set window
if (argument0==1){
    if (global.window==0){
        full_screen=0;
        scale_window=100;
        }
    else if (global.window==1){
        full_screen=0;
        scale_window=200;
        }
    else if (global.window==2){
        full_screen=1;
        scale_window=0;
        }
    }
//Draw alpha background
else if (argument0==2)
    draw_sprite_ext(spr_bggrid,0,x,y,1,1,alpha);
//Do nothing
else if (argument0==3){
    }
//Tetron explodes
else if (argument0==4){
    with (obj_bgdraw)
        instance_destroy();
    }
//Do nothing
else if (argument0==5){
    }
//Do nothing
else if (argument0==6){
    }
*/
//GM7
//Set window
if (argument0==1){
    if (global.window==0){
        window_set_fullscreen(0);
        window_set_region_scale(1,1);
        window_center();
        }
    else if (global.window==1){
        window_set_fullscreen(0);
        window_set_region_scale(2,1);
        window_center();
        }
    else if (global.window==2){
        window_set_fullscreen(1);
        window_set_region_scale(-1,0);
        }
    }
//Draw background
else if (argument0==2)
    draw_sprite(spr_bggrid,0,x,y);
//Motion blur
else if (argument0==3){
    if (visible){
        if (sprite_index==spr_lurk){
            tempid=instance_create(round(x/2)*2,(round(y/2)*2)-obj_sabot.shake,obj_blur);
            tempid.sprite_index=sprite_index;
            if (wake)
                tempid.img=anim+8;
            else
                tempid.img=image_single;
            tempid.alpha=1-obj_bgdraw.alpha;
            for (i=1;i<24;i+=1){
                tempid=instance_create(round(x/2)*2,(round((y+i*16)/2)*2)-obj_sabot.shake,obj_blur);
                tempid.sprite_index=sprite_index;
                img=anim+24-i;
                while (img>7)
                    img-=8;
                if (img<0)
                    img+=8;
                tempid.img=img;
                tempid.alpha=1-obj_bgdraw.alpha;
                }
            }
        else if (sprite_index==spr_hydlashead){
            if (deadcyc<75){
                tempid=instance_create(round((x+(xdiff/5)*5)/2)*2,(round((y+(ydiff/5)*5)/2)*2)-obj_sabot.shake,obj_blur);
                tempid.sprite_index=spr_neck;
                tempid.img=0;
                tempid.alpha=1-obj_bgdraw.alpha;
                }
            if (deadcyc<60){
                tempid=instance_create(round((x+(xdiff/5)*4)/2)*2,(round((y+(ydiff/5)*4.6)/2)*2)-obj_sabot.shake,obj_blur);
                tempid.sprite_index=spr_neck;
                tempid.img=0;
                tempid.alpha=1-obj_bgdraw.alpha;
                }
            if (deadcyc<45){
                tempid=instance_create(round((x+(xdiff/5)*3)/2)*2,(round((y+(ydiff/5)*4)/2)*2)-obj_sabot.shake,obj_blur);
                tempid.sprite_index=spr_neck;
                tempid.img=0;
                tempid.alpha=1-obj_bgdraw.alpha;
                }
            if (deadcyc<30){
                tempid=instance_create(round((x+(xdiff/5)*2)/2)*2,(round((y+(ydiff/5)*3)/2)*2)-obj_sabot.shake,obj_blur);
                tempid.sprite_index=spr_neck;
                tempid.img=0;
                tempid.alpha=1-obj_bgdraw.alpha;
                }
            if (deadcyc<15){
                tempid=instance_create(round((x+(xdiff/5)*1)/2)*2,(round((y+(ydiff/5)*1.5)/2)*2)-obj_sabot.shake,obj_blur);
                tempid.sprite_index=spr_neck;
                tempid.img=0;
                tempid.alpha=1-obj_bgdraw.alpha;
                }
            if (deadcyc==0){
                tempid=instance_create(round(x/2)*2,(round(y/2)*2)-obj_sabot.shake,obj_blur);
                tempid.sprite_index=sprite_index;
                tempid.img=image_single;
                tempid.alpha=1-obj_bgdraw.alpha;
                }
            }
        else if (sprite_index==spr_lurkhitbox){
            for (i=0;i<24;i+=1){
                img=anim+24-i
                while (img>7)
                    img-=8;
                if (img<0)
                    img+=8;
                tempid=instance_create(round(x/2)*2,(round((y+i*16)/2)*2)-obj_sabot.shake,obj_blur);
                tempid.sprite_index=spr_lurk;
                tempid.img=img;
                tempid.alpha=1-obj_bgdraw.alpha;
                tempid=instance_create(round((x-2)/2)*2,(round((y+i*16)/2)*2)-obj_sabot.shake,obj_blur);
                tempid.sprite_index=spr_lurk;
                tempid.img=img-1;
                tempid.alpha=1-obj_bgdraw.alpha;
                tempid=instance_create(round((x+2)/2)*2,(round((y+i*16)/2)*2)-obj_sabot.shake,obj_blur);
                tempid.sprite_index=spr_lurk;
                tempid.img=img+1;
                tempid.alpha=1-obj_bgdraw.alpha;
                }
            }
        else if (!noblur){
            tempid=instance_create(round(x/2)*2,(round(y/2)*2)-obj_sabot.shake,obj_blur);
            tempid.sprite_index=sprite_index;
            if (image_single>-1)
                tempid.img=image_single;
            else
                tempid.img=image_index;
            tempid.alpha=1-obj_bgdraw.alpha;
            }
        }
    }
//Tetron explodes
else if (argument0==4){
    obj_sabot.zeroalpha=1;
    obj_sabot.tetcirx=floor(round(obj_tetron.x/2)*2);
    obj_sabot.tetciry=floor(round(obj_tetron.y/2)*2);
    }
//Draw blur object
else if (argument0==5)
    draw_sprite_ext(sprite_index,img,x,y,1,1,0,c_white,alpha);
//Draw Tetron circle
else if (argument0==6){
    draw_set_color(c_white);
    tetcirr2+=1;
    if (tetcirr2==2){
        tetcirr2=0;
        tetcirr+=4;
        }
    draw_rectangle(tetcirx-tetcirr,tetciry-tetcirr,tetcirx+tetcirr+1,tetciry+tetcirr+1,0);
    }