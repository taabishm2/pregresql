I am just making some notes here

*********************
//primnodes.h Var struct
message Var {
    Oid relid = 1;
    int32 attrNumber = 2;
}

This message is representing a column variable in any qualification condition.
Instead of having the string, this Var contains two fields:
1. relid - the relation Oid of the relation to which this column belongs
2. attrnumber - the attribute number of the column in the relation.

