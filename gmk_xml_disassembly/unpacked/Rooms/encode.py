import xml.etree.ElementTree as ET
import sys
import math



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
            rooms[rx][ry].append([1, rix, riy])
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




for child in root:
    if child.tag == "instances":
        parse_instances(child)


def encode_room(x, y):
    start_x = x * 20
    start_y = y * 20
    rx = x
    ry = y

    print("{", end="")
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
