keyboard_unset_map();
keyboard_set_map(ord("Z"),vk_end);
keyboard_set_map(ord("X"),vk_end);
keyboard_set_map(global.fireleft,ord("Z"));
keyboard_set_map(global.fireright,ord("X"));
keyboard_set_map(ord("1"),global.fireleft);
keyboard_set_map(ord("2"),global.fireright);
keyboard_set_map(vk_numpad1,global.fireleft);
keyboard_set_map(vk_numpad2,global.fireright);