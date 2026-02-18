#pragma once
#include "include.h"

namespace KW   
{
    const string CREATE_TABLE = "create_table";
    const string CREATE_ENHANCED_TABLE = "create_enhanced_table";
    const string DROP_TABLE = "drop_table";
    const string INSERT_INTO = "insert into";
    const string SELECT = "select";
    const string UPDATE = "update";
    const string DELETE = "delete";

    const string TYPE_INT = "int";
    const string TYPE_STRING = "string";

    const string REQUIRED = "required";
    const string OPTIONAL = "optional";

    const char FIELD_SEPARATOR = ';';
    const char KEY_VALUE_SEPARATOR = ':';
    const char NEXT_LINE = '\n';
}

namespace MSG
{
    const string TABLE_CREATED_1 = "Success: Table ";
    const string TABLE_CREATED_2 = " created successfully";
    const string TABLE_DELETED_1 = "Success: Table ";
    const string TABLE_DELETED_2 = " deleted successfully";
    const string RECORD_INSERTED = "Success: Record inserted into table successfully";
    const string RECORD_UPDATED = "Success: Records updated in table";

    const string ERR_TABLE_EXISTS = "Error: A table with this name already exists";
    const string ERR_TABLE_NOT_FOUND_1 = "Error: Table ";
    const string ERR_TABLE_NOT_FOUND_2 = " does not exist";
    const string ERR_DUPLICATE_FIELD = "Error: Duplicate field name in table definition";
    const string ERR_REQUIRED_MISSING = "Error: Missing value for required field";
    const string ERR_DUPLICATE_REQUIRED = "Error: Duplicate value for required field";
    const string ERR_NO_MATCHING_RECORD = "Error: No matching records found";
    
}
