FROM drogonframework/drogon:latest

WORKDIR /app
COPY common /app/common
COPY services/auth-service /app/services/auth-service

WORKDIR /app/services/auth-service

RUN mkdir -p build \
    && cd build \
    && cmake .. \
    && make

EXPOSE 8081

CMD ["./build/auth-service"]
