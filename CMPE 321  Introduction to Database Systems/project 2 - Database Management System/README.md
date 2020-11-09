## Storage Management System

### Project Description

This is a storage management system implementation project. It basically contains two main parts which are system catalog and data storage units. Purpose of the system catalog is storing the meta data. In the scope of this project, meta data is types that can be defined by user. The other part is data storage units that preserve actual data. There are 8 different operations that supported by the storage management system which are *create/delete/list* type, and *create/delete/search/update/list* record.

### How to Run

In order to run this program you can basically  use `python3 src/storageManager.py <input-file> <output-file>`  command. User shall provide an input file and an output file. Operations in the input file is performed and the output is written into output file.

### How to Use

Program in total supports 8 operations (3 DDL, 5 DML). Syntax to use these operations are provided in the below tables.

* DDL Operations

| Operation | Input Format                                                 | Output Format                                                |
| --------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Create    | <font size="1">create type \<type-name> \<number-of-fields> \<field1-name> \<field2-name>...</font> | <font size="1"> None </font>                                 |
| Delete    | <font size="1"> delete type \<type-name></font>                | <font size="1"> None </font>                                 |
| List      | <font size="1"> list type</font>                             | <font size="1">\<type1-name><br />\<type2-name><br />...</font> |

* DML Operations

| Operation | Input Format                                                 | Output Format                                                |
| --------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Create    | <font size="1">create record \<type-name> \<field1-value> \<field2-value>...</font> | <font size="1"> None </font>                                 |
| Delete    | <font size="1">delete record \<type-name> \<primary-key></font> | <font size="1"> None </font>                                 |
| Search    | <font size="1">search record \<type-name> \<primary-key></font> | <font size="1"> \<field1-value>\<field2-value>...</font>     |
| Update    | <font size="1">update record \<type-name> \<primary-key> \<field2-value> \<field3-value>...</font> | <font size="1"> None </font>                                 |
| List      | <font size="1">list record \<type-name></font>               | <font size="1">\<record1-field1-value>\<record1-field2-value>...<br />\<record2-field1-value>\<record2-field2-value>...<br />...</font> |

### Project Structure

Entrance point of the project is `src/storageManager.py`.  Subdirectories of the project is described in the following sections.

- #### Operations

  * **DDL:** Contains meta knowledge related operations.
  
  * **DML:** Contains data related operations.
  
- #### Structures

  * **Page:** Page object contains either records or types. It is responsible for encoding/decoding/reading/writing a page.
  * **Record:** Record object is responsible for encoding and decoding records.
  * **Type:** Type object is responsible for encoding and decoding types.

- #### Services

  This section of the program contains file manager and the helper functions.

- #### Storage

  This section of the program contains the data and meta (system catalog) files. 



