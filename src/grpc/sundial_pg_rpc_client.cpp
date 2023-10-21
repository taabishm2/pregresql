#include <iostream>
#include <memory>
#include <string>
#include "sundial_client.h"

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

// #include "sundial_pg_rpc_client.h"
#include <grpcpp/grpcpp.h>
#include "sundial.grpc.pb.h"
#include "sundial.pb.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using sundial_rpc::ClassCatalogRequest;
using sundial_rpc::ClassCatalogResponse;
using sundial_rpc::SundialPostgresRPC;

using namespace std;
#include "string.h"

// ABSL_FLAG(std::string, target, "localhost:50051", "Server address");
struct ClassCatResponse catResponse;

class SundialPostgresRPCClient {
 public:

 void create_stub(std::shared_ptr<Channel> channel) {
    stub_ = SundialPostgresRPC::NewStub(channel);
  }
  // SundialPostgresRPCClient(std::shared_ptr<Channel> channel)
  //     : stub_(SundialPostgresRPC::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
struct ClassCatResponse getClassCatalog(const std::string& user) {
    // Data we are sending to the server.
    ClassCatalogRequest request;
    request.set_relname(user);

    // Container for the data we expect from the server.
    ClassCatalogResponse reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->getClassCatalog(&context, request, &reply);
    std::cout<<"REPLY!! - "<<reply.relname()<<std::endl;

    //TODO: convert reply to ClassCatResponse
    catResponse.oid = reply.oid();
    strcpy(catResponse.relname, reply.relname().c_str());
    std:cout<<"CAtRESPOnse relname = "<<catResponse.relname<<std::endl;
    catResponse.relnamespace = reply.relnamespace();
    catResponse.reltype = reply.reltype();
    catResponse.reloftype = reply.reloftype();
    catResponse.relowner = reply.relowner();
    catResponse.relam = reply.relam();
    catResponse.relfilenode = reply.relfilenode();
    catResponse.reltablespace = reply.reltablespace();
    catResponse.relpages = reply.relpages();
    catResponse.reltuples = reply.reltuples();
    catResponse.relallvisible = reply.relallvisible();
    catResponse.reltoastrelid = reply.reltoastrelid();
    catResponse.relhasindex = reply.relhasindex();
    catResponse.relisshared = reply.relisshared();
    std::cout<<"GRPC : relpersistence = "<< reply.relpersistence().c_str();
    strcpy(catResponse.relpersistence, reply.relpersistence().c_str());
    strcpy(catResponse.relkind, reply.relkind().c_str());
    catResponse.relnatts = reply.relnatts();
    catResponse.relchecks = reply.relchecks();
    catResponse.relhasrules = reply.relhasrules();
    catResponse.relhastriggers = reply.relhastriggers();
    catResponse.relhassubclass = reply.relhassubclass();
    catResponse.relrowsecurity = reply.relrowsecurity();
    catResponse.relforcerowsecurity = reply.relforcerowsecurity();
    catResponse.relispopulated = reply.relispopulated();
    strcpy(catResponse.relreplident, reply.relreplident().c_str());
    catResponse.relispartition = reply.relispartition();
    catResponse.relrewrite = reply.relrewrite();
    catResponse.relfrozenxid = reply.relfrozenxid();
    catResponse.relminmxid = reply.relminmxid();

    // Act upon its status.
    if (status.ok()) {
      // return reply.relname();
      return catResponse;
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      // return "RPC failed";
      return catResponse;
    }
  }

 private:
  std::unique_ptr<SundialPostgresRPC::Stub> stub_;
};

extern "C" struct ClassCatResponse getClassCatalog(char message[]) {
    // RPC is created and response is stored
    string msg(message);
    return client.getClassCatalog(msg);
    // catResponse.set_() = x.smthn();
    // return 0;
}


extern "C" void initSdClient() {
//   absl::ParseCommandLine(argc, argv);
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint specified by
  // the argument "--target=" which is the only expected argument.
//   std::string target_str = absl::GetFlag(FLAGS_target);
  // We indicate that the channel isn't authenticated (use of
  // InsecureChannelCredentials()).
  // SundialPostgresRPCClient greeter(
  //     grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
  // std::string user("TABLE_NAME");
  // std::string reply = greeter.getClassCatalog(user);
  // std::cout << "SundialPostgres received: " << reply << std::endl;

//   return 0;
  std::cout<<"INIT SD CLIENT!!!"<<std::endl;
  std::string target_address("127.0.0.1:50052");
  std::shared_ptr<Channel> channel = grpc::CreateChannel(target_address,
                          // Indicate when channel is not authenticated
                          grpc::InsecureChannelCredentials());
  std::cout<<"INIT SD CLIENT   Channel created!!!"<<std::endl;
  client.create_stub(channel);
  std::cout<<"INIT SD CLIENT   Stub created!!!"<<std::endl;
}

// int main() {
//   initSdClient();
//   char message[5] = "yolo";
//   string msg(message);
//   std::string reply = client.getClassCatalog(msg);
//   std::cout << "SundialPostgres received: " << reply << std::endl;
// }
SundialPostgresRPCClient client;