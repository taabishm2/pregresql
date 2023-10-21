

#ifdef __cplusplus
class SundialPostgresRPCClient;
extern SundialPostgresRPCClient client;
#endif

#ifdef __cplusplus
extern "C"
#endif
struct ClassCatResponse {
    int oid;
    char relname[100];
    int relnamespace;
    int reltype;
    int reloftype;
    int relowner;
    int relam;
    int relfilenode;
    int reltablespace;
    int relpages;
    float reltuples;
    int relallvisible;
    int reltoastrelid;
    bool relhasindex;
    bool relisshared;
    char relpersistence[10];
    char relkind[1];
    int relnatts;
    int relchecks;
    bool relhasrules;
    bool relhastriggers;
    bool relhassubclass;
    bool relrowsecurity;
    bool relforcerowsecurity;
    bool relispopulated;
    char relreplident[1];
    bool relispartition;
    int relrewrite;
    int relfrozenxid;
    int relminmxid;
};
// struct ClassCatResponse catResponse;
// #endif

#ifdef __cplusplus
extern "C"
#endif
struct ClassCatResponse getClassCatalog(char message[]);
// int SayHello(char message[]);

// #ifdef __cplusplus
// extern "C"
// #endif
// int sendPlan(char* inputStr);
// //int sendPlan(int plan_width);

#ifdef __cplusplus
extern "C"
#endif
void initSdClient();
