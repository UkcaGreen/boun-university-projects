import sys
import operations.ddl as DDL
import operations.dml as DML
import services.file_manager as fm

operations = { 
    "create" : { "record" : DML.createRecord, "type" : DDL.createType },
    "delete" : { "record" : DML.deleteRecord, "type" : DDL.deleteType },
    "list"   : { "record" : DML.listRecord,   "type" : DDL.listType   },
    "update" : { "record" : DML.updateRecord},
    "search" : { "record" : DML.searchRecord}
    }

inputFile = sys.argv[1]
outputFile = sys.argv[2]
sys.stdout = open(outputFile, 'w')

with open(sys.argv[1], "r+") as f:
    lines = f.readlines()
    for line in lines:
        tokens = line.split()
        operations[tokens[0]][tokens[1]](tokens[2:])