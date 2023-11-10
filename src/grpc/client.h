

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
