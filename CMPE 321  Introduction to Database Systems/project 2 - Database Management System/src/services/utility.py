from components.page import Page
from components.type import Type
import services.file_manager as fm

DIRECTORY = fm.DIRECTORY["meta"]

def numberOfFields(typeName):
    fileList = fm.getFiles("meta")
    for fileName in fileList:
        f = open(DIRECTORY + fileName, "r+b")
        for i in range(0, fm.MAX_NUM_PAGES):
            p = Page(f, i, "meta")
            for j in range(0, Page.MAX_NUM_OF_ELEMENTS["meta"]):
                if p.elements[j].state != 0:
                    if p.elements[j].name == typeName:
                        return p.elements[j].numOfFields
        f.close()
    return -1