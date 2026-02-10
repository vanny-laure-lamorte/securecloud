param(
    [string]$Command
)

$ScriptDir = $PSScriptRoot

# Write-Color simplifiée avec alignement
function Write-Color {
    param(
        [int]$Type,
        [string]$Text1,
        [string]$Text2="" # Description
    )

    switch ($Type) {
        1 {
            # Titre bleu
            Write-Host $Text1 -ForegroundColor Cyan
        }
        2 {
            Write-Host $Text1 -ForegroundColor Yellow
        }
        3 {
            if ($Text2 -eq "") {
                Write-Host $Text1 -ForegroundColor Green
            } else {
                $paddedCommand = $Text1.PadRight(30)
                Write-Host -NoNewline $paddedCommand -ForegroundColor Green
                Write-Host $Text2 -ForegroundColor White
            }
        }
    }
}

function Show-Help {
    Write-Host ""
    Write-Color 1 "SECURECLOUD CLI - Command Menu"
    Write-Color 1 "Run securecloud <command> to perform actions"
    Write-Host ""
    Write-Color 2 "CMake"
    Write-Color 3 "cmake:configure" "Build the client"
    Write-Color 3 "cmake:clear"     "Remove CMake build directory"
    Write-Host ""
    Write-Color 2 "Docker Stack"
    Write-Color 3 "docker:up"     "Build and start all containers"
    Write-Color 3 "docker:start"  "Start containers without rebuild"
    Write-Color 3 "docker:stop"   "Stop all running containers"
    Write-Color 3 "docker:logs"   "Show logs for all containers"
    Write-Host ""
    Write-Color 2 "Docker Databases"
    Write-Color 3 "docker:auth-db"       "Start auth database"
    Write-Color 3 "docker:messaging-db"  "Start messaging database"
    Write-Color 3 "docker:audit-db"      "Start audit database"
    Write-Host ""
    Write-Color 2 "Docker Services"
    Write-Color 3 "docker:auth-api"       "Start auth API service"
    Write-Color 3 "docker:messaging-api"  "Start messaging API service"
    Write-Color 3 "docker:audit-api"      "Start audit API service"
    Write-Color 3 "docker:gateway"        "Start API gateway"
    Write-Host ""
    Write-Color 2 "Translation"
    Write-Color 3 "translate:scan"       "Scan translation keys"
    Write-Color 3 "translate:update"     "Compile translation files"
    Write-Color 3 "translate:migrate"    "Build client with translations"
    Write-Host ""
    Write-Color 2 "Migrations Management"
    Write-Color 3 "sqitch:version"              "Check Sqitch version"
    Write-Host ""
    Write-Color 2 "Migrations Generation"
    Write-Color 3 "sqitch:generate-audit"      "Generate audit DB migration"
    Write-Color 3 "sqitch:generate-auth"       "Generate auth DB migration"
    Write-Color 3 "sqitch:generate-messaging"  "Generate messaging DB migration"
    Write-Host ""
    Write-Color 2 "Migrations Deployment"
    Write-Color 3 "sqitch:deploy-audit"        "Deploy audit DB migrations"
    Write-Color 3 "sqitch:deploy-auth"         "Deploy auth DB migrations"
    Write-Color 3 "sqitch:deploy-messaging"    "Deploy messaging DB migrations"
    Write-Host ""
}

# No argument → show help
if (-not $Command) {
    Show-Help
    exit 0
}

switch -Regex ($Command) {

    "^cmake:" {
        & "$ScriptDir\cmake.ps1" $Command
        break
    }

    "^docker:" {
        & "$ScriptDir\docker.ps1" $Command
        break
    }

    "^translate:" {
        & "$ScriptDir\translate.ps1" $Command
        break
    }

    "^sqitch:" {
    & "$ScriptDir\sqitch.ps1" $Command $args[0] $args[1]
    break
    }

    default {
        Write-Host "Unknown command: $Command"
        Show-Help
        exit 1
    }
}
