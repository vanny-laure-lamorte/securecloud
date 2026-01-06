# api-gateway/Dockerfile

FROM drogonframework/drogon:latest

WORKDIR /app

COPY /api-gateway /app/api-gateway

WORKDIR /app/api-gateway

RUN mkdir -p build \
    && cd build \
    && cmake .. \
    && make

EXPOSE 8080

CMD ["./build/api-gateway"]