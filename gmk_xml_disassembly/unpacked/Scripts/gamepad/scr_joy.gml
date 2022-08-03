if (joystick_exists(1)){
//argument0 1 is directions only
if (argument0!=1){
    //BUTTONS
    for (i=1;i<imaxx;i+=1){
        if (joystick_check_button(1,i)==1){
            if (!joy[i] && !joyhold[i])
                joy[i]=1;
            else
                joy[i]=0;
            joyhold[i]=1;
            }
        else{
            joy[i]=0;
            joyhold[i]=0;
            }
        }
    }

//D-PAD
if ((joyhold[global.joyr] && argument0!=1) || joystick_direction(1)==vk_numpad3 || joystick_direction(1)==vk_numpad6 || joystick_direction(1)==vk_numpad9){
    if (!joy[33] && !joyhold[33])
        joy[33]=1;
    else
        joy[33]=0;
    joyhold[33]=1;
    }
else{
    joy[33]=0;
    joyhold[33]=0;
    }
if ((joyhold[global.joyu] && argument0!=1) || joystick_direction(1)==vk_numpad7 || joystick_direction(1)==vk_numpad8 || joystick_direction(1)==vk_numpad9){
    if (!joy[34] && !joyhold[34])
        joy[34]=1;
    else
        joy[34]=0;
    joyhold[34]=1;
    }
else{
    joy[34]=0;
    joyhold[34]=0;
    }
if ((joyhold[global.joyl] && argument0!=1) || joystick_direction(1)==vk_numpad1 || joystick_direction(1)==vk_numpad4 || joystick_direction(1)==vk_numpad7){
    if (!joy[35] && !joyhold[35])
        joy[35]=1;
    else
        joy[35]=0;
    joyhold[35]=1;
    }
else{
    joy[35]=0;
    joyhold[35]=0;
    }
if ((joyhold[global.joyd] && argument0!=1) || joystick_direction(1)==vk_numpad1 || joystick_direction(1)==vk_numpad2 || joystick_direction(1)==vk_numpad3){
    if (!joy[36] && !joyhold[36])
        joy[36]=1;
    else
        joy[36]=0;
    joyhold[36]=1;
    }
else{
    joy[36]=0;
    joyhold[36]=0;
    }

//ACTIONS
if (argument0!=1){
    if (joy[global.joyz])
        event_perform(ev_keypress,ord("Z"));
    if (joy[global.joyx])
        event_perform(ev_keypress,ord("X"));
    if (joy[global.joyauto])
        event_perform(ev_keypress,vk_space);
    if (joy[global.joypause]){
        if (room==rom_title)
            event_perform(ev_keypress,vk_enter);
        else{
            if (obj_sabot.pause)
                event_perform(ev_keypress,vk_enter);
            else
                event_perform(ev_keypress,vk_escape);
            }
        }
    if (joy[global.joymap])
        event_perform(ev_keypress,vk_enter);
    }

if (joy[33])
    event_perform(ev_keypress,vk_right);
if (joy[34])
    event_perform(ev_keypress,vk_up);
if (joy[35])
    event_perform(ev_keypress,vk_left);
if (joy[36])
    event_perform(ev_keypress,vk_down);
}