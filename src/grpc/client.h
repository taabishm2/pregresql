

#ifdef __cplusplus
class GreeterClient;
extern GreeterClient client;
#endif

#ifdef __cplusplus
extern "C"
#endif
int SayHello(char message[]);

#ifdef __cplusplus
extern "C"
#endif
int executeOnServer(char* plannedStmtString);

#ifdef __cplusplus
extern "C"
#endif
int sendPlan(char* inputStr);

#ifdef __cplusplus
extern "C"
#endif
void initClient();

///////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
#endif
const char* RunSelect(const char* column_names, int search_key);

#ifdef __cplusplus
extern "C"
#endif
const char* RunInsert(const char* column_names, const char* values);

#ifdef __cplusplus
extern "C"
#endif
const char* InitSchema(const char* column_names, const char* column_sizes, const char* column_types, const char* table_name, const char* key_column_name) 
