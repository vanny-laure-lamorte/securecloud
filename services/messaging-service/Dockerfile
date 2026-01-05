FROM drogonframework/drogon:latest

WORKDIR /app
COPY common /app/common
COPY services/messaging-service /app/services/messaging-service

WORKDIR /app/services/messaging-service

RUN mkdir -p build \
    && cd build \
    && cmake .. \
    && make

EXPOSE 8082

CMD ["./build/messaging-service"]
