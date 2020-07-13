import os
import re
from components.page import Page

script_dir = os.path.dirname(__file__)
rel_meta_path = "..\\storage\\meta\\"
rel_data_path = "..\\storage\\data\\"
abs_meta_path = os.path.join(script_dir, rel_meta_path)
abs_data_path = os.path.join(script_dir, rel_data_path)

os.makedirs(abs_meta_path, exist_ok=True)
os.makedirs(abs_data_path, exist_ok=True)

DIRECTORY = {
    "meta": abs_meta_path,
    "data": abs_data_path
}

MAX_NUM_PAGES = 10

def createFile(fileType, typeName = "syscat"):
    fileName = generateFileName(fileType, typeName)
    f = open(DIRECTORY[fileType] + fileName, "w+").close()
    f = open(DIRECTORY[fileType] + fileName, "r+b")
    for i in range(0, MAX_NUM_PAGES):
        Page(fileType=fileType).write(f, i)
    f.close()


def getFiles(fileType, typeName = "syscat"):
    regex = re.compile(typeName + "\\.[0-9]{10}$")
    allFiles = os.listdir(DIRECTORY[fileType])
    typeFiles = [name for name in allFiles if regex.match(name)]
    return typeFiles


def deleteAllFiles(typeName):
    filesToBeDeleted = getFiles("data", typeName)
    for fileName in filesToBeDeleted:
        deleteFile("data", fileName)


def deleteFile(fileType, fileName):
    os.remove(DIRECTORY[fileType] + fileName)


def isEmpty(fileType, fileName):
    f = open(DIRECTORY[fileType] + fileName, "r+b")
    p = Page(f,0,fileType)
    f.close()
    if p.numOfElements == 0:
        return True
    else:
        return False


def generateFileName(fileType, typeName = "syscat"):
    typeFiles = getFiles(fileType, typeName)
    if len(typeFiles) == 0:
        return typeName + "." + "0".rjust(10, "0")
    else:
        fileNumbers = [int(name[-10:]) for name in typeFiles]
        maxNumber = max(fileNumbers)
        newFileNumber = maxNumber + 1
        return typeName + "." + str(newFileNumber).rjust(10, "0")


def isAvailableFileExist(fileType, typeName = "syscat"):
    fileList = getFiles(fileType, typeName)
    if fileList != []:
        lastFile = fileList[-1]
        f = open(DIRECTORY[fileType] + lastFile, "r+b")
        for i in range(0, MAX_NUM_PAGES):
            p = Page(f,i,fileType)
            if p.state == 0:
                return True
    return False
