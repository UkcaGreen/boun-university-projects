import struct

HEADER_PATTERN = '=BB10s'
BODY_PATTERN = '=10s10s10s10s10s10s10s10s10s10s'

class Type:

    HEADER_SIZE = struct.calcsize(HEADER_PATTERN)
    SIZE = HEADER_SIZE + struct.calcsize(BODY_PATTERN)

    def __init__(self, rawType=None):
        if rawType == None:
            self.state = 0
            self.numOfFields = 0
            self.name = ""
            self.fieldNames = [""] * 10
        else:
            self.decode(rawType)
          
            
    def decode(self, rawType):
        rawTypeHeader = rawType[:Type.HEADER_SIZE]
        typeHeader = struct.unpack(HEADER_PATTERN, rawTypeHeader)
        self.state = typeHeader[0]
        self.numOfFields = typeHeader[1]
        self.name = typeHeader[2].decode("ascii").replace(" ", "")

        rawFieldNameList = rawType[Type.HEADER_SIZE:]
        fieldNameList = struct.unpack(BODY_PATTERN, rawFieldNameList)
        self.fieldNames = [fieldName.decode("ascii").replace(" ", "") for fieldName in fieldNameList]


    def encode(self):
        rawTypeHeader = struct.pack(HEADER_PATTERN,
                                    self.state,
                                    self.numOfFields,
                                    self.name.ljust(10).encode("ascii"))

        rawFieldNameList = struct.pack(BODY_PATTERN, 
                                        self.fieldNames[0].ljust(10).encode("ascii"),
                                        self.fieldNames[1].ljust(10).encode("ascii"),
                                        self.fieldNames[2].ljust(10).encode("ascii"),
                                        self.fieldNames[3].ljust(10).encode("ascii"),
                                        self.fieldNames[4].ljust(10).encode("ascii"),
                                        self.fieldNames[5].ljust(10).encode("ascii"),
                                        self.fieldNames[6].ljust(10).encode("ascii"),
                                        self.fieldNames[7].ljust(10).encode("ascii"),
                                        self.fieldNames[8].ljust(10).encode("ascii"),
                                        self.fieldNames[9].ljust(10).encode("ascii"))

        return rawTypeHeader + rawFieldNameList