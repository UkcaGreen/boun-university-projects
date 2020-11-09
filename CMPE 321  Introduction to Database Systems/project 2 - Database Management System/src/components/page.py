import struct
from components.record import Record
from components.type import Type

class Page:

    HEDADER_SIZE = 2
    MAX_NUM_OF_ELEMENTS = {
        "data": 40,
        "meta": 15
    }
    UNIT_ELEMENT = {
        "data": Record,
        "meta": Type
    }
    SIZE = {
        "data": HEDADER_SIZE + Record.SIZE * MAX_NUM_OF_ELEMENTS["data"],
        "meta": HEDADER_SIZE + Type.SIZE * MAX_NUM_OF_ELEMENTS["meta"]
    }

    def __init__(self, file=None, index=0, fileType=None):
        self.fileType = fileType
        if file == None:
            self.state = 0
            self.numOfElements = 0
            self.elements = [Page.UNIT_ELEMENT[self.fileType]()] * Page.MAX_NUM_OF_ELEMENTS[fileType]
        else:
            self.read(file, index)
    

    def decode(self, rawPage):
        rawPageHeader = rawPage[0:Page.HEDADER_SIZE]
        self.decodePageHeader(rawPageHeader)

        rawPageBody = rawPage[Page.HEDADER_SIZE:]
        self.decodePageBody(rawPageBody)


    def decodePageHeader(self, rawPageHeader):
        pageHeader = struct.unpack('=BB', rawPageHeader)
        self.state = pageHeader[0]
        self.numOfElements = pageHeader[1]
    

    def decodePageBody(self, rawPageBody):
        unitElementSize = Page.UNIT_ELEMENT[self.fileType].SIZE
        rawElementList = [rawPageBody[i:i+unitElementSize] for i in range(0, len(rawPageBody), unitElementSize)]
        self.elements = [Page.UNIT_ELEMENT[self.fileType](rawElement) for rawElement in rawElementList]


    def encode(self):
        rawPageHeader = self.encodePageHeader()
        rawPageBody = self.encodePageBody()
        return rawPageHeader + rawPageBody


    def encodePageHeader(self):
        return struct.pack('=BB', 
                            self.state,
                            self.numOfElements)


    def encodePageBody(self):
        rawElementList = [element.encode() for element in self.elements]

        rawPageBody = b""
        for rawElement in rawElementList:
            rawPageBody += rawElement
        return rawPageBody


    def write(self, file, index):
        rawPage = self.encode()
        file.seek(Page.SIZE[self.fileType] * index)
        file.write(rawPage)


    def read(self, file, index):
        file.seek(Page.SIZE[self.fileType] * index)
        rawPage = file.read(Page.SIZE[self.fileType])
        self.decode(rawPage)