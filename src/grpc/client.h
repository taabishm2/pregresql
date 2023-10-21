

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
int sendPlan(char* inputStr);
//int sendPlan(int plan_width);

#ifdef __cplusplus
extern "C"
#endif
void initClient();
