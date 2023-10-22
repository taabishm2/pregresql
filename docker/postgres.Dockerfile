FROM grpc-base

ENV PATH="/root/.local/bin:${PATH}"
ENV LD_LIBRARY_PATH="/pregresql/src/grpc/cmake/build:${LD_LIBRARY_PATH}"

RUN apt-get update && apt-get install -y git cmake build-essential autoconf libtool pkg-config g++ libre2-dev libssl-dev zlib1g-dev && \
    git clone https://github.com/taabishm2/pregresql.git && cd /pregresql/src/grpc && rm -rf cmake && mkdir -p cmake/build && cd cmake/build && \
    cd /pregresql/src/grpc/cmake/build && cmake ../.. && make -j 8 && \
    apt-get update && apt-get install -y libicu-dev libreadline-dev bison flex && \
    cd /pregresql && ./configure && make -j 8 && make install && \
    adduser postgres && rm -rf /usr/local/pgsql/data && mkdir /usr/local/pgsql/data && chown postgres /usr/local/pgsql/data && \
    su - postgres -c "/usr/local/pgsql/bin/initdb -D /usr/local/pgsql/data"

CMD cd /postgres/src/grpc && mkdir -p cmake/build && cd cmake/build && cmake ../.. && make -j 8 && \
    su - postgres -c "/usr/local/pgsql/bin/pg_ctl -D /usr/local/pgsql/data -l ./postgres.logs start && sleep 5" && \
    su - postgres -c "/usr/local/pgsql/bin/createdb test && /usr/local/pgsql/bin/psql -c 'CREATE TABLE users(name TEXT);' test" && \
    su - postgres -c "/usr/local/pgsql/bin/psql -c \"INSERT INTO users (name) VALUES ('John');\" test" && \
    su - postgres -c "/usr/local/pgsql/bin/psql -c 'SELECT * FROM users' test"
