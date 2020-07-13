import os
from components.page import Page
from components.record import Record
import services.file_manager as fm
import services.utility as util

DIRECTORY = fm.DIRECTORY["data"]

def createRecord(args):
    r = Record()
    typeName = args[0]
    r.state = 1
    r.fields = [int(e) for e in args[1:]] + [0] * (10 - len(args[1:]))

    if not fm.isAvailableFileExist("data", typeName):
        fm.createFile("data", typeName)

    fileList = fm.getFiles("data", typeName)

    for fileName in fileList:
        f = open(DIRECTORY + fileName, "r+b")
        for i in range(0, fm.MAX_NUM_PAGES):
            p = Page(f, i, "data")
            for j in range(0, Page.MAX_NUM_OF_ELEMENTS["data"]):
                if p.elements[j].state == 0:
                    p.elements[j] = r
                    p.numOfElements += 1
                    if p.numOfElements == Page.MAX_NUM_OF_ELEMENTS["data"]:
                        p.state = 1
                    p.write(f, i)
                    f.close()
                    return
                elif r.fields[0] < p.elements[j].fields[0]:
                    r, p.elements[j] = p.elements[j], r
            p.write(f, i)
        f.close()


def deleteRecord(args):
    typeName = args[0]
    primaryKey = int(args[1])
    fileList = fm.getFiles("data", typeName)
    t = Record()
    updateHeaderFlag = True
    for fileName in reversed(fileList):
        f = open(DIRECTORY + fileName, "r+b")
        for i in reversed(range(0, fm.MAX_NUM_PAGES)):
            p = Page(f, i, "data")
            if updateHeaderFlag == True and p.numOfElements > 0:
                updateHeaderFlag = False
                p.numOfElements -= 1
                p.state = 0
            for j in reversed(range(0, Page.MAX_NUM_OF_ELEMENTS["data"])):
                if p.elements[j].state != 0:
                    t, p.elements[j] = p.elements[j], t
                    if t.fields[0] == primaryKey:
                        p.write(f, i)
                        f.close()
                        if fm.isEmpty("data", fileList[-1]):
                            fm.deleteFile("data", fileList[-1])
                        return
            p.write(f, i)
        f.close()


def listRecord(args):
    typeName = args[0]
    numOfFields = util.numberOfFields(typeName)
    fileList = fm.getFiles("data", typeName)
    for fileName in fileList:
        f = open(DIRECTORY + fileName, "r+b")
        for i in range(0, fm.MAX_NUM_PAGES):
            p = Page(f, i, "data")
            for j in range(0, Page.MAX_NUM_OF_ELEMENTS["data"]):
                if p.elements[j].state != 0:
                    print(" ".join([str(i) for i in p.elements[j].fields[0:numOfFields]]))
        f.close()


def searchRecord(args):
    typeName = args[0]
    primaryKey = int(args[1])
    numOfFields = util.numberOfFields(typeName)
    fileList = fm.getFiles("data", typeName)
    for fileName in fileList:
        f = open(DIRECTORY + fileName, "r+b")
        for i in range(0, fm.MAX_NUM_PAGES):
            p = Page(f, i, "data")
            for j in range(0, Page.MAX_NUM_OF_ELEMENTS["data"]):
                if p.elements[j].state != 0 and p.elements[j].fields[0] == primaryKey:
                    print(" ".join([str(i) for i in p.elements[j].fields[0:numOfFields]]))
        f.close()


def updateRecord(args):
    deleteRecord(args)
    createRecord(args)