/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "client.h"


//  #include "/users/pankajkr/postgresql/src/include/nodes/plannodes.h"


#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using postgresGRPC::Greeter;
using postgresGRPC::HelloReply;
using postgresGRPC::HelloRequest;
using postgresGRPC::PlannedStmtRPC;
using postgresGRPC::PlannedStmt;

using postgresGRPC::RunSelectRequest;
using postgresGRPC::Response;


using namespace std;

#include "string.h"

class GreeterClient {
 public:
  
  void create_stub(std::shared_ptr<Channel> channel) {
    stub_ = Greeter::NewStub(channel);
  }

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string SayHello(const std::string& msg) {

    // Data we are sending to the server.
    HelloRequest request;
    request.set_name(msg);

    // Container for the data we expect from the server.
    HelloReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->SayHello(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

  std::string sendPlan(int plan_width) {
    // Data we are sending to the server.
    PlannedStmtRPC request;
    request.set_plan_width(plan_width);

    // Container for the data we expect from the server.
    HelloReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->sendPlan(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

  std::string executeOnServer(const std::string& plannedStmtString) {
    PlannedStmt request;
    request.set_plantree(plannedStmtString);

    HelloReply reply;
    ClientContext context;

    Status status = stub_->executeOnServer(&context, request, &reply);

    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }


  ////////////////////////////////////////////////////////////////////

  int RunSelect(const char* column_names, int search_key) {
    // Create and populate the RunSelectRequest
    RunSelectRequest request;
    std::string column_name_str(column_names);
    std::stringstream ss(column_name_str);
    std::string item;
    while (std::getline(ss, item, ',')) {
      request.add_column_names(item);
    }
    request.set_search_key(search_key);

    // Prepare the response and context
    Response reply;
    ClientContext context;

    // Make the RPC call
    Status status = stub_->RunSelect(&context, request, &reply);

    // Handle the response
    if (status.ok()) {
      std::cout << "RPC success: " << reply.message() << std::endl;
      return 0; // Success
    } else {
      std::cout << "RPC failed: " << status.error_code() << ": " << status.error_message() << std::endl;
      return 1; // Failure
    }
  }

 private:
  std::unique_ptr<Greeter::Stub> stub_;
};

extern "C" int SayHello(char message[]) {
    // RPC is created and response is stored
    string msg(message);
    client.SayHello(msg);
    return 0;
}


extern "C" int executeOnServer(char plannedStmtString[]) {
    string msg(plannedStmtString);
    client.executeOnServer(plannedStmtString);
    return 0;
}

//////////////////////////////////////////////////////////////

extern "C" int RunSelect(const char* column_names, int search_key) {
  client.RunSelect(column_names, search_key);
  return 0;
}

//extern "C" int sendPlan(int plan_width) {
//    // RPC is created and response is stored
//    string msg;
//    msg = "sendPlan was called";
//    client.SayHello(msg);
//    // client.sendPlan(plan_width);
//    client.sendPlan(plan_width);
//    return 0;
//}

extern "C" int sendPlan(char* inputStr) {
    // RPC is created and response is stored
    string msg(inputStr);
    // msg = "sendPlan was called";
    client.SayHello(msg);
    // client.sendPlan(62);
    return 0;
}

extern "C" void initClient() {


   std::ofstream outputFile("/pregres.log"); // create a new file called "example.txt"

  if (outputFile.is_open()) { // check if the file is opened successfully
    outputFile << "Init function was called"; // write "Hello, world!" to the file
    outputFile.close(); // close the file
    std::cout << "Successfully wrote to the file." << std::endl;
  }
  else {
    std::cout << "Unable to open file." << std::endl;
  }

    std::string target_address("127.0.0.1:50051");
    std::shared_ptr<Channel> channel = grpc::CreateChannel(target_address,
                          // Indicate when channel is not authenticated
                          grpc::InsecureChannelCredentials());
    
    client.create_stub(channel);
}

GreeterClient client;
