if (joywait>=20 && !nodraw){
    yoff=32;
    scr_ctext(200,120+yoff,scr_omnitext(527));
    scr_ctext(200,152+yoff,scr_omnitext(528));
    scr_ctext(200,168+yoff,scr_omnitext(529));
    scr_ctext(200,184+yoff,scr_omnitext(581));

    if (selected==0){
        scr_drawtext(172,216+yoff,">"+scr_omnitext(372),16,400,spr_text,32,1);
        scr_drawtext(172,232+yoff," "+scr_omnitext(373),16,400,spr_text,32,1);
        }
    else{
        scr_drawtext(172,216+yoff," "+scr_omnitext(372),16,400,spr_text,32,1);
        scr_drawtext(172,232+yoff,">"+scr_omnitext(373),16,400,spr_text,32,1);
        }
    }