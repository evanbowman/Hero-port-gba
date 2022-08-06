import xml.etree.ElementTree as ET
import sys
import math
import re



if sys.argv[1] == 'normal':
    src = 'rom_quest1.xml'
elif sys.argv[1] == 'hard':
    src = 'rom_quest2.xml'
else:
    raise Exception("specify normal or hard in argv[1]")


tree = ET.parse(src)

root = tree.getroot()


tilemap = []
rooms = []
zones = []


for i in range(0, 300):
    sublist = []
    for j in range(0, 300):
        sublist.append(0)
    tilemap.append(sublist)


for i in range(0, 15):
    rlist = []
    for j in range(0, 15):
        rlist.append([])
    rooms.append(rlist)


for i in range(0, 15):
    rlist = []
    for j in range(0, 15):
        rlist.append(0)
    zones.append(rlist)



def set_wall(x, y):
    if x < 300 and y < 300:
        tilemap[x][y] = 1



def parse_instances(xml_list):

    # NOTE: The game's original author created level maps as one large Room in
    # gamemaker, which, in gamemaker, is a sensible thing to do; you wouldn't
    # want to keep track of 300 different rooms, I suppose. But makes our job
    # of generating level data for the gba more difficult. We generate one
    # giant tilemap by rendering all of the map's walls to a huge matrix, then
    # we slice the matrix into 20x20 rooms.
    #
    # Most blocks are not 1x1, so we need to render multiple tiles per block.

    for inst in xml_list:
        objname = inst[0].text
        x = int(inst[1].attrib["x"])
        y = int(inst[1].attrib["y"])
        tx = int(x / 16)
        ty = int(y / 16)

        rx = int(math.floor(tx / 20))
        ry = int(math.floor(ty / 20))
        rix = int((x - rx * 20 * 16) / 16)
        riy = int((y - ry * 20 * 16) / 16)

        ra_x = int((x - rx * 20 * 16) / 2)
        ra_y = int((y - ry * 20 * 16) / 2)

        if objname == "obj_block":
            set_wall(tx, ty)
        elif objname == "obj_blocktall":
            set_wall(tx, ty)
            set_wall(tx, ty + 1)
            set_wall(tx, ty + 2)
            set_wall(tx, ty + 3)
        elif objname == "obj_blockwide":
            set_wall(tx, ty)
            set_wall(tx + 1, ty)
            set_wall(tx + 2, ty)
            set_wall(tx + 3, ty)
        elif objname == "obj_blockmed":
            for x in range(0, 2):
                for y in range(0, 2):
                    set_wall(tx + x, ty + y)
        elif objname == "obj_blockbig":
            for x in range(0, 4):
                for y in range(0, 4):
                    set_wall(tx + x, ty + y)
        elif objname == "obj_blocktall2":
            for x in range(0, 3):
                for y in range(0, 6):
                    set_wall(tx + x, ty + y)
        elif objname == "obj_blockwide2":
            for x in range(0, 6):
                for y in range(0, 3):
                    set_wall(tx + x, ty + y)
        elif objname == "obj_blockwide3":
            for x in range(0, 10):
                set_wall(tx + x, ty)
        elif objname == "obj_blockhuge":
            for x in range(0, 6):
                for y in range(0, 6):
                    if tx + x < 300 and ty + y < 300:
                        tilemap[tx + x][ty + y] = 1
        elif objname == "obj_blocktall3":
            for y in range(0, 10):
                set_wall(tx, ty + y)
        elif objname == "obj_dirt" or objname == "obj_dirt_s":
            tilemap[tx][ty] = 6
        elif objname == "obj_dirtbig_s":
            for x in range(0, 3):
                for y in range(0, 3):
                    if tilemap[tx + x][ty + y] != 1:
                        tilemap[tx + x][ty + y] = 6
        elif objname == "obj_pipe1_s":
            if tilemap[tx][ty] != 1:
                tilemap[tx][ty] = 7
        elif objname == "obj_pipe2_s":
            if tilemap[tx][ty] != 1:
                tilemap[tx][ty] = 8
        elif objname == "obj_pipe1long_s":
            for y in range(0, 4):
                if y < 300 and tilemap[tx][ty + y] != 1:
                    tilemap[tx][ty + y] = 7
        elif objname == "obj_pipe2long_s":
            for x in range(0, 4):
                if x < 300 and tilemap[tx + x][ty] != 1:
                    tilemap[tx + x][ty] = 8
        elif objname == "obj_drone_s":
            rooms[rx][ry].append([1, ra_x, ra_y])
        elif objname == "obj_reaver_s":
            rooms[rx][ry].append([2, rix, riy])
        elif objname == "obj_spew_s":
            rooms[rx][ry].append([3, rix, riy])
        elif objname == "obj_crusher_s":
            rooms[rx][ry].append([4, rix, riy])
        elif objname == "obj_blomb_s":
            rooms[rx][ry].append([5, rix, riy])
        elif objname == "obj_bolt_s":
            rooms[rx][ry].append([6, rix, riy])
        elif objname == "obj_turretleft_s":
            rooms[rx][ry].append([7, rix, riy])
        elif objname == "obj_turretright_s":
            rooms[rx][ry].append([8, rix, riy])
        elif objname == "obj_boltaray_s":
            rooms[rx][ry].append([9, rix, riy])
        elif objname == "obj_sharte_s":
            rooms[rx][ry].append([10, rix, riy])
        elif objname == "obj_elite_s":
            rooms[rx][ry].append([11, rix, riy])
        # NOTE: there's a typo in Daniel's code.
        elif objname == "obj_powerturretlerft_s":
            rooms[rx][ry].append([12, rix, riy])
        elif objname == "obj_powerturretright_s":
            rooms[rx][ry].append([13, rix, riy])
        elif objname == "obj_lurk_s":
            rooms[rx][ry].append([14, rix, riy])
        elif objname == "obj_weed":
            rooms[rx][ry].append([15, ra_x, ra_y])
        elif objname == "obj_weed2":
            rooms[rx][ry].append([16, ra_x, ra_y])
        elif objname == "obj_generator_s":
            rooms[rx][ry].append([17, rix, riy])
        elif objname == "obj_barrier_s":
            rooms[rx][ry].append([18, rix, riy])
        elif objname == "obj_barrier2_s":
            rooms[rx][ry].append([19, rix, riy])
        elif objname == "obj_liquid3long_s":
            for y in range(0, 4):
                if y < 300 and tilemap[tx][ty + y] != 1:
                    tilemap[tx][ty + y] = 25
            rooms[rx][ry].append([20, ra_x, ra_y])
        elif objname == "obj_liquid1long_s":
            for x in range(0, 4):
                if tx + x < 300:
                    prev = tilemap[tx + x][ty]
                    if x < 300 and prev != 1 and prev != 25:
                        tilemap[tx + x][ty] = 9
            rooms[rx][ry].append([21, ra_x, ra_y])
        elif objname == "obj_liquid3_s":
            if tilemap[tx][ty] != 1:
                tilemap[tx][ty] = 25
            rooms[rx][ry].append([23, ra_x, ra_y])
        elif objname == "obj_liquid1_s":
            prev = tilemap[tx][ty]
            if prev != 1 and prev != 25:
                tilemap[tx][ty] = 9
            rooms[rx][ry].append([23, ra_x, ra_y])
        elif objname == "obj_liquid2_s":
            prev = tilemap[tx][ty]
            if prev != 1 and prev != 25 and prev != 9:
                tilemap[tx][ty] = 17
            rooms[rx][ry].append([23, ra_x, ra_y])
        elif objname == "obj_liquid2big_s":
            for x in range(0, 3):
                for y in range(0, 3):
                    if tx + x < 300 and ty + y < 300:
                        prev = tilemap[tx + x][ty + y]
                        if prev != 1 and prev != 25 and prev != 9:
                            tilemap[tx + x][ty + y] = 17
            rooms[rx][ry].append([22, ra_x, ra_y])
        elif objname == "obj_door1_s":
            tilemap[tx][ty] = 33
            tilemap[tx][ty + 1] = 34
        elif objname == "obj_door2_s":
            tilemap[tx][ty] = 37
            tilemap[tx + 1][ty] = 38
        elif objname == "obj_savepoint_s":
            rooms[rx][ry].append([24, ra_x, ra_y])
        elif objname == "obj_reaperdrone_s":
            rooms[rx][ry].append([25, ra_x, ra_y])
        elif objname == "obj_star":
            rooms[rx][ry].append([26, ra_x, ra_y])
        elif objname == "obj_antenna":
            if tilemap[tx][ty - 1] == 0: # Idk why these object jump down a slot...
                tilemap[tx][ty - 1] = 41
        elif objname == "obj_antenna2":
            if tilemap[tx][ty - 1] == 0:
                tilemap[tx][ty - 1] = 45
        elif objname == "obj_loc":
            # Sigh. The gamemaker objects set zone number via creation code
            # in gml attached to the room data. I guess we'll have to parse gml.
            # Thankfully, the author uses a regular coding style...
            code = inst[2].text
            znstr = code.find("zonenum=")
            if znstr > -1:
                num = int(re.search(r'\d+', code[znstr + 8:]).group())
                zones[rx][ry] = num
        elif objname == "obj_phaze_s":
            rooms[rx][ry].append([27, ra_x, ra_y])
        elif objname == "obj_battledoor_s":
            rooms[rx][ry].append([28, ra_x, ra_y])




for child in root:
    if child.tag == "instances":
        parse_instances(child)


def encode_room(x, y):
    start_x = x * 20
    start_y = y * 20
    rx = x
    ry = y

    print("{", end="")
    print(str(zones[x][y]) + ",", end="")
    print("{", end="")
    for y in range(0, 20):
        print("{", end="")
        for x in range(0, 20):
            print(tilemap[start_x + x][start_y + y], end=", ")
        print("},", end="")
        if y != 19:
            print("")
    print("}, {", end="")

    for obj in rooms[rx][ry]:
        print("{", end="")
        print(str(obj[0]), ", ", str(obj[1]), ", ", str(obj[2]), end="")
        print("},", end="")
    print("}},")


print("{", end="")
for y in range(0, 15):
    for x in range(0, 15):
        encode_room(x, y)
print("};")
