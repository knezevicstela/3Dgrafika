import math



def coordinates(): 

    l = []

    for i in range (0, 12):

        x = round(math.cos(i*2*math.pi/12), 6)

        y = round(math.sin(i*2*math.pi/12), 6)

        l.append((x,y))

    for i in range (0, 12):

        x = round(math.cos(i*2*math.pi/12), 6)

        y = round(math.sin(i*2*math.pi/12), 6)

        l.append((x,y))

    return l





def main():

    list_normals = [(0, 0, 1), (0, 0, -1)]

    f = open("cylinder.obj", 'w')

    for i in range(0, 12):

        f.write("v" + " " + str(coordinates()[i][0]) + " " + str(coordinates()[i][1]) + " " + "0" + "\n")

    for i in range(0, 12):

        f.write("v" + " " + str(coordinates()[i][0]) + " " + str(coordinates()[i][1]) + " " + "2" + "\n")

    f.write("\n")

    f.write("vn" + " " + str(list_normals[0][0]) +  " " + str(list_normals[0][1]) + " " + str(list_normals[0][2]) + "\n")

    f.write("vn" + " " + str(list_normals[1][0]) +  " " + str(list_normals[1][1]) + " " + str(list_normals[1][2]) + "\n")

    f.close()





if __name__ == "__main__":

    main()