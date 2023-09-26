import sys
import grpc
import kvstore_pb2
import kvstore_pb2_grpc

from random import randint

CHANNEL = grpc.insecure_channel(f'127.0.0.1:5440')
STUB = kvstore_pb2_grpc.KvStoreStub(CHANNEL)

def put(key, value):
    return STUB.Set(kvstore_pb2.SetRequest(key=key, value=value))

def get(key):
    return STUB.Get(kvstore_pb2.GetRequest(key=key))

# Usage
# Set request: python3 client.py set <key> <value>
# Get request: python3 client.py get <key>
if __name__ == '__main__':
    if sys.argv[1].lower() == 'set':
        response = put(sys.argv[2], sys.argv[3])
        print(f"[Client] SET {response.key}:{response.value}")
    
    elif sys.argv[1].lower() == 'get':
        response = get(sys.argv[2])
        print(f"[Client] GET {response.key}:{response.value}")