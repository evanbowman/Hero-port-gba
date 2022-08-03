if(argument0==999){
if (global.normalrush)
    return (scr_omnitext(351)+" _"+scr_omnitext(319)+"\");
else
    return (scr_omnitext(351)+" _"+scr_omnitext(320)+"\");}
if(argument0==457){return global.omnitext[457]+string_lower(scr_button(0))+"/"+string_lower(scr_button(1))+".";}
else if(argument0==509){return global.omnitext[509]+string(foreverlevel);}
else if(argument0==462 && !global.retromode){return global.omnitext[28]}
else if(argument0==463 && !global.retromode){return global.omnitext[29]}
else if(argument0==464 && !global.retromode){return global.omnitext[30]}
else return global.omnitext[argument0];