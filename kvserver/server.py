import grpc
import kvstore_pb2
import kvstore_pb2_grpc

from concurrent import futures

KV_STORE = dict()

class KvStoreServicer(kvstore_pb2_grpc.KvStoreServicer):

    def Set(self, request, context):
        print(f"[Server] SET {request.key}:{request.value}")
        KV_STORE[request.key] = request.value
        return kvstore_pb2.KvResponse(key=request.key, value=request.value)

    def Get(self, request, context):
        print(f"[Server] GET {request.key}")
        value = KV_STORE.get(request.key, None)
        return kvstore_pb2.KvResponse(key=request.key, value=value)

def server():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=1))
    kvstore_pb2_grpc.add_KvStoreServicer_to_server(KvStoreServicer(), server)
    server.add_insecure_port('[::]:5440')
    print("Server listening on port:5440...")
    server.start()
    server.wait_for_termination()
    print("Server terminated")


if __name__ == '__main__':
    server()