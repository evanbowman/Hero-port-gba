if (joywait>=20 && !nodraw){
    nodraw=1;
    if (selected==0){
        alarm[0]=40;
        scr_save(2);
        scr_sound("save");
        instance_create(200,200,obj_explospewer2);
        }
    else{
        alarm[0]=20;
        scr_sound("pain");
        //instance_create(200,200,obj_explospewer);
        }
    }