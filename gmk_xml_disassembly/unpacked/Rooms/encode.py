import xml.etree.ElementTree as ET


tree = ET.parse('rom_quest2.xml')

root = tree.getroot()


tilemap = []

for i in range(0, 300):
    sublist = []
    for j in range(0, 300):
        sublist.append(0)
    tilemap.append(sublist)



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



for child in root:
    if child.tag == "instances":
        parse_instances(child)


def encode_room(x, y):
    start_x = x * 20
    start_y = y * 20

    print("{{", end="")
    for y in range(0, 20):
        print("{", end="")
        for x in range(0, 20):
            print(tilemap[start_x + x][start_y + y], end=", ")
        print("},", end="")
        if y != 19:
            print("")
    print("}},")


#encode_room(6, 0)


print("{", end="")
for y in range(0, 15):
    for x in range(0, 15):
        encode_room(x, y)
print("};")



with open('output.csv', 'w') as csv_file:
    for row in tilemap:
        for tile in row:
            csv_file.write(str(tile))
            csv_file.write(", ")
        csv_file.write("\n")
