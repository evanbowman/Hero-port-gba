global.bladelook=image_single;
if (other.sprite_index==spr_eshot || other.sprite_index==spr_shot){
    with (other){
        tempid=instance_create(x,y,obj_shot);
        tempid.direction=180-global.bladelook*180;
        tempid.speed=4;
        tempid.nocount=1;
        instance_create(x,y,obj_explo);
        instance_destroy();
        }
    }
else if ((other.sprite_index==spr_esupershot || other.sprite_index==spr_ebomb) && global.blade>=2){
    with (other){
        tempid=instance_create(x,y-4,obj_shot);
        tempid.direction=180-global.bladelook*180;
        tempid.speed=4;
        tempid.nocount=1;
        tempid=instance_create(x,y+4,obj_shot);
        tempid.direction=180-global.bladelook*180;
        tempid.speed=4;
        tempid.nocount=1;
        instance_create(x,y,obj_explo);
        instance_destroy();
        }
    }
else if (other.sprite_index==spr_emegashot && global.blade>=2){
    with (other){
        tempid=instance_create(x,y-6,obj_shot);
        tempid.direction=180-global.bladelook*180;
        tempid.speed=4;
        tempid.nocount=1;
        tempid=instance_create(x,y+6,obj_shot);
        tempid.direction=180-global.bladelook*180;
        tempid.speed=4;
        tempid.nocount=1;
        tempid=instance_create(x-6+global.bladelook*12,y,obj_shot);
        tempid.direction=180-global.bladelook*180;
        tempid.speed=4;
        tempid.nocount=1;
        instance_create(x,y,obj_explo);
        instance_destroy();
        }
    }
else if ((other.sprite_index==spr_egigashot || other.sprite_index==spr_evortex || other.sprite_index==spr_elaser) && global.blade==3){
    obj_sabot.explo[2]=1;
    with (other){
        if (sprite_index!=spr_evortex){
            tempid=instance_create(x,y-6,obj_shot);
            tempid.direction=180-global.bladelook*180;
            tempid.speed=4;
            tempid.nocount=1;
            tempid=instance_create(x,y+6,obj_shot);
            tempid.direction=180-global.bladelook*180;
            tempid.speed=4;
            tempid.nocount=1;
            tempid=instance_create(x-6,y,obj_shot);
            tempid.direction=180-global.bladelook*180;
            tempid.speed=4;
            tempid.nocount=1;
            tempid=instance_create(x+6,y,obj_shot);
            tempid.direction=180-global.bladelook*180;
            tempid.speed=4;
            tempid.nocount=1;
            }
        instance_create(x,y,obj_bigexplo);
        instance_destroy();
        }
    }
