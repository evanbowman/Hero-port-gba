import xml.etree.ElementTree as ET


tree = ET.parse('rom_quest1.xml')

root = tree.getroot()


tilemap = []

for i in range(0, 300):
    sublist = []
    for j in range(0, 300):
        sublist.append(0)
    tilemap.append(sublist)



def parse_instances(xml_list):
    print(xml_list.tag, xml_list.attrib)

    for inst in xml_list:
        objname = inst[0].text
        x = int(inst[1].attrib["x"])
        y = int(inst[1].attrib["y"])
        tx = int(x / 16)
        ty = int(y / 16)

        if objname == "obj_block":
            tilemap[tx][ty] = 1
        elif objname == "obj_blocktall":
            tilemap[tx][ty] = 1
            tilemap[tx][ty + 1] = 1
            tilemap[tx][ty + 2] = 1
            tilemap[tx][ty + 3] = 1
        elif objname == "obj_blockwide":
            tilemap[tx][ty] = 1
            tilemap[tx + 1][ty] = 1
            tilemap[tx + 2][ty] = 1
            tilemap[tx + 3][ty] = 1
        elif objname == "obj_blockwide2":
            for x in range(0, 6):
                for y in range(0, 3):
                    tilemap[tx + x][ty + y] = 1
        elif objname == "obj_blockwide3":
            for x in range(0, 10):
                if tx + x < 300:
                    tilemap[tx + x][ty] = 1
        elif objname == "obj_blockhuge":
            for x in range(0, 6):
                for y in range(0, 6):
                    if tx + x < 300 and ty + y < 300:
                        tilemap[tx + x][ty + y] = 1



for child in root:
    if child.tag == "instances":
        parse_instances(child)


def print_room(x, y):
    start_x = x * 20
    start_y = y * 20

    for y in range(0, 20):
        for x in range(0, 20):
            print(tilemap[start_x + x][start_y + y], end=", ")
        print("")


print_room(11, 14)




with open('output.csv', 'w') as csv_file:
    for row in tilemap:
        for tile in row:
            csv_file.write(str(tile))
            csv_file.write(", ")
        csv_file.write("\n")
