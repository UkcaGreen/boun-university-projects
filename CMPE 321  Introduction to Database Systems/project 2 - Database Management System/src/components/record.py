import struct

HEADER_PATTERN = 'B'
BODY_PATTERN = 'iiiiiiiiii'

class Record:

    HEADER_SIZE = struct.calcsize(HEADER_PATTERN)
    SIZE = HEADER_SIZE + struct.calcsize(BODY_PATTERN)

    def __init__(self, rawRecord=None):
        if rawRecord == None:
            self.state = 0
            self.fields = [0] * 10
        else:
            self.decode(rawRecord)


    def decode(self, rawRecord):
        rawRecordHeader = rawRecord[0:Record.HEADER_SIZE]
        recordHeader = struct.unpack(HEADER_PATTERN, rawRecordHeader)
        self.state = recordHeader[0]
        rawFieldList = rawRecord[Record.HEADER_SIZE:]
        self.fields = struct.unpack(BODY_PATTERN, rawFieldList)


    def encode(self):
        rawRecordHeader = struct.pack(HEADER_PATTERN, self.state)

        rawFieldList = struct.pack(BODY_PATTERN, 
                                    self.fields[0],
                                    self.fields[1],
                                    self.fields[2],
                                    self.fields[3],
                                    self.fields[4],
                                    self.fields[5],
                                    self.fields[6],
                                    self.fields[7],
                                    self.fields[8],
                                    self.fields[9])

        return rawRecordHeader + rawFieldList





