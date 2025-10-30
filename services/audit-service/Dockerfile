# services/audit-service/Dockerfile

FROM drogonframework/drogon:latest

WORKDIR /app

COPY common /app/common
COPY services/audit-service /app/services/audit-service

WORKDIR /app/services/audit-service

RUN mkdir -p build \
    && cd build \
    && cmake .. \
    && make

EXPOSE 8083

CMD ["./build/audit-service"]