import os
from components.page import Page
from components.type import Type
import services.file_manager as fm

DIRECTORY = fm.DIRECTORY["meta"]

def createType(args):
    t = Type()
    t.name = args[0]
    t.state = 1
    t.numOfFields = int(args[1])
    t.fieldNames = args[2:] + [""] * (10 - int(t.numOfFields))

    if not fm.isAvailableFileExist("meta"):
        fm.createFile("meta")

    fileList = fm.getFiles("meta")

    for fileName in fileList:
        f = open(DIRECTORY + fileName, "r+b")
        for i in range(0, fm.MAX_NUM_PAGES):
            p = Page(f, i, "meta")
            for j in range(0, Page.MAX_NUM_OF_ELEMENTS["meta"]):
                if p.elements[j].state == 0:
                    p.elements[j] = t
                    p.numOfElements += 1
                    if p.numOfElements == Page.MAX_NUM_OF_ELEMENTS["meta"]:
                        p.state = 1
                    p.write(f, i)
                    f.close()
                    return
                elif t.name < p.elements[j].name:
                    t, p.elements[j] = p.elements[j], t
            p.write(f, i)
        f.close()


def deleteType(args):
    fileList = fm.getFiles("meta")
    typeToBeDeleted = args[0]
    t = Type()
    updateHeaderFlag = True
    for fileName in reversed(fileList):
        f = open(DIRECTORY + fileName, "r+b")
        for i in reversed(range(0, fm.MAX_NUM_PAGES)):
            p = Page(f, i, "meta")
            if updateHeaderFlag == True and p.numOfElements > 0:
                updateHeaderFlag = False
                p.numOfElements -= 1
                p.state = 0
            for j in reversed(range(0, Page.MAX_NUM_OF_ELEMENTS["meta"])):
                if p.elements[j].state != 0:
                    t, p.elements[j] = p.elements[j], t
                    if t.name == typeToBeDeleted:
                        p.write(f, i)
                        f.close()
                        fm.deleteAllFiles(typeToBeDeleted)
                        if fm.isEmpty("meta", fileList[-1]):
                            fm.deleteFile("meta", fileList[-1])
                        return
            p.write(f, i)
        f.close()

        
def listType(args):
    fileList = fm.getFiles("meta")
    for fileName in fileList:
        f = open(DIRECTORY + fileName, "r+b")
        for i in range(0, fm.MAX_NUM_PAGES):
            p = Page(f, i, "meta")
            for j in range(0, Page.MAX_NUM_OF_ELEMENTS["meta"]):
                if p.elements[j].state != 0:
                    print(p.elements[j].name)
        f.close()
            