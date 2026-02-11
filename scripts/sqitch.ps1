param(
    [string]$Command,
    [string]$Name,
    [string]$Note
)

$RootDir = Split-Path $PSScriptRoot -Parent

$SqitchImage = "sqitch/sqitch:latest"
$Network = "securecloud_default"

# TODO: init sqitch, generate migration folder if not exists, etc.
# Write-Host "  sqitch:revert                  Revert migrations"
# Write-Host "  sqitch:verify                  Verify migrations"

if (-not $Command -or $Command -eq "sqitch") {
    Show-SqitchHelp
    exit 0
}

switch ($Command) {

    "sqitch:version" {
        Write-Host "[ Checking Sqitch version... ]"
        docker run --rm -it $SqitchImage --version
    }

    "sqitch:generate-audit" {
        Write-Host "[ Generating audit DB migration file... ]"
        docker run --rm -it `
        -v "${PWD}\services\audit-service/migrations:/repo" `
        -w /repo `
        sqitch/sqitch:latest add "$(date +%Y%m%d%H%M%S)_version_migrations" -n "Create table x"
    }

    "sqitch:generate-auth" {
        Write-Host "[ Generating auth DB migration file... ]"
        docker run --rm -it `
        -v "${PWD}\services\auth-service/migrations:/repo" `
        -w /repo `
        sqitch/sqitch:latest add "$(date +%Y%m%d%H%M%S)_version_migrations" -n "Create table x"
    }

    "sqitch:generate-messaging" {
        Write-Host "[ Generating messaging DB migration file... ]"
        docker run --rm -it `
        docker run --rm -it `
        -v "${PWD}\services\messaging-service/migrations:/repo" `
        -w /repo `
        sqitch/sqitch:latest add "$(date +%Y%m%d%H%M%S)_version_migrations" -n "Create table x"
    }

    "sqitch:deploy-audit" {
        Write-Host "[ Deploying audit DB migrations... ]"
        $pw = Get-Content ./secrets/audit_db_password.txt -Raw
        docker run --rm -it `
        -v "${PWD}/services/audit-service/migrations:/repo" `
        --network securecloud_default `
        -e PGPASSWORD="$pw" `
        sqitch/sqitch:latest `
        deploy db:pg://postgres@audit-db:5432/audit_service --chdir /repo/
    }

    "sqitch:deploy-auth" {
        Write-Host "[ Deploying auth DB migrations... ]"
        $pw = Get-Content ./secrets/auth_db_password.txt -Raw
        docker run --rm -it `
        -v "${PWD}/services/auth-service/migrations:/repo" `
        --network securecloud_default `
        -e PGPASSWORD="$pw" `
        sqitch/sqitch:latest `
        deploy db:pg://postgres@auth-db:5432/auth_service --chdir /repo/
    }

    "sqitch:deploy-messaging" {
        Write-Host "[ Deploying messaging DB migrations... ]"
        $pw = Get-Content ./secrets/messaging_db_password.txt -Raw
        docker run --rm -it `
        -v "${PWD}/services/messaging-service/migrations:/repo" `
        --network securecloud_default `
        -e PGPASSWORD="$pw" `
        sqitch/sqitch:latest `
        deploy db:pg://postgres@messaging-db:5432/messaging_service --chdir /repo/
    }

    default {
        Write-Host "Unknown command: $Command"
        exit 1
    }
}
