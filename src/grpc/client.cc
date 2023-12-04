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
using postgresGRPC::RunInsertsRequest;
using postgresGRPC::StringList;
using postgresGRPC::InitSchemaRequest;
using postgresGRPC::ColumnDetailRequest;
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

std::string RunSelect(const char* column_names, int search_key) {
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
      return reply.message(); // Return the message from the response
    } else {
      // Construct and return an error message
      std::string error_message = "RPC failed: " + std::to_string(status.error_code()) + ": " + status.error_message();
      std::cout << error_message << std::endl;
      return error_message;
    }
}

std::string RunInsert(const char* column_names, const char* values) {
    RunInsertsRequest request;
    std::string column_name_str(column_names);
    std::stringstream col_name_ss(column_name_str);
    std::string item;

    while (std::getline(col_name_ss, item, ',')) {
        request.add_column_names(item);
    }

    StringList row_values;
    std::string value_str(values);
    std::stringstream value_ss(value_str);
    while (std::getline(value_ss, item, ',')) {
        row_values.add_values(item);
    }
    *request.add_input_rows() = row_values;

    Response reply;
    ClientContext context;

    Status status = stub_->RunInserts(&context, request, &reply);

    if (status.ok()) {
        return reply.message();
    } else {
        std::string error_message = "RPC failed: " + std::to_string(status.error_code()) + ": " + status.error_message();
        std::cout << error_message << std::endl;
        return error_message;
    }
}

std::string InitSchema(const std::vector<std::string>& column_names,
                          const std::vector<int>& column_sizes,
                          const std::vector<std::string>& column_types,
                          const std::string& table_name,
                          const std::string& key_column_name) {
    InitSchemaRequest request;
    request.set_table_name(table_name);
    request.set_key_column_name(key_column_name);

    for (size_t i = 0; i < column_names.size(); ++i) {
        ColumnDetailRequest* column = request.add_columns();
        column->set_name(column_names[i]);
        column->set_size(column_sizes[i]);
        column->set_type(column_types[i]);
    }

    Response reply;
    ClientContext context;

    Status status = stub_->InitSchema(&context, request, &reply);

    if (status.ok()) {
        return reply.message();
    } else {
        std::string error_message = "RPC failed: " + std::to_string(status.error_code()) + ": " + status.error_message();
        std::cout << error_message << std::endl;
        return error_message;
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

extern "C" const char* RunSelect(const char* column_names, int search_key) {
  std::string result = client.RunSelect(column_names, search_key);
  static char resultBuffer[1024];
  strncpy(resultBuffer, result.c_str(), sizeof(resultBuffer));
  resultBuffer[sizeof(resultBuffer) - 1] = '\0'; // Ensure null termination

  return resultBuffer;
}

extern "C" const char* RunInsert(const char* column_names, const char* values) {
    std::string result = client.RunInsert(column_names, values);
    static char resultBuffer[1024];
    strncpy(resultBuffer, result.c_str(), sizeof(resultBuffer));
    resultBuffer[sizeof(resultBuffer) - 1] = '\0'; // Ensure null termination
    return resultBuffer;
}

extern "C" const char* InitSchema(const char* column_names, const char* column_sizes, 
                                  const char* column_types, const char* table_name, 
                                  const char* key_column_name) {
    static char resultBuffer[1024];

    // Parse the column details from the input strings
    std::vector<std::string> names, types;
    std::vector<int> sizes;
    std::stringstream names_stream(column_names), sizes_stream(column_sizes), types_stream(column_types);
    std::string item;

    while (std::getline(names_stream, item, ',')) {
        names.push_back(item);
    }

    while (std::getline(sizes_stream, item, ',')) {
        sizes.push_back(std::stoi(item));
    }

    while (std::getline(types_stream, item, ',')) {
        types.push_back(item);
    }

    std::string result = client.InitSchema(names, sizes, types, table_name, key_column_name);
    strncpy(resultBuffer, result.c_str(), sizeof(resultBuffer));
    resultBuffer[sizeof(resultBuffer) - 1] = '\0'; // Ensure null termination

    return resultBuffer;
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
