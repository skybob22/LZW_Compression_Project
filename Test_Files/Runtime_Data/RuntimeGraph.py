import matplotlib.pyplot as plt


def parseFile(filepath,array2d):
    file = open(filepath, "r")
    line = "NULL"
    while line is not "":
        line = file.readline()
        splitLine = line.split()
        if len(splitLine) >= 2:
            array2d[0].append(int(splitLine[0]))
            array2d[1].append(int(splitLine[1]))


HashData = [[], []]
LinkData = [[], []]

parseFile("Hash_Runtime.dat", HashData)
parseFile("Link_Runtime.dat", LinkData)

plt.plot(HashData[0], HashData[1], color="red", label="Hash Table")
plt.plot(LinkData[0], LinkData[1], color="blue", label="Linked List")
plt.yscale("linear")
plt.xlabel("Size of file")
plt.ylabel("Time (ms)")
plt.legend()
plt.title("Hash Table v. Linked List")
plt.show()
