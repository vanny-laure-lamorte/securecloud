param(
    [string]$Command
)

$RootDir = Split-Path $PSScriptRoot -Parent
Set-Location $RootDir

switch ($Command) {

    # Docker Stack

    "docker:up" {
        Write-Host "[ Build and start all containers... ]"
        docker compose up --build -d
    }

    "docker:start" {
        Write-Host "[ Start containers without rebuilding...]"
        docker compose up
    }

    "docker:stop" {
        Write-Host "[ Stop all running containers... ]"
        docker compose down
    }

    "docker:logs" {
        Write-Host "[ Show logs for all containers... ]"
        docker compose logs -f
    }

    "docker:logs" {
        Write-Host "[ Show logs for all containers... ]"
        docker compose logs -f
    }

    # Databases

    "docker:auth-db" {
        Write-Host "[ Starting auth-db... ]"
        docker compose up -d auth-db
    }

    "docker:messaging-db" {
        Write-Host "[ Starting messaging-db... ]"
        docker compose up -d messaging-db
    }

    "docker:audit-db" {
        Write-Host "[ Starting audit-db... ]"
        docker compose up -d audit-db
    }

    # Services

    "docker:auth-api" {
        Write-Host "[ Starting auth-service-api... ]"
        docker compose up -d auth-service-api
    }

    "docker:messaging-api" {
        Write-Host "[ Starting messaging-service-api... ]"
        docker compose up -d messaging-service-api
    }

    "docker:audit-api" {
        Write-Host "[ Starting audit-service... ]"
        docker compose up -d audit-service
    }

    "docker:gateway" {
        Write-Host "[ Starting api-gateway... ]"
        docker compose up -d api-gateway
    }

    default {
        Write-Host "Unknown Docker command: $Command"
        exit 1
    }
}
